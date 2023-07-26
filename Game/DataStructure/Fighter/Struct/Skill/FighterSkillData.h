#pragma once

#include <SimpleMath.h>
#include <vector>
using namespace DirectX;
using namespace DirectX::SimpleMath;

struct DamageData
{
	// �Ռ��j�^�C�v
	enum Type
	{
		High,
		Low,
		OverHead,
		Throw
	};

	Type DamageType; // �_���[�W�^�C�v
	int EffectID; // �G�t�F�N�gID
	int Damage; // �_���[�W��
	int BlockDamage; // �K�[�h�����Ƃ��̃_���[�W��
	int StunDamage; // �X�^���l
	int HitStunTime_Difference; // �q�b�g���̍d����
	int GuardStunTime_Difference; // �K�[�h���̍d����
	Vector2 Hit_Impact; // �q�b�g���̎��̐�����΂��x�N�g��
	Vector2 Guard_Impact; // �K�[�h���̐�����΂��x�N�g��
};

struct ImpactData
{
	Vector2 Position; // ���S���W
	float Height; // �c��
	float Width; // ����
	int StartUpTime; // ��������
	int ActiveTime; // ��������
	int RigidityTime; // �d������
	DamageData Damage; // �_���[�W�f�[�^
};

struct MoveMentData
{
	// �v�Z���@
	enum Type
	{
		Constant,
		Acceleration,
		Parabola
	};

	Type MoveType; // �v�Z���@
	Vector2 constant; // �萔
};

struct SkillData
{
	std::string SkillNama; // �Z��
	int AnimationID; // ��ID
	int Condition; // ���̋Z���o����t�@�C�^�[�̏��
	std::vector<int> LeverCommand; // ���o�[�R�}���h�f�[�^	
	int LastPushButton; // �{�^���f�[�^	
	int TimeLimit; // �R�}���h�̐�������
	MoveMentData Movement; // �ړ��f�[�^
	ImpactData Impact; // �Ռ��g�f�[�^
};

// �X�L���f�[�^��D��x���ɂ܂Ƃ߂��R���e�i
using SkillRepository = std::vector<SkillData>;
