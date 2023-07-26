#include "pch.h"
#include "ControllerManager.h"
#include <numeric>

// �R���g���[���[�̒ǉ�
void ControllerManager::Register(int num, std::unique_ptr<IController> controller)
{
	// �R���g���[���[��o�^����
	m_controllers[num] = std::move(controller);
}

// ���͗����̎擾
CommandData ControllerManager::CommandHistory(int num)
{
	return m_commands[num];
}

// �R���g���[���̍X�V
void ControllerManager::Update()
{
	for (int i = 0; i < m_controllers.size(); i++)
	{
		int command = m_controllers[i]->GetInput();
		m_commands[i].Input(command);
	}
}

// ���̓f�[�^�̎擾
int ControllerManager::GetInputData(int num)
{
	// �w�肵���R���g���[���[������̓f�[�^���󂯎��
	return m_commands[num][0];
}
int ControllerManager::GetInputData()
{
	// �S�ẴR���g���[���̓��͒l�����Z���ĕԂ�
	int totalInput = 0;
	for (int i = 0; i < m_controllers.size(); i++)
	{
		totalInput += GetInputData(i);
	}
	return totalInput;
}

// �u�ԓ��͂̎擾
int ControllerManager::GetMomentInputData(int num)
{
	// �w�肵���R���g���[���[���牟�����΂���̓��̓f�[�^���󂯎��
	return m_commands[num][0] - (m_commands[num][0] &m_commands[num][1]);
}
int ControllerManager::GetMomentInputData()
{
	// �S�ẴR���g���[���̏u�ԓ��͒l�����Z���ĕԂ�
	int totalInput = 0;
	for (int i = 0; i < m_controllers.size(); i++)
	{
		totalInput += GetMomentInputData(i);
	}
	return totalInput;
}
