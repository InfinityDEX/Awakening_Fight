#pragma once

#include <SimpleMath.h>
#include <vector>
using namespace DirectX;
using namespace DirectX::SimpleMath;

struct DamageData
{
	// 衝撃破タイプ
	enum Type
	{
		High,
		Low,
		OverHead,
		Throw
	};

	Type DamageType; // ダメージタイプ
	int EffectID; // エフェクトID
	int Damage; // ダメージ量
	int BlockDamage; // ガードしたときのダメージ量
	int StunDamage; // スタン値
	int HitStunTime_Difference; // ヒット時の硬直差
	int GuardStunTime_Difference; // ガード時の硬直差
	Vector2 Hit_Impact; // ヒット時の時の吹き飛ばしベクトル
	Vector2 Guard_Impact; // ガード時の吹き飛ばしベクトル
};

struct ImpactData
{
	Vector2 Position; // 中心座標
	float Height; // 縦幅
	float Width; // 横幅
	int StartUpTime; // 発生時間
	int ActiveTime; // 持続時間
	int RigidityTime; // 硬直時間
	DamageData Damage; // ダメージデータ
};

struct MoveMentData
{
	// 計算方法
	enum Type
	{
		Constant,
		Acceleration,
		Parabola
	};

	Type MoveType; // 計算方法
	Vector2 constant; // 定数
};

struct SkillData
{
	std::string SkillNama; // 技名
	int AnimationID; // 個別ID
	int Condition; // この技が出せるファイターの状態
	std::vector<int> LeverCommand; // レバーコマンドデータ	
	int LastPushButton; // ボタンデータ	
	int TimeLimit; // コマンドの制限時間
	MoveMentData Movement; // 移動データ
	ImpactData Impact; // 衝撃波データ
};

// スキルデータを優先度順にまとめたコンテナ
using SkillRepository = std::vector<SkillData>;
