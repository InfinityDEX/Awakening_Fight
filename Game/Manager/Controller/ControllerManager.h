#pragma once
#include <Game\Controller\CommandData\CommandData.h>
#include <Game\Controller\Interface\IController.h>
#include <deque>
#include <map>

// �R���g���[���}�l�[�W��
class ControllerManager final
{
	public:
		// �R���g���[���[�̒ǉ�
		void Register(int num, std::unique_ptr<IController> controller);
		// ���͗����̎擾
		CommandData CommandHistory(int num);
		// �R���g���[���̃A�b�v�f�[�g
		void Update();
		// ���̓f�[�^�̎擾
		int GetInputData(int num);
		int GetInputData();
		// �u�ԓ��͂̎擾
		int GetMomentInputData(int num);
		int GetMomentInputData();
	private:
		// ���̓f�[�^�̗���
		std::map<int, CommandData> m_commands;
		// �R���g���[���[�̃R���e�i
		std::map<int,std::unique_ptr<IController>> m_controllers;
};