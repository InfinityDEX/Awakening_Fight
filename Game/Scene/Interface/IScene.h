#pragma once
#ifndef ISCENE
#define  ISCENE

#include <Game\3rdParty\Timer\StepTimer.h>
#include <Keyboard.h>
#include <GamePad.h>

class IScene
{
public:
	// �R���X�g���N�^
	IScene() = default;
	// �f�X�g���N�^
	virtual ~IScene() = default;

	// ����������
	virtual void initialize() = 0;

	// ��������
	virtual void Create() = 0;

	// �X�V����
	virtual void Update(DX::StepTimer const& timer) = 0;

	// ���X�V����
	virtual void BackUpdate(DX::StepTimer const& timer) {};

	// �`�揈��
	virtual void Render() = 0;
	
	// �I������
	virtual void Finalize() = 0;


protected:
	// �f�o�b�N�\���g���K�[
	static bool m_debugTrigger;
};
#endif

