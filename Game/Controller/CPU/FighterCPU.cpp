#include "pch.h"
#include <Game\Controller\CPU\FighterCPU.h>


FighterCPU::FighterCPU(FighterShareData& plug)
	: m_plug(plug)
	, m_beforeKey(0)
{
	// 移動
	m_nowMoveState = &FighterCPU::Stop;
	// 攻撃
	m_nowAttackState = &FighterCPU::NotAttack;
	// スタンス
	m_nowStanceState = &FighterCPU::Stand;
	// ガードの有無
	m_nowDefenseState = &FighterCPU::NonGuard;
	// 思考カウンタをリセット
	m_notOperationLimit = 0;
	// 目的達成状態
	m_quest = false;
}


FighterCPU::~FighterCPU()
{}

int FighterCPU::GetInput()
{
	// キーのデータ
	int key = m_beforeKey;
	return key;
	// 硬直してからある程度時間が経つか、一部の状態が目的を達成していたなら
	if (m_notOperationLimit < 0 || m_quest)
	{
		// リセット
		key = 0;
		// 移動状態の変更
		MoveStateChange();
		// 攻撃状態の変更
		AttackStateChange();
		// スタンスの変更
		StanceStateChange();
		// ガード状態の変更
		DefaultStateChange();
		

		//if (abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x) >= 2)
		//{
		//	key = DirectionToKeyData(m_plug.Dir);
		//}

		// 目的を未達成状態に戻す
		m_quest = false;

		// 0.2秒から0.5秒ぐらいでセット　
		m_notOperationLimit = rand() % 18 + 12;
	}

	// 全ての状態を掛け合わせる
	key = 
		(this->*m_nowMoveState)() |
		(this->*m_nowAttackState)() | 
		(this->*m_nowStanceState)() |
		(this->*m_nowDefenseState)();

	//// 状態を記憶
	//m_rivalBeforeState = (*m_plug.Rival)->GetFighterState();
	// 思考カウンターを減らす
	m_notOperationLimit--;

	// 前のキーに登録
	m_beforeKey = key;

	return key;
}

int FighterCPU::Withdrawal()
{
	// 相手との距離を調べる
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);

	// 相手との距離が自分の横幅の2.0倍より遠退いたら
	if (length > m_plug.Size->x * 2.0f)
	{
		// 目的達成
		m_quest = true;
	}
	// 後ろ向きにキー入力
	return DirectionToKeyData(-m_plug.Dir);
}

int FighterCPU::Access()
{
	// 相手との距離を調べる
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);

	// 相手との距離が自分の横幅の1.5倍より近づいていたら
	if (length < m_plug.Size->x * 1.5f)
	{
		// 目的達成
		m_quest = true;
	}

	// 前向きにキー入力
	return DirectionToKeyData(m_plug.Dir);
}

int FighterCPU::Stop()
{
	// 何も入力しない
	return 0;
}

int FighterCPU::NotAttack()
{
	// 何も入力しない
	return 0;
}

int FighterCPU::Wake()
{
	// 相手との距離を調べる
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);
	// 相手との距離が自分の横幅と同じぐらいかより近い
	if (length <= m_plug.Size->x)
	{
		// 弱攻撃
		return KeyData::WEAK;

		// 目的達成
		m_quest = true;
	}
}

int FighterCPU::Moderato()
{
	// 相手との距離を調べる
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);
	// 相手との距離が自分の横幅と同じぐらいかより近い
	if (length <= m_plug.Size->x)
	{
		// 中攻撃
		return KeyData::MODERATO;

		// 目的達成
		m_quest = true;
	}
}

int FighterCPU::Hard()
{
	// 相手との距離を調べる
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);
	// 相手との距離が自分の横幅と同じぐらいかより近い
	if (length <= m_plug.Size->x)
	{
		// 強攻撃
		return KeyData::HARD;

		// 目的達成
		m_quest = true;
	}
}

int FighterCPU::Stand()
{
	// 何もしない
	return 0;
}

int FighterCPU::Sqaut()
{
	// 下入力
	return KeyData::DOWN;
}

int FighterCPU::Jump()
{
	// ジャンプキーを押した瞬間に跳ぶという目的は達成されている
	m_quest = true;
	// 上入力
	return KeyData::UP;
}

int FighterCPU::Guard()
{
	//　ガード
	return DirectionToKeyData(-m_plug.Dir);
}

int FighterCPU::NonGuard()
{
	// 入力しない
	return 0;
}

void FighterCPU::MoveStateChange()
{
	// 相手との距離を調べる
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);
	
	// 相手との距離が自分の横幅の1.5倍以上離れていたら
	if (length > m_plug.Size->x * 1.5f)
	{
		// 前進
		m_nowMoveState = &FighterCPU::Access;
	}
	// 相手との距離が自分の横幅の1.5倍より近ければ
	else if(length < m_plug.Size->x * 1.5f)
	{
		// 後退
		m_nowMoveState = &FighterCPU::Withdrawal;
	}
	// 相手との距離が自分の横幅の1.5倍より近ければ
	else
	{
		// 止まる
		m_nowMoveState = &FighterCPU::Stop;
	}
}

void FighterCPU::AttackStateChange()
{
	// 相手との距離を調べる
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);

	// 空中にいたら
	if (m_plug.transform->position.y > 0)
	{
		// 相手との距離が自分の横幅の1.5倍より近ければ
		if (length < m_plug.Size->x * 2.0f)
		{
			// 強攻撃
			m_nowAttackState = &FighterCPU::Hard;
		}
		// 相手との距離が自分の横幅の1.5倍より近ければ
		else if(length < m_plug.Size->x * 1.5f)
		{
			// 中攻撃
			m_nowAttackState = &FighterCPU::Moderato;
		}
		// 相手との距離が自分の横幅より近ければ
		if (length < m_plug.Size->x * 1.0f)
		{
			// 弱攻撃
			m_nowAttackState = &FighterCPU::Wake;
		}
	}
}

void FighterCPU::StanceStateChange()
{
	// 相手との距離を調べる
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);


	// 相手の距離が自分の横幅の2.0倍以上近ければ
	if (length < m_plug.Size->x * 2.0f)
	{
		// ある程度近ければ三回に一回程度で跳ぶ
		if (rand() % 3)
		{
			// 跳ぶ
			m_nowStanceState = &FighterCPU::Jump;
		}
		// しゃがみ
		m_nowStanceState = &FighterCPU::Sqaut;
	}
	else
	{
		// 立つ
		m_nowStanceState = &FighterCPU::Stand;
	}
}

void FighterCPU::DefaultStateChange()
{
	// 相手との距離を調べる
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);

	// 技が当たる程度の距離で硬直中だったら
	if (length < m_plug.Size->x * 1.5f && (*m_plug.Rival)->GetRigor() > 0)
	{
		m_nowDefenseState = &FighterCPU::Guard;
		return;
	}
	m_nowDefenseState = &FighterCPU::NonGuard;
}
