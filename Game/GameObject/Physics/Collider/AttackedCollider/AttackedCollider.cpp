#include "pch.h"
#include "AttackedCollider.h"
//#include <Game\Effect\SlashEffect.h>
//#include <Game\Effect\ShockWaveEffect.h>
#include <Game\GameObject\Fighter\FighterFrame\Fighter.h>

AttackedCollider::AttackedCollider(FighterFrame* target, Vector3* chase, ImpactData impactData)
	: m_target(target)
	, m_chaseTarget(chase)
	, m_interval(impactData.Position)
	, m_life(impactData.ActiveTime)
{
	// 向きを決める
	float dir = target->GetTransform()->position.x - chase->x;
	if (dir != 0)
	{
		dir = dir / abs(dir);
	}
	else
		dir = 1;
	m_interval.x *= dir;

	GetTransform()->position = *m_chaseTarget + m_interval;
	m_size = Vector3(impactData.Width, impactData.Height, 1);
	m_damageData = impactData.Damage;
}

AttackedCollider::~AttackedCollider()
{
}

void AttackedCollider::Create()
{
}

void AttackedCollider::Initialize()
{
}

void AttackedCollider::Update(DX::StepTimer const & timer)
{
	if (isHitting(m_target))
	{
		// オブジェクト削除の申請
		KillSwitch();

		// 攻撃が通ったかを判定
		m_target->Attacked(m_damageData, GetTransform()->position);
	}

	if (m_life <= 0)
	{
		// オブジェクト削除の申請
		KillSwitch();
		return;
	}

	//// キャラクターを追尾しつつ、一定の位置に固定
	GetTransform()->position = *m_chaseTarget + m_interval;
	m_life--;
}

void AttackedCollider::Render(const Matrix view, const Matrix proj)
{
}

void AttackedCollider::Reset()
{
}
