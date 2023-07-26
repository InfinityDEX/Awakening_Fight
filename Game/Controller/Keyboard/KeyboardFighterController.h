#pragma once

#include <Game\Controller\Interface\IController.h>
#include <Keyboard.h>

using namespace DirectX;

// �L�[�{�[�h�̃��b�p�[
class KeyboardFighterController final: public IController
{
	public:
		// �R���X�g���N�^
		KeyboardFighterController();
		// �f�X�g���N�^
		~KeyboardFighterController();
		// ���̓f�[�^�̎擾
		virtual int GetInput() override;
	private:
		// �L�[�{�[�h
		static Keyboard KEYBOARD;
		// �L�[�{�[�h�̃g���b�J�[
		Keyboard::KeyboardStateTracker m_tracker;
};

