#pragma once
#include <list>
#include <Game\Controller\CommandData\CommandData.h>

// �R���g���[���[�̃C���^�[�t�F�[�X
class IController
{
	public:
		// �R���X�g���N�^
		IController() = default;
		// �f�X�g���N�^
		virtual ~IController() {};
		// ���̓f�[�^�̎擾
		virtual int GetInput() = 0;
};