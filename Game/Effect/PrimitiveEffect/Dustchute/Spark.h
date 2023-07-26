#pragma once
#include <Game\Effect\DefaultEffect.h>
#include <Game\Effect\AnimationEffect.h>
#include <Game\3rdParty\DeviceResources.h>

class Spark : public AnimationEffect
{
public:
	Spark(Vector3 position,Vector3 direction, float range, int sparkNum, float speed = 1.0f, float length = 1.0f);
	~Spark() = default;
	void Create(DX::DeviceResources* deviceResources) override;
	void Update(DX::StepTimer timer) override;
	void Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj);
private:
	// パーティクルを一つ生成
	void MakeSpark();
	//それぞれのクラスが固有の定数バッファを持つ
	struct ConstBuffer
	{
		Matrix		matWorld;
		Matrix		matView;
		Matrix		matProj;
		Vector4		Time;
		Vector4		Center;
		Vector4		Length;
	};

	// 固定バッファ
	ConstBuffer m_cbuff;
	// 粒子の数のカウンター
	int m_sparkNum;
	// 粒子が拡散する角度
	const float m_range;
	// 放射の軸ベクトル
	const Vector3 m_direction;
	// パーティクルの移動スピード
	const float m_speed;
	// 離れられる長さ
	const float m_length;
	// 生成時間のインターバル
	const int m_interval;
	// 生成時間のインターバルカウンター
	int m_counter;
	// 移動ベクトル
	std::vector<Vector3> m_velocities;
};