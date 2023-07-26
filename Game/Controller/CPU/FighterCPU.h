#pragma once

#include <Game\Controller\Interface\IController.h>
#include <Game\GameObject\Fighter\FighterFrame\Fighter.h>

class FighterCPU : public IController
{
public:
	FighterCPU(FighterShareData& plug);
	~FighterCPU() override;
	virtual int GetInput() override;

private:

	/// 移動状態
	// 離れる
	int Withdrawal();
	// 近づく
	int Access();
	// 止まる
	int Stop();
	// 移動状態の変更処理
	void MoveStateChange();

	/// 攻撃状態
	// 攻撃しない
	int NotAttack();
	// 弱攻撃
	int Wake();
	// 中攻撃
	int Moderato();
	// 強攻撃
	int Hard();
	// 攻撃状態の変更処理
	void AttackStateChange();

	/// スタンス
	// 立つ
	int Stand();
	// しゃがむ
	int Sqaut();
	// 跳ぶ
	int Jump();
	// スタンスの変更処理
	void StanceStateChange();

	/// ガード状態
	// ガードする
	int Guard();
	// ガードしない
	int NonGuard();
	// ガード状態の変更処理
	void DefaultStateChange();

private:
	// 移動状態の遷移
	int(FighterCPU::*m_nowMoveState)();

	// 攻撃状態の遷移
	int(FighterCPU::*m_nowAttackState)();

	// スタンスの遷移
	int(FighterCPU::*m_nowStanceState)();

	// ガード状態の遷移
	int(FighterCPU::*m_nowDefenseState)();

	// 操作するファイターが持っている情報
	FighterShareData& m_plug;
	// 相手の前フレームの状態
	FighterState m_rivalBeforeState;
	// 思考カウンター(なにも操作しない時間の制限)
	int m_notOperationLimit;
	// 目標の達成状態
	bool m_quest;
	// 前のキーデータ
	int m_beforeKey;
};

