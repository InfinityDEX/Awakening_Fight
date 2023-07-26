#include "pch.h"
#include <Game\Controller\CPU\FighterCPU.h>


FighterCPU::FighterCPU(FighterShareData& plug)
	: m_plug(plug)
	, m_beforeKey(0)
{
	// �ړ�
	m_nowMoveState = &FighterCPU::Stop;
	// �U��
	m_nowAttackState = &FighterCPU::NotAttack;
	// �X�^���X
	m_nowStanceState = &FighterCPU::Stand;
	// �K�[�h�̗L��
	m_nowDefenseState = &FighterCPU::NonGuard;
	// �v�l�J�E���^�����Z�b�g
	m_notOperationLimit = 0;
	// �ړI�B�����
	m_quest = false;
}


FighterCPU::~FighterCPU()
{}

int FighterCPU::GetInput()
{
	// �L�[�̃f�[�^
	int key = m_beforeKey;
	return key;
	// �d�����Ă��炠����x���Ԃ��o���A�ꕔ�̏�Ԃ��ړI��B�����Ă����Ȃ�
	if (m_notOperationLimit < 0 || m_quest)
	{
		// ���Z�b�g
		key = 0;
		// �ړ���Ԃ̕ύX
		MoveStateChange();
		// �U����Ԃ̕ύX
		AttackStateChange();
		// �X�^���X�̕ύX
		StanceStateChange();
		// �K�[�h��Ԃ̕ύX
		DefaultStateChange();
		

		//if (abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x) >= 2)
		//{
		//	key = DirectionToKeyData(m_plug.Dir);
		//}

		// �ړI�𖢒B����Ԃɖ߂�
		m_quest = false;

		// 0.2�b����0.5�b���炢�ŃZ�b�g�@
		m_notOperationLimit = rand() % 18 + 12;
	}

	// �S�Ă̏�Ԃ��|�����킹��
	key = 
		(this->*m_nowMoveState)() |
		(this->*m_nowAttackState)() | 
		(this->*m_nowStanceState)() |
		(this->*m_nowDefenseState)();

	//// ��Ԃ��L��
	//m_rivalBeforeState = (*m_plug.Rival)->GetFighterState();
	// �v�l�J�E���^�[�����炷
	m_notOperationLimit--;

	// �O�̃L�[�ɓo�^
	m_beforeKey = key;

	return key;
}

int FighterCPU::Withdrawal()
{
	// ����Ƃ̋����𒲂ׂ�
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);

	// ����Ƃ̋����������̉�����2.0�{��艓�ނ�����
	if (length > m_plug.Size->x * 2.0f)
	{
		// �ړI�B��
		m_quest = true;
	}
	// �������ɃL�[����
	return DirectionToKeyData(-m_plug.Dir);
}

int FighterCPU::Access()
{
	// ����Ƃ̋����𒲂ׂ�
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);

	// ����Ƃ̋����������̉�����1.5�{���߂Â��Ă�����
	if (length < m_plug.Size->x * 1.5f)
	{
		// �ړI�B��
		m_quest = true;
	}

	// �O�����ɃL�[����
	return DirectionToKeyData(m_plug.Dir);
}

int FighterCPU::Stop()
{
	// �������͂��Ȃ�
	return 0;
}

int FighterCPU::NotAttack()
{
	// �������͂��Ȃ�
	return 0;
}

int FighterCPU::Wake()
{
	// ����Ƃ̋����𒲂ׂ�
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);
	// ����Ƃ̋����������̉����Ɠ������炢�����߂�
	if (length <= m_plug.Size->x)
	{
		// ��U��
		return KeyData::WEAK;

		// �ړI�B��
		m_quest = true;
	}
}

int FighterCPU::Moderato()
{
	// ����Ƃ̋����𒲂ׂ�
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);
	// ����Ƃ̋����������̉����Ɠ������炢�����߂�
	if (length <= m_plug.Size->x)
	{
		// ���U��
		return KeyData::MODERATO;

		// �ړI�B��
		m_quest = true;
	}
}

int FighterCPU::Hard()
{
	// ����Ƃ̋����𒲂ׂ�
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);
	// ����Ƃ̋����������̉����Ɠ������炢�����߂�
	if (length <= m_plug.Size->x)
	{
		// ���U��
		return KeyData::HARD;

		// �ړI�B��
		m_quest = true;
	}
}

int FighterCPU::Stand()
{
	// �������Ȃ�
	return 0;
}

int FighterCPU::Sqaut()
{
	// ������
	return KeyData::DOWN;
}

int FighterCPU::Jump()
{
	// �W�����v�L�[���������u�Ԃɒ��ԂƂ����ړI�͒B������Ă���
	m_quest = true;
	// �����
	return KeyData::UP;
}

int FighterCPU::Guard()
{
	//�@�K�[�h
	return DirectionToKeyData(-m_plug.Dir);
}

int FighterCPU::NonGuard()
{
	// ���͂��Ȃ�
	return 0;
}

void FighterCPU::MoveStateChange()
{
	// ����Ƃ̋����𒲂ׂ�
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);
	
	// ����Ƃ̋����������̉�����1.5�{�ȏ㗣��Ă�����
	if (length > m_plug.Size->x * 1.5f)
	{
		// �O�i
		m_nowMoveState = &FighterCPU::Access;
	}
	// ����Ƃ̋����������̉�����1.5�{���߂����
	else if(length < m_plug.Size->x * 1.5f)
	{
		// ���
		m_nowMoveState = &FighterCPU::Withdrawal;
	}
	// ����Ƃ̋����������̉�����1.5�{���߂����
	else
	{
		// �~�܂�
		m_nowMoveState = &FighterCPU::Stop;
	}
}

void FighterCPU::AttackStateChange()
{
	// ����Ƃ̋����𒲂ׂ�
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);

	// �󒆂ɂ�����
	if (m_plug.transform->position.y > 0)
	{
		// ����Ƃ̋����������̉�����1.5�{���߂����
		if (length < m_plug.Size->x * 2.0f)
		{
			// ���U��
			m_nowAttackState = &FighterCPU::Hard;
		}
		// ����Ƃ̋����������̉�����1.5�{���߂����
		else if(length < m_plug.Size->x * 1.5f)
		{
			// ���U��
			m_nowAttackState = &FighterCPU::Moderato;
		}
		// ����Ƃ̋����������̉������߂����
		if (length < m_plug.Size->x * 1.0f)
		{
			// ��U��
			m_nowAttackState = &FighterCPU::Wake;
		}
	}
}

void FighterCPU::StanceStateChange()
{
	// ����Ƃ̋����𒲂ׂ�
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);


	// ����̋����������̉�����2.0�{�ȏ�߂����
	if (length < m_plug.Size->x * 2.0f)
	{
		// ������x�߂���ΎO��Ɉ����x�Œ���
		if (rand() % 3)
		{
			// ����
			m_nowStanceState = &FighterCPU::Jump;
		}
		// ���Ⴊ��
		m_nowStanceState = &FighterCPU::Sqaut;
	}
	else
	{
		// ����
		m_nowStanceState = &FighterCPU::Stand;
	}
}

void FighterCPU::DefaultStateChange()
{
	// ����Ƃ̋����𒲂ׂ�
	int length = abs(m_plug.transform->position.x - (*m_plug.Rival)->GetTransform()->position.x);

	// �Z����������x�̋����ōd������������
	if (length < m_plug.Size->x * 1.5f && (*m_plug.Rival)->GetRigor() > 0)
	{
		m_nowDefenseState = &FighterCPU::Guard;
		return;
	}
	m_nowDefenseState = &FighterCPU::NonGuard;
}
