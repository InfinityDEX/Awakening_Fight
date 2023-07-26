#include "pch.h"
#include "Subway.h"
#include <Game\Locator\Locator.h>

// �R���X�g���N�^
Subway::Subway()
{
	// �G�t�F�N�g�}�l�[�W���[�擾
	auto effectManager = Locator::GetEffectManager();

	// ��{�̃X�P�[��
	Vector3 scale = Vector3(7, 4, 0);
	// ��{�̍��W
	Vector3 pos = Vector3(0, 3, 0);
	m_fTrainStartPos = Vector3(-130, 5, -1);
	m_bTrainStartPos = Vector3(100, 5, -3);
	m_bTrainSpeed = -0.5f;
	m_fTrainSpeed = 0.5f;

	// �v���b�g�t�H�[���쐬
	m_platform = new UnlimitedEffect(scale, pos - Vector3::UnitZ * 0, L"Resources\\Sprite\\Stages\\Subway\\Platform.png");
	// �d�؂�Ǎ쐬
	m_partition = new UnlimitedEffect(scale, pos - Vector3::UnitZ * 2, L"Resources\\Sprite\\Stages\\Subway\\Partition.png");
	// ���������̃v���b�g�t�H�[���쐬
	m_oppositePlatform = new UnlimitedEffect(scale * 1.1f, pos - Vector3::UnitZ * 4, L"Resources\\Sprite\\Stages\\Subway\\OppositePlatform.png");
	// �d�ԍ쐬
	m_frontTrain = new UnlimitedEffect(scale * 2, m_fTrainStartPos, L"Resources\\Sprite\\Stages\\Subway\\Train.png");
	m_backTrain = new UnlimitedEffect(scale * 2, m_bTrainStartPos, L"Resources\\Sprite\\Stages\\Subway\\Train.png");

	// �}�l�[�W���ɓo�^
	effectManager->Register(DrawingOrder::Back, m_oppositePlatform);
	effectManager->Register(DrawingOrder::Back, m_backTrain);
	effectManager->Register(DrawingOrder::Back, m_partition);
	effectManager->Register(DrawingOrder::Back, m_frontTrain);
	effectManager->Register(DrawingOrder::Back, m_platform);

	// �X�e�[�W�e�[�}�̍Đ�
	Locator::GetSoundManager()->Loop(300);
	Locator::GetSoundManager()->Volume(300, 0.5f);
	Locator::GetSoundManager()->Loop(301);
	Locator::GetSoundManager()->Volume(301, 1);
}

// �f�X�g���N�^
Subway::~Subway()
{
	// �X�e�[�W�������~�߂�
	Locator::GetSoundManager()->LoopStop(300);
	Locator::GetSoundManager()->LoopStop(301);
}

// �X�V����
void Subway::Update(const DX::StepTimer & timer)
{
	// ���������̓d�Ԃ��X�^�[�g�n�_�̑΋ɂ̈ʒu�ɒB������
	if (
		((m_bTrainSpeed / abs(m_bTrainSpeed)) == 1 && m_backTrain->Position().x >= -m_bTrainStartPos.x) ||
		((m_bTrainSpeed / abs(m_bTrainSpeed)) == -1 && m_backTrain->Position().x <= -m_bTrainStartPos.x)
		)
	{
		// �X�^�[�g�n�_�Ƀ��[�v
		m_backTrain->Teleport(m_bTrainStartPos);
	}
	// ��O�̓d�Ԃ��X�^�[�g�n�_�̑΋ɂ̈ʒu�ɒB�����炽��
	if (
		((m_fTrainSpeed / abs(m_fTrainSpeed)) == 1 && m_frontTrain->Position().x >= -m_fTrainStartPos.x) ||
		((m_fTrainSpeed / abs(m_fTrainSpeed)) == -1 && m_frontTrain->Position().x <= -m_fTrainStartPos.x)
		)
	{
		// �X�^�[�g�n�_�Ƀ��[�v
		m_frontTrain->Teleport(m_fTrainStartPos);
	}

	// �d�Ԃ̈ړ�
	m_backTrain->Movement(Vector3(m_bTrainSpeed, 0, 0));
	m_frontTrain->Movement(Vector3(m_fTrainSpeed, 0, 0));
}
