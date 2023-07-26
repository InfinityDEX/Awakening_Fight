#pragma once

#include <Game\Controller\Interface\IController.h>
#include <GamePad.h>

using namespace DirectX;

// �Q�[���p�b�h�̃��b�p�[
class GamePadFighterController final: public IController
{
	public:
		// �R���X�g���N�^
		GamePadFighterController(int num = 0);
		// �f�X�g���N�^
		~GamePadFighterController() override;
		// ���̓f�[�^�̎擾
		virtual int GetInput() override;
	private:
		// �Q�[���p�b�h
		static GamePad GAMEPAD;
		// �Q�[���p�b�h�̃g���b�J�[
		GamePad::ButtonStateTracker m_tracker;
		// �Q�[���p�b�h�̌ʔԍ�
		int m_padNum;
};

