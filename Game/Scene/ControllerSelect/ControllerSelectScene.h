#pragma once
#include <Game\Scene\Interface\IScene.h>
#include <Game\Controller\Interface\IController.h>
#include <Game\Effect\PrimitiveEffect\Unlimited\UnlimitedEffect.h>
#include <Game\Effect\PrimitiveEffect\SwichableAnimation\SwichableAnimation.h>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <CommonStates.h>
#include <Audio.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class ControllerSelectScene : public IScene
{
	public:
		ControllerSelectScene();
		~ControllerSelectScene() override;
		// ����������
		void initialize() override;
		// ��������
		void Create() override;
		// �X�V����
		void Update(DX::StepTimer const& timer) override;
		// �`�揈��
		void Render() override;
		// �I������
		void Finalize() override;
	private:
		// �L�[�{�[�h�̍X�V
		void KeyboardUpdate();
		// �Q�[���p�b�h�P�̍X�V
		void GamePad1Update();
		// �Q�[���p�b�h�Q�̍X�V
		void GamePad2Update();
	private:
		// �v���C���[�P�̃R���g���[���ԍ�
		int m_p1;
		// �v���C���[�Q�̃R���g���[���ԍ�
		int m_p2;
		// ���j���[�̉��n
		UnlimitedEffect* m_backGrand;
		// �v���C���[�P�A�C�R��
		UnlimitedEffect* m_p1Icon;
		// �v���C���[�Q�A�C�R��
		UnlimitedEffect* m_p2Icon;
		// �v���C���[�P���ǂ̃R���g���[���ɂȂ�������\���A�C�R��
		SwichableAnimation* m_p1ControllerIcon;
		// m_p1ControllerIcon�̍��W
		Vector3 m_p1CIconPos;
		// �v���C���[�Q���ǂ̃R���g���[���ɂȂ�������\���A�C�R��
		SwichableAnimation* m_p2ControllerIcon;
		// m_p2ControllerIcon�̍��W
		Vector3 m_p2CIconPos;
};

