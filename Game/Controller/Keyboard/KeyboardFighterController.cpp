#include "pch.h"
#include "KeyboardFighterController.h"

// �L�[�{�[�h�̐���
Keyboard KeyboardFighterController::KEYBOARD;

// �R���X�g���N�^
KeyboardFighterController::KeyboardFighterController()
{}

// �f�X�g���N�^
KeyboardFighterController::~KeyboardFighterController()
{
}

// ���̓f�[�^�̎擾
int KeyboardFighterController::GetInput()
{
	auto kb = Keyboard::Get().GetState();
	m_tracker.Update(kb);

	// ������
	int command = 0;

	// �����L�[����
	if (kb.IsKeyDown(Keyboard::Keys::Right))
	{
		command |= KeyData::RIGHT;
	}
	else if (kb.IsKeyDown(Keyboard::Keys::Left))
	{
		command |= KeyData::LEFT;
	}
	if (kb.IsKeyDown(Keyboard::Keys::Up))
	{
		command |= KeyData::UP;
	}
	else if (kb.IsKeyDown(Keyboard::Keys::Down))
	{
		command |= KeyData::DOWN;
	}
	if (
		!kb.IsKeyDown(Keyboard::Keys::Right) &&
		!kb.IsKeyDown(Keyboard::Keys::Left) &&
		!kb.IsKeyDown(Keyboard::Keys::Up) &&
		!kb.IsKeyDown(Keyboard::Keys::Down) 
		)
	{
		command |= KeyData::NEUTRAL;
	}

	/// �V�X�e���p
	if (m_tracker.pressed.Back) // �L�����Z�����D��
	{
		// �L�����Z��
		command |= KeyData::CANCEL;
	}
	else if (m_tracker.pressed.Space)
	{
		// ����
		command |= KeyData::DICISION;
	}

	if (m_tracker.pressed.Escape)
	{
		// �v���O�����I��
		command |= KeyData::ESCAPE;
	}

	// �{�^������
	if (m_tracker.IsKeyPressed(Keyboard::Keys::A))
	{
		command |= KeyData::WEAK;
	}
	if (m_tracker.IsKeyPressed(Keyboard::Keys::S))
	{
		command |= KeyData::MODERATO;
	}
	if (m_tracker.IsKeyPressed(Keyboard::Keys::D))
	{
		command |= KeyData::HARD;
	}

	return command;
}
