#include "pch.h"
#include "FighterIdentity.h"
#include <Game\GameObject\Fighter\FighterFrame\Fighter.h>

const int FighterIdentity::Repeatedly_coefficient = 10;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                        
// public                                                                                                                 
//                                                                                                                        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FighterIdentity::Initialize(FighterShareData* plug)
{
	m_plug = plug;
}

void FighterIdentity::StateUpdata(const DX::StepTimer & timer)
{
	switch (m_plug->fighterState)
	{
	case STAND:
	case FRONTWALK:
		Stand(timer);
		break;
	case SQUAT:
		Squat(timer);
		break;
	case FLY:
		Fly(timer);
		break;
	default:
		break;
	}
}

const SkillData* FighterIdentity::Find_Matched_Skill()
{
	// �Ԃ�l
	const SkillData* tmp = nullptr;
	// �Z�𒲂ׂ�
	for (auto skill_Iter = Skills().begin(); skill_Iter != Skills().end() ; skill_Iter++)
	{
		// ���݂̏�Ԃ��Ƃ�
		auto nowState = m_plug->fighterState;
		//�����͗�����ԂƂ���
		if (nowState == FighterState::FRONTWALK || nowState == FighterState::BACKWALK)
			nowState = FighterState::STAND;
			 
		// ���̋Z�����݂̏�ԂŔ����ł��Ȃ����
		if(nowState != skill_Iter->Condition)
			continue; // ���̋Z�̔���͂��Ȃ�

		// �{�^����������������Ă��Ȃ����
		if (!PushJudgment(skill_Iter->LastPushButton))
			continue; // ���̋Z�̔���͂��Ȃ�

		// �R�}���h�̔���
		if (LeverCommandJudgment(*skill_Iter))
		{
			// ���v����
			tmp = &(*skill_Iter);
			break;
		}
	}
	return tmp;
}

// �{�^������
bool FighterIdentity::PushJudgment(int buttom)
{
	// �{�^��������o�^����Ă��Ȃ��f�[�^�͖���
	assert(buttom != 0 && "�{�^��������o�^����Ă��Ȃ��f�[�^���n����܂���");

	// �R���g���[���}�l�[�W��
	auto manager = Locator::GetControllerManager();
	// �R�}���h�f�[�^�̎Q��
	CommandData& command = manager->CommandHistory(m_plug->ControllerNum);

	for (int i = 0; i < CommandData::PushFream; i++)
	{
		if ((command[i] & buttom) != 0)
		{
			buttom -= command[i] & buttom;
			// �{�^�������ׂĉ�����Ă�����
			if (buttom == 0)
			{
				// �{�^�����͐���
				return true;
			}
		}
		// ��ԍŏ��ɕK�v�ȃ{�^���̓��͂�����Ȃ����
		else if(i == 0)
		{
			// �{�^�����͎��s
			return false;
		}
	}

	// ������Ă��Ȃ��{�^�������݂��Ă���
	return false;
}

// �A�Ŕ���(�{�^���P��)
bool FighterIdentity::RepeatedlyJudgment(KeyData buttom, int num)
{
	// �{�^��������o�^����Ă��Ȃ��f�[�^�͖���
	assert(buttom != 0 && "�{�^��������o�^����Ă��Ȃ��f�[�^���n����܂���");

	// �R���g���[���}�l�[�W��
	auto manager = Locator::GetControllerManager();
	// �R�}���h�f�[�^�̎Q��
	CommandData& command = manager->CommandHistory(m_plug->ControllerNum);

	// ���̃t���[�����ɕK�v�ȃL�[��������Ă邩�m�F����
	if ((command[0] & buttom) == 0)
	{
		// ���̃t���[�����Ƀ{�^���͉�����Ȃ�����
		return false;
	}

	// �Ō�ɓ��͂����m����Ă���k�����t���[����
	int frameCount = 0;
	// �A�ł�����������
	int success = 1;

	// ��O�̃R�}���h�f�[�^����m�F����
	for (int i = 1; i < command.CommandSize; i++)
	{
		// �����K�v�ȃ{�^���̓��͂���������
		if ((command[i] & buttom) != 0)
		{
			// �t���[���J�E���^�����Z�b�g
			frameCount = 0;
			// �����J�E���^��i�߂�
			success++;
			// �����A�ł��ꐔ���w�肵���l�ȏ�ł�������
			if (success >= num)
				return true; // �A�ŃR�}���h���� 

			// ���̂܂܎��̃��[�v�֐i��
			continue;
		}
		// �t���[���J�E���^��i�߂�
		frameCount++;
		
		// �����t���[���J�E���^�̒l���A�ł̗L���t���[�����𒴂��Ă�����
		if (frameCount > Repeatedly_coefficient)
		{
			// ���[�v���I������
			break;
		}
	}

	// �K�v�ȘA�Ő��ɒB���Ȃ�����
	return false;
}

// ���o�[�R�}���h����
bool FighterIdentity::LeverCommandJudgment(const SkillData& data)
{
	// �R���g���[���}�l�[�W��
	auto manager = Locator::GetControllerManager();
	// �R�}���h�f�[�^�̎Q��
	CommandData& inputData = manager->CommandHistory(m_plug->ControllerNum);
	// �ŏ��ɔ��肷����̓f�[�^��ێ�
	auto currentCommand = data.LeverCommand.rbegin();

	// �R�}���h���������Ă���
	for (int before_What_Frame = 0; before_What_Frame < CommandData::CommandSize; before_What_Frame++)
	{
		// �������Ԃ𒴂��Ă�����
		if (before_What_Frame > data.TimeLimit)
			// ���̋Z�̔��f���I����
			false;

		// ���������Ĕ��]����
		int input = *currentCommand;
		if (m_plug->Dir == -1)
		{
			input = Flip_horizontal(input);
		}

		// �ێ����Ă�����̓f�[�^���`�F�b�N����
		if (
			input & inputData[before_What_Frame] &&
			(
				before_What_Frame == 0 ||
				inputData[before_What_Frame] != inputData[before_What_Frame - 1]
				)
			)
		{
			// �C�e���[�^��i�߂�
			currentCommand++;
			//	���������ׂ��f�[�^���Ō�̃R�}���h��������
			if (currentCommand == data.LeverCommand.rend())
			{
				// ���v����
				return true;
			}
		}
	}

	// �R�}���h�s����
	return false;
}
bool FighterIdentity::LeverCommandJudgment(const std::vector<int>& command, const int & limit)
{
	// �R���g���[���}�l�[�W��
	auto manager = Locator::GetControllerManager();
	// �R�}���h�f�[�^�̎Q��
	CommandData& inputData = manager->CommandHistory(m_plug->ControllerNum);
	// �ŏ��ɔ��肷����̓f�[�^��ێ�
	auto currentCommand = command.rbegin();

	// ���͗��������Ă���
	for (int before_What_Frame = 0; before_What_Frame < CommandData::CommandSize; before_What_Frame++)
	{
		// �������Ԃ𒴂��Ă�����
		if (before_What_Frame > limit)
			// ���̋Z�̔��f���I����
			false;

		// ���������Ĕ��]����
		int input = *currentCommand;
		if (m_plug->Dir == -1)
		{
			input = Flip_horizontal(input);
		}

		// �ێ����Ă�����̓f�[�^���`�F�b�N����
		if (
			input & inputData[before_What_Frame] &&
			(
				before_What_Frame == 0 ||
				inputData[before_What_Frame] != inputData[before_What_Frame - 1]
				)
			)
		{
			// �C�e���[�^��i�߂�
			currentCommand++;
			//	���������ׂ��f�[�^���Ō�̃R�}���h��������
			if (currentCommand == command.rend())
			{
				// ���v����
				return true;
			}
		}
	}

	// �R�}���h�s����
	return false;
}
