#include "pch.h"
#include <Game\Scene\Duel\DuelScene.h>
#include <Game\3rdParty\Debug\Font\DebugFont.h>
#include <Game\3rdParty\Debug\GridFloor\GridFloor.h>
#include <Game\Locator\Locator.h>
#include <Game\GameObject\Fighter\Strategy\Brave\Brave.h>
#include <Game\Scene\Result\ResultScene.h>
#include <Game\Effect\EffectObject\Stage\Stage1_Subway\Subway.h>
#include "WICTextureLoader.h"

const float DuelScene::READY_END_MOVE_NODE	= 2.0f;
const float DuelScene::START_END_MOVE_NODE	= 0.8f;
const float DuelScene::JUDGE_END_MOVE_NODE	= 1.4f;
const float DuelScene::READY_STOP_INTERVAL	= 0.5f;
const float DuelScene::START_STOP_INTERVAL	= 0.3f;
const float DuelScene::JUDGE_STOP_INTERVAL	= 1.0f;
const float DuelScene::RESULT_INTERVAL		= 2.5f;
const float DuelScene::CAMERA_STOP_INTERVAL = 1.0f;
const float DuelScene::CANGE_CAMERA_NUM     = 3;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                        
// public                                                                                                                 
//                                                                                                                        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �R���X�g���N�^
DuelScene::DuelScene(int p1, int p2)
	: IScene()
	, P1CNumber(p1)
	, P2CNumber(p2)
{
	m_fastRender = 1;
}

// �f�X�g���N�^
DuelScene::~DuelScene()
{
}

// ����������
void DuelScene::initialize()
{
	m_intervalTime = 0;
	m_limitTime = 60;
	m_cameraNum = 0;
	// �ŏ��̏�Ԃ����߂�
	m_nowState = &DuelScene::StartState;
	// �J������X���W�̏����l��ݒ�
	m_nowCameraXposition = (P1->GetTransform()->position.x + P2->GetTransform()->position.x) / 2;
	// �J�����̋߂��̏����l��ݒ�
	m_lastDepth = abs(P1->GetTransform()->position.x - P2->GetTransform()->position.x);
}

// ��������
void DuelScene::Create()
{
	// �G�t�F�N�g�}�l�[�W���[�擾
	auto manager = Locator::GetEffectManager();
	// �X�e�[�W�̍쐬
	m_stage = std::make_unique<Subway>();
	// ���v�𐶐�
	m_segment = std::make_unique<SegmentCounter>();
	// �v���C���[�P��HP�o�[�쐬
	m_p1HP = new RectangleGage(
		Vector3(-0.55f, 0.9f, 0),
		Vector3(0.4f, 0.2f, 0.2f),
		L"Resources\\Sprite\\UI\\HPGage\\Body.png",
		L"Resources\\Sprite\\UI\\HPGage\\gage.png",
		L"Resources\\Sprite\\UI\\HPGage\\GageMask.png"
	);
	// �v���C���[�Q��HP�o�[�쐬
	m_p2HP = new RectangleGage(
		Vector3(0.55f, 0.9f, 0),
		Vector3(-0.4f, 0.2f, 0.2f),
		L"Resources\\Sprite\\UI\\HPGage\\Body.png",
		L"Resources\\Sprite\\UI\\HPGage\\gage.png",
		L"Resources\\Sprite\\UI\\HPGage\\GageMask.png"
	);
	// �v���C���[�Q��HP�o�[�쐬
	manager->Register(DrawingOrder::Front, m_p1HP);
	manager->Register(DrawingOrder::Front, m_p2HP);
	// �v���C���[��ݒ肷��
	P1 = new FighterFrame(new Brave(), &P2, P1CNumber, 1, Vector3(-3, 0, 0));
	P2 = new FighterFrame(new Brave(), &P1, P2CNumber, 2, Vector3(3, 0, 0));
	// �I�u�W�F�N�g����
	P1->Create();
	P2->Create();
	// ������
	P1->Initialize();
	P2->Initialize();
	// �J�n���ɏo��uReady?�v�̃G�t�F�N�g���쐬
	EffectInterface* effect = new FadeEffect(Vector3::Zero, Vector3(1, 0.5f, 1), 5, 5, 30, L"Resources\\Sprite\\EffectParts\\Announce\\AYR.png");
	manager->Register(DrawingOrder::Front, effect);
}

// �X�V����
void DuelScene::Update(const DX::StepTimer& timer)
{
	// �^�C�}�[
	timer;
	// ��ԑJ��
	(this->*m_nowState)(timer);
	// �X�e�[�W�̍X�V
	StageUpdate(timer);
	// ����L�����N�^�[�̍X�V
	FightersUpdate(timer);
	// �}�l�[�W���̍X�V
	ManagersUpdate(timer);
	// ���݂̎��Ԃ����v�ɑ���
	CountDownClock(timer);
}

// �`�揈��
void DuelScene::Render()
{
	RECT size = Locator::GetDeviceResources()->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);
	float forAngleY = XMConvertToRadians(45.0f);
	float cameraXpos = (P1->GetTransform()->position.x + P2->GetTransform()->position.x) / 2;
	m_nowCameraXposition += (cameraXpos - m_nowCameraXposition) * 0.1f;
	float depth = 6 * (abs(P1->GetTransform()->position.x - P2->GetTransform()->position.x) / FighterFrame::PLAYER_INTERVAL_LIMIT);
	m_nowCameraXposition = std::fmaxf(std::fminf(m_nowCameraXposition, FighterFrame::PLAYER_INTERVAL_LIMIT / 2), -FighterFrame::PLAYER_INTERVAL_LIMIT / 2);
	depth = std::fmaxf(std::fminf(depth, 6.6), 5.5f);
	m_lastDepth = m_lastDepth + 0.01f * (depth - m_lastDepth);
	float height = 2;
	Matrix view = Matrix::CreateLookAt(Vector3(m_nowCameraXposition, height, m_lastDepth), Vector3(m_nowCameraXposition, height, 0), Vector3::UnitY);
	Matrix proj = Matrix::CreatePerspectiveFieldOfView(
		forAngleY,
		aspectRatio,
		0.01f,
		1000.0f
	);
	// �G�t�F�N�g�}�l�[�W���[�̍X�V
	Locator::GetEffectManager()->Render(DrawingOrder::Back, Matrix::Identity, view, proj);
	// �t�@�C�^�[�̕`��
	switch (m_fastRender)
	{
	case 1:
		P1->Render(view, proj);
		P2->Render(view, proj);
		break;
	case 2:
		P2->Render(view, proj);
		P1->Render(view, proj);
		break;
	default:
		break;
	}
	// �G�t�F�N�g�}�l�[�W���[�̍X�V
	Locator::GetEffectManager()->Render(DrawingOrder::Front, Matrix::Identity, view, proj);
}

// �I������
void DuelScene::Finalize()
{}

// ��ԍŏ��ɌĂяo��
void DuelScene::ReadyState(const DX::StepTimer& timer)
{
	if (m_intervalTime >= READY_END_MOVE_NODE)
	{
		m_intervalTime = 0;
		// �V�X�e���G�t�F�N�g���쐬
		EffectInterface* effect = new FadeEffect(Vector3::Zero, Vector3(1, 1, 1), 5, 10, 60, L"Resources\\Sprite\\EffectParts\\Announce\\Fight.png");
		Locator::GetEffectManager()->Register(DrawingOrder::Front, effect);
		m_nowState = &DuelScene::StartState;
		return;
	}
	m_intervalTime += timer.GetElapsedSeconds();
	FighterFrame::ControlLock();
}

// ���E���h�H�R�[��
void DuelScene::StartState(const DX::StepTimer& timer)
{
	if (m_intervalTime >= START_END_MOVE_NODE)
	{
		m_intervalTime = 0;
		m_nowState = &DuelScene::FightingState;
		Locator::GetSoundManager()->Volume(5, 10);
		Locator::GetSoundManager()->OneShot(5);
		return;
	}
	m_intervalTime += timer.GetElapsedSeconds();
	FighterFrame::ControlLock();
}

// �t�@�C�^���삪�ł�����
void DuelScene::FightingState(const DX::StepTimer& timer)
{		
	// �i�Q�[�̓Q�[���X�s�[�h�̉e���Ŏ��Ԃ�����������̂Ŏ��Ԍo�߂��������ɂ���
	m_limitTime -= timer.GetElapsedSeconds() * 0.7f;
	m_limitTime = std::fmax(m_limitTime, 0);
	if (m_limitTime == 0)
	{
		// �^�C���A�b�v���o�̌Ăяo��
		m_nowState = &DuelScene::TimeUpState;
		// �^�C���A�b�v�̃G�t�F�N�g���쐬
		EffectInterface* effect = new FadeEffect(Vector3::Zero, Vector3(1, 0.5f, 1), 10, 10, 60, L"Resources\\Sprite\\EffectParts\\Announce\\TimeUp.png");
		Locator::GetEffectManager()->Register(DrawingOrder::Front, effect);
	}
	if (P1->KO() || P2->KO())
	{
		// �������o�̌Ăяo��
		m_nowState = &DuelScene::DramaticState;
		// KO�̃G�t�F�N�g���쐬
		EffectInterface* effect = new FadeEffect(Vector3::Zero, Vector3(1, 0.5f, 1), 10, 10, 60, L"Resources\\Sprite\\EffectParts\\Announce\\KO.png");
		Locator::GetEffectManager()->Register(DrawingOrder::Front, effect);
	}
	FighterFrame::ControlOpen();
}

// ���Ԑ؂�ɂȂ�����Ăяo��
void DuelScene::TimeUpState(const DX::StepTimer& timer)
{
	m_intervalTime += timer.GetElapsedSeconds();
	if (m_intervalTime >= JUDGE_END_MOVE_NODE)
	{
		m_nowState = &DuelScene::BeforeResultState;
	}
	FighterFrame::ControlLock();
}

// �ǂ�������KO�������ɌĂяo��
void DuelScene::DramaticState(DX::StepTimer const & timer)
{
	FighterFrame::ControlLock();
	m_dramaticTrigger = true;
	m_intervalTime += timer.GetElapsedSeconds();
	if (m_intervalTime >= CAMERA_STOP_INTERVAL)
	{
		m_cameraNum++;
		m_intervalTime = 0;
	}
	if (m_cameraNum >= CANGE_CAMERA_NUM)
	{
		m_cameraNum = 0;
		m_intervalTime = 0;
		m_nowState = &DuelScene::KOState;
	}
}

// ���̉��o���I�������Ăяo��
void DuelScene::KOState(const DX::StepTimer& timer)
{
	FighterFrame::ControlLock();
	m_intervalTime += timer.GetElapsedSeconds();
	if (m_intervalTime >= JUDGE_END_MOVE_NODE)
	{
		m_intervalTime = 0;
		m_nowState = &DuelScene::BeforeResultState;
	}
}

// ���U���g�O�ɌĂяo��
void DuelScene::BeforeResultState(DX::StepTimer const & timer)
{
	FightersUpdate(timer);
	m_intervalTime += timer.GetElapsedSeconds();
	
	if (m_intervalTime >= RESULT_INTERVAL)
	{
		Locator::GetCollisionManager()->Finalize();

		ResultScene::Judge jugge;
		if (P1->KO() || P1->HPReference() < P2->HPReference())
		{
			jugge = ResultScene::Judge::P2;
		}
		if (P2->KO() || P1->HPReference() > P2->HPReference())
		{
			jugge = ResultScene::Judge::P1;
		}
		if ((P1->KO() && P2->KO()) || P1->HPReference() == P2->HPReference())
		{
			jugge = ResultScene::Judge::Draw;
		}
		Locator::GetSceneManager()->RequestScene_Add(std::make_unique<ResultScene>(jugge));
	}
	FighterFrame::ControlLock();
}

// �X�e�[�W�̍X�V
void DuelScene::StageUpdate(DX::StepTimer const & timer)
{
	// �X�e�[�W�̍X�V
	m_stage->Update(timer);
}

void DuelScene::FightersUpdate(DX::StepTimer const & timer)
{
	// HP�o�[���Ǘ�����
	m_p1HP->Amount(static_cast<float>(P1->HPReference()) / static_cast<float>(P1->MAXHP));
	m_p2HP->Amount(static_cast<float>(P2->HPReference()) / static_cast<float>(P2->MAXHP));


	if (m_nowState != &DuelScene::DramaticState)
	{
		// �t�@�C�^�[�̍X�V
		P1->Update(timer);
		P2->Update(timer);
	}
}

void DuelScene::ManagersUpdate(DX::StepTimer const & timer)
{
	// �R���C�_�[�}�l�[�W���[�̍X�V
	Locator::GetCollisionManager()->Update(timer);
}

void DuelScene::CountDownClock(DX::StepTimer const & timer)
{
	// ���Ԃ����S�Ɂu0�v�ɂȂ����^�C�~���O�ŃJ�E���^�[���u00�v�ɂ������̂ŏ����׍H����
	if (m_limitTime == int(m_limitTime))
	{
		// �҂����肾�����炻�̂܂�
		m_segment->SetTime(m_limitTime);
	}
	else
	{
		// �����_�P�ʂŗ]�肪����΂P���₷
		m_segment->SetTime(m_limitTime + 1);
	}
}
