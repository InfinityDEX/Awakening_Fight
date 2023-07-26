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

	/// �ړ����
	// �����
	int Withdrawal();
	// �߂Â�
	int Access();
	// �~�܂�
	int Stop();
	// �ړ���Ԃ̕ύX����
	void MoveStateChange();

	/// �U�����
	// �U�����Ȃ�
	int NotAttack();
	// ��U��
	int Wake();
	// ���U��
	int Moderato();
	// ���U��
	int Hard();
	// �U����Ԃ̕ύX����
	void AttackStateChange();

	/// �X�^���X
	// ����
	int Stand();
	// ���Ⴊ��
	int Sqaut();
	// ����
	int Jump();
	// �X�^���X�̕ύX����
	void StanceStateChange();

	/// �K�[�h���
	// �K�[�h����
	int Guard();
	// �K�[�h���Ȃ�
	int NonGuard();
	// �K�[�h��Ԃ̕ύX����
	void DefaultStateChange();

private:
	// �ړ���Ԃ̑J��
	int(FighterCPU::*m_nowMoveState)();

	// �U����Ԃ̑J��
	int(FighterCPU::*m_nowAttackState)();

	// �X�^���X�̑J��
	int(FighterCPU::*m_nowStanceState)();

	// �K�[�h��Ԃ̑J��
	int(FighterCPU::*m_nowDefenseState)();

	// ���삷��t�@�C�^�[�������Ă�����
	FighterShareData& m_plug;
	// ����̑O�t���[���̏��
	FighterState m_rivalBeforeState;
	// �v�l�J�E���^�[(�Ȃɂ����삵�Ȃ����Ԃ̐���)
	int m_notOperationLimit;
	// �ڕW�̒B�����
	bool m_quest;
	// �O�̃L�[�f�[�^
	int m_beforeKey;
};

