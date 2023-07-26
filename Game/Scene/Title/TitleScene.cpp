#include "pch.h"
#include <Game\Locator\Locator.h>
#include <Game\Scene\Title\TitleScene.h>
#include <Game\Scene\ControllerSelect\ControllerSelectScene.h>
#include <Game\Effect\PrimitiveEffect\Unlimited\UnlimitedEffect.h>

// �R���X�g���N�^
TitleScene::TitleScene()
{
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
	// ���C���e�[�}���~�߂�
	Locator::GetSoundManager()->LoopStop(200);
}

// ����������
void TitleScene::initialize()
{
	// ���C���e�[�}�𗬂�
	Locator::GetSoundManager()->Loop(200);
}

// ��������
void TitleScene::Create()
{
	auto effectManager = Locator::GetEffectManager();
	m_backColor = Vector4(1.0f, 0.0f, 0.1f, 0);
	m_backGrand = new TitleBackGrandEffect();
	effectManager->Register(DrawingOrder::Front, m_backGrand);
	effectManager->Register(DrawingOrder::Front, new UnlimitedEffect(Vector3(0.8f), Vector3::Zero, L"Resources\\Sprite\\Logo\\TitleLogo.png"));
	Vector3 nodeScale = Vector3::One;
}

// �X�V����
void TitleScene::Update(DX::StepTimer const& timer)
{
	// �R���g���[���[�̂ǂꂩ���������������
	if (Locator::GetControllerManager()->GetInputData() & KeyData::ANY)
	{
		// ���艹�𗬂�
		Locator::GetSoundManager()->OneShot(0);
		// �R���g���[���I���V�[���Ăяo��
		Locator::GetSceneManager()->RequestScene_Add(std::make_unique<ControllerSelectScene>());
	}
}

// �o�b�N�X�V����
void TitleScene::BackUpdate(DX::StepTimer const & timer)
{
	auto effectManager = Locator::GetEffectManager();
	effectManager->Update(timer);
}

// �`�揈��
void TitleScene::Render()
{
	auto objectManager = Locator::GetCollisionManager();
	auto effectManager = Locator::GetEffectManager();
	effectManager->Render(DrawingOrder::Front, Matrix::Identity, Matrix::Identity, Matrix::Identity);
}

// �I������
void TitleScene::Finalize()
{
	// �G�t�F�N�g�����Z�b�g����
	Locator::GetEffectManager()->Reset();
}
