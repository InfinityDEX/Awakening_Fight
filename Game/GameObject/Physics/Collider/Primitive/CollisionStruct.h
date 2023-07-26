#ifndef COLLISION_S
#define COLLISION_S
#include <SimpleMath.h>

class CollisionStruct
{

public:
	// 球の構造体
	struct Sphere {
		DirectX::SimpleMath::Vector3 c; // 球の中心 
		float r;   // 球の半径 
	};

	// カプセルの構造体
	struct Capsule {
		DirectX::SimpleMath::Vector3 a; // 中間部の線分の開始点 
		DirectX::SimpleMath::Vector3 b; // 中間部の線分の終了点 
		float r;   // 半径 
	};

	// 直線の構造体 
	struct Line
	{
		DirectX::SimpleMath::Vector3 p; // スタート位置 
		DirectX::SimpleMath::Vector3 d; // 方向ベクトル 
	};

	// 箱の構造体
	struct Cube
	{
		DirectX::SimpleMath::Vector3 pos;
		DirectX::SimpleMath::Vector3 size;
	};
public:
	// 球と球の衝突判定関数
	static bool HitCheck_Sphere2Sphere(const Sphere& sphere1, const Sphere& sphere2)
	{
		// 中心間の距離の平方を計算
		DirectX::SimpleMath::Vector3 d = sphere1.c - sphere2.c;
		float dist2 = d.Dot(d);
		// 平方した距離が平方した半径の合計よりも小さい場合に球は交差している
		float radiusSum = sphere1.r + sphere2.r;
		return dist2 <= radiusSum * radiusSum;
	}

	// 直線とAABBの衝突判定関数
	static bool HitCheck_Line2AABB(const Line& line, const Cube& box, const DirectX::SimpleMath::Matrix matAABB
		, float* t = nullptr, DirectX::SimpleMath::Vector3* colPos = nullptr)
	{
		// 方向ベクトル正規化
		Line nLine = line;
		nLine.d.Normalize();
		// 直線を境界ボックスの空間へ移動
		DirectX::SimpleMath::Matrix invMat = matAABB.Invert();
		DirectX::SimpleMath::Vector3 p_l, dir_l;
		p_l = DirectX::SimpleMath::Vector3::Transform(nLine.p, invMat);
		invMat._41 = 0.0f;
		invMat._42 = 0.0f;
		invMat._43 = 0.0f;
		dir_l = DirectX::SimpleMath::Vector3::Transform(nLine.d, invMat);
		// 交差判定
		float p[3], d[3], min[3], max[3];
		memcpy(p, &p_l, sizeof(DirectX::SimpleMath::Vector3));
		memcpy(d, &dir_l, sizeof(DirectX::SimpleMath::Vector3));
		min[0] = box.pos.x - box.size.x / 2;
		min[1] = box.pos.y - box.size.y / 2;
		min[2] = box.pos.z - box.size.z / 2;
		max[0] = box.pos.x + box.size.x / 2;
		max[1] = box.pos.y + box.size.y / 2;
		max[2] = box.pos.z + box.size.z / 2;
		float tmp_t = -FLT_MAX;
		float t_max = FLT_MAX;
		for (int i = 0; i < 3; ++i) {
			if (abs(d[i]) < FLT_EPSILON) {
				if (p[i] < min[i] || p[i] > max[i])
					return false; // 交差していない
			}
			else {
				// スラブとの距離を算出
				// t1が近スラブ、t2が遠スラブとの距離
				float odd = 1.0f / d[i];
				float t1 = (min[i] - p[i]) * odd;
				float t2 = (max[i] - p[i]) * odd;
				if (t1 > t2) {
					float tmp = t1; t1 = t2; t2 = tmp;
				}
				if (t1 > tmp_t) tmp_t = t1;
				if (t2 < t_max) t_max = t2;
				// スラブ交差チェック
				if (tmp_t >= t_max)
					return false;
			}
		}
		if (t)
		{
			*t = tmp_t;
		}
		// 交差している
		if (colPos) {
			*colPos = nLine.p + tmp_t * nLine.d;
		}
		return true;
	}

	// 球とカプセルの衝突判定関数 
	static bool HitCheck_Sphere2Capsule(Sphere sphere, Capsule capsule)
	{
		// 球の中心とカプセルの中心の線分との距離の平方を計算
		float dist2 = SqDistPointSegment(capsule.a, capsule.b, sphere.c);
		float radius = sphere.r + capsule.r; return dist2 <= radius * radius;
	}

	// 点cと線分abの間の距離の平方を返す関数 
	// a: 線分の始点 
	// b: 線分の終点 
	// c: 点 
	// 返り値: 点cと線分abの間の距離の平方 
	static float SqDistPointSegment(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c) {
		DirectX::SimpleMath::Vector3 ab = b - a;
		DirectX::SimpleMath::Vector3 ac = c - a;
		DirectX::SimpleMath::Vector3 bc = c - b;
		float e = ac.Dot(ab);
		if (e <= 0.0f) return ac.Dot(ac);
		float f = ab.Dot(ab);
		if (e >= f) return bc.Dot(bc);
		return ac.Dot(ac) - e * e / f;
	}

	// カプセルとカプセルの衝突判定関数
	static bool HitCheck_Capsule2Capsule(Capsule capsule1, Capsule capsule2)
	{
		float s, t;
		DirectX::SimpleMath::Vector3 c1, c2;
		// カプセルの中心の線分間の距離の平方を計算
		float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b, capsule2.a, capsule2.b, s, t, c1, c2);
		float radius = capsule1.r + capsule2.r;
		return dist2 <= radius * radius;
	}
	// クランプ関数
	static float Clamp(float n, float min, float max)
	{
		if (n < min) return min;
		if (n > max) return max;
		return n;
	}

	// ボックスとボックスの衝突判定関数
	static bool HitCheck_Box2Box(Cube box1, Cube box2)
	{
		if (abs(box1.pos.x - box2.pos.x) > (box1.size.x / 2 + box2.size.x / 2)) return false;
		if (abs(box1.pos.y - box2.pos.y) > (box1.size.y / 2 + box2.size.y / 2)) return false;
		if (abs(box1.pos.z - box2.pos.z) > (box1.size.z / 2 + box2.size.z / 2)) return false;
		return true;
	}
	// ２つの線分の最短距離の平方を返す関数
	// p1: 線分1の始点 q1: 線分1の終点
	// p2: 線分2の始点 q2: 線分2の終点
	// s: 線分１上の最短位置を表す係数 t: 線分2上の最短位置を表す係数
	// c1: 線分１上の最短距離の位置 c2: 線分2上の最短距離の位置
	// 返り値: ２つの線分の最短距離の平方
	static float ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 q1, DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 q2, float &s, float &t, DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
	{
		DirectX::SimpleMath::Vector3 d1 = q1 - p1; DirectX::SimpleMath::Vector3 d2 = q2 - p2; DirectX::SimpleMath::Vector3 r = p1 - p2;
		float a = d1.Dot(d1); float e = d2.Dot(d2); float f = d2.Dot(r);
		if (a <= FLT_EPSILON && e <= FLT_EPSILON)
		{
			s = t = 0.0f;
			c1 = p1;
			c2 = p2;
			return (c1 - c2).Dot(c1 - c2);
		}
		if (a <= FLT_EPSILON)
		{
			s = 0.0f;
			t = f / e;
			t = Clamp(t, 0.0f, 1.0f);
		}
		else
		{
			float c = d1.Dot(r);
			if (e <= FLT_EPSILON)
			{
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else
			{
				float b = d1.Dot(d2);
				float denom = a * e - b * b;
				if (denom != 0.0f)
				{
					s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
				}
				else
				{
					s = 0.0f;
				}
				float tnom = (b * s + f);
				if (tnom < 0.0f)
				{
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else if (tnom > e)
				{
					t = 1.0f;
					s = Clamp((b - c) / a, 0.0f, 1.0f);
				}
				else
				{
					t = tnom / e;
				}
			}
		}
		c1 = p1 + d1 * s;
		c2 = p2 + d2 * t;
		return (c1 - c2).Dot(c1 - c2);
	}
};

#endif