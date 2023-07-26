#include "pch.h"
#include <Game\Controller\GamePad\GamePadFighterController.h>
#include <Game\Controller\Keyboard\KeyboardFighterController.h>
#include <Game\Scene\Duel\DuelScene.h>
#include "ControllerSelectScene.h"
#include <WICTextureLoader.h>

// �R���X�g���N�^
ControllerSelectScene::ControllerSelectScene()
{
	// -1��CPU
	m_p1 = m_p2 = -1;
}

// �f�X�g���N�^
ControllerSelectScene::~ControllerSelectScene()
{
	
}

// ����������
void ControllerSelectScene::initialize()
{
	m_p1CIconPos = Vector3(-0.5f, -0.4f, 0);
	m_p2CIconPos = Vector3( 0.5f, -0.4f, 0);
}

// ��������
void ControllerSelectScene::Create()
{
	auto deviceResouces = Locator::GetDeviceResources();
	auto device = deviceResouces->GetD3DDevice();
	auto context = deviceResouces->GetD3DDeviceContext();
	auto effectManager = Locator::GetEffectManager();
	m_backGrand = new UnlimitedEffect(Vector3::One, Vector3::Zero, L"Resources\\Sprite\\Black.png");
	m_p1Icon = new UnlimitedEffect(Vector3(0.5f), Vector3(-0.5f, 0.4f, 0), L"Resources\\Sprite\\Logo\\Player1.png");
	m_p2Icon = new UnlimitedEffect(Vector3(0.5f), Vector3(0.5f, 0.4f, 0), L"Resources\\Sprite\\Logo\\Player2.png");
	m_p1ControllerIcon = new SwichableAnimation(m_p1CIconPos, Vector3(0.5f), "Resources\\Data\\UI\\ControllerSelect\\Controller.json");
	m_p2ControllerIcon = new SwichableAnimation(m_p2CIconPos, Vector3(0.5f),"Resources\\Data\\UI\\ControllerSelect\\Controller.json");
	effectManager->Register(DrawingOrder::Front, m_backGrand);
	effectManager->Register(DrawingOrder::Front, m_p1Icon);
	effectManager->Register(DrawingOrder::Front, m_p2Icon);
	effectManager->Register(DrawingOrder::Front, m_p1ControllerIcon);
	effectManager->Register(DrawingOrder::Front, m_p2ControllerIcon);
	m_p1ControllerIcon->SwitchAnimetion(1,-1);
	m_p2ControllerIcon->SwitchAnimetion(1,-1);
}

// �X�V����
void ControllerSelectScene::Update(DX::StepTimer const & timer)
{
	// �L�[�{�[�h�̍X�V
	KeyboardUpdate();
	// �Q�[���p�b�h�P�̍X�V
	GamePad1Update();
	// �Q�[���p�b�h�Q�̍X�V
	GamePad2Update();

	m_p1ControllerIcon->SwitchAnimetion(1, m_p1);
	m_p2ControllerIcon->SwitchAnimetion(1, m_p2);
}

// �L�[�{�[�h�̍X�V
void ControllerSelectScene::KeyboardUpdate()
{
	// �L�[�{�[�h�̎��ʔԍ�
	int id = 2;
	// �L�[�{�[�h�̎擾
	CommandData Command = Locator::GetControllerManager()->CommandHistory(id);

	// ���{�^���������Ă�����
	if (Command[0] & KeyData::LEFT)
	{
		// �J�[�\���ړ���
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == -1)
			m_p1 = id;
		if (m_p2 == id)
			m_p2 = -1;
	}

	// �E�{�^���������Ă�����
	if (Command[0] & KeyData::RIGHT)
	{
		// �J�[�\���ړ���
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == id)
			m_p1 = -1;
		if (m_p2 == -1)
			m_p2 = id;
	}

	// �L�����Z���{�^���������Ă�����
	if (Command[0] & KeyData::CANCEL)
	{
		// �L�����Z����
		Locator::GetSoundManager()->OneShot(1);
		if (m_p1 == id)
			m_p1 = -1;
		else if (m_p2 == id)
			m_p2 = -1;
		else
			Locator::GetSceneManager()->PopScene(1);
	}

	// ����{�^���������Ă�����
	if (Command[0] & KeyData::DICISION)
	{
		// �ǂ��炩�̃R���g���[���ɓo�^����Ă�����
		if (m_p1 == id || m_p2 == id)
		{
			// ���艹���Đ�
			Locator::GetSoundManager()->OneShot(0);
			// �V�[�����Ăяo��
			Locator::GetSceneManager()->RequestScene_Reset(std::make_unique<DuelScene>(m_p1, m_p2));
		}
		// ������Ă��Ȃ�������
		else
		{
			// �J�[�\���ړ���
			Locator::GetSoundManager()->OneShot(2);
			// �E���D��
			if (m_p1 == -1)
				m_p1 = id;
			else if (m_p2 == -1)
				m_p2 = id;
		}
	}
}

// �Q�[���p�b�h�P�̍X�V
void ControllerSelectScene::GamePad1Update()
{
	// �Q�[���p�b�h�P�̎��ʔԍ�
	int id = 0;
	// �Q�[���p�b�h�P�̎擾
	CommandData Command = Locator::GetControllerManager()->CommandHistory(id);

	// ���{�^���������Ă�����
	if (Command[0] & KeyData::LEFT)
	{
		// �J�[�\���ړ���
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == -1)
			m_p1 = id;
		if (m_p2 == id)
			m_p2 = -1;
	}

	// �E�{�^���������Ă�����
	if (Command[0] & KeyData::RIGHT)
	{
		// �J�[�\���ړ���
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == id)
			m_p1 = -1;
		if (m_p2 == -1)
			m_p2 = id;
	}

	// �L�����Z���{�^���������Ă�����
	if (Command[0] & KeyData::CANCEL)
	{
		// �L�����Z����
		Locator::GetSoundManager()->OneShot(1);
		if (m_p1 == id)
			m_p1 = -1;
		else if (m_p2 == id)
			m_p2 = -1;
		else
			Locator::GetSceneManager()->PopScene(1);
	}

	// ����{�^���������Ă�����
	if (Command[0] & KeyData::DICISION)
	{
		// �ǂ��炩�̃R���g���[���ɓo�^����Ă�����
		if (m_p1 == id || m_p2 == id)
		{
			// ���艹���Đ�
			Locator::GetSoundManager()->OneShot(0);
			// �V�[�����Ăяo��
			Locator::GetSceneManager()->RequestScene_Reset(std::make_unique<DuelScene>(m_p1, m_p2));
		}
		// ������Ă��Ȃ�������
		else
		{
			// �J�[�\���ړ���
			Locator::GetSoundManager()->OneShot(2);
			// �E���D��
			if (m_p1 == -1)
				m_p1 = id;
			else if (m_p2 == -1)
				m_p2 = id;
		}
	}
}

// �Q�[���p�b�h�Q�̍X�V
void ControllerSelectScene::GamePad2Update()
{
	// �Q�[���p�b�h�Q�̎��ʔԍ�
	int id = 1;
	// �Q�[���p�b�h�Q�̎擾
	CommandData Command = Locator::GetControllerManager()->CommandHistory(id);

	// ���{�^���������Ă�����
	if (Command[0] & KeyData::LEFT)
	{
		// �J�[�\���ړ���
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == -1)
			m_p1 = id;
		if (m_p2 == id)
			m_p2 = -1;
	}

	// �E�{�^���������Ă�����
	if (Command[0] & KeyData::RIGHT)
	{
		// �J�[�\���ړ���
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == id)
			m_p1 = -1;
		if (m_p2 = -1)
			m_p2 = id;
	}

	// �L�����Z���{�^���������Ă�����
	if (Command[0] & KeyData::CANCEL)
	{
		// �L�����Z����
		Locator::GetSoundManager()->OneShot(1);
		if (m_p1 == id)
			m_p1 = -1;
		else if (m_p2 == id)
			m_p2 = -1;
		else
			Locator::GetSceneManager()->PopScene(1);
	}

	// ����{�^���������Ă�����
	if (Command[0] & KeyData::DICISION)
	{
		// �ǂ��炩�̃R���g���[���ɓo�^����Ă�����
		if (m_p1 == id || m_p2 == id)
		{
			// ���艹���Đ�
			Locator::GetSoundManager()->OneShot(0);
			// �V�[�����Ăяo��
			Locator::GetSceneManager()->RequestScene_Reset(std::make_unique<DuelScene>(m_p1, m_p2));
		}
		// ������Ă��Ȃ�������
		else
		{
			// �E���D��
			if (m_p1 == -1)
				m_p1 = id;
			else if (m_p2 == -1)
				m_p2 = id;
		}
	}
}

// �`�揈��
void ControllerSelectScene::Render()
{
	Matrix ide = Matrix::Identity;
	Locator::GetEffectManager()->Render(DrawingOrder::Back, ide, ide, ide);
	Locator::GetEffectManager()->Render(DrawingOrder::Front, ide, ide, ide);
}

// �I������
void ControllerSelectScene::Finalize()
{
	m_backGrand->Kill();
	m_p1Icon->Kill();
	m_p2Icon->Kill();
	m_p1ControllerIcon->Kill();
	m_p2ControllerIcon->Kill();
}