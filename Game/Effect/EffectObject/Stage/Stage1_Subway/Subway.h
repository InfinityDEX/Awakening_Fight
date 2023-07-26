#pragma once
#include <Game\Effect\EffectObject\Stage\Interface\IStage.h>
#include <Game\Effect\PrimitiveEffect\Unlimited\UnlimitedEffect.h>

class Subway final : public IStage
{
	public:
		// �R���X�g���N�^
		Subway();
		// �f�X�g���N�^
		~Subway();
		// ����������
		void Initialize() override {};
		// �X�V����
		void Update(const DX::StepTimer& timer) override;
	private:
		// �v���b�g�t�H�[��(���)
		UnlimitedEffect* m_platform;
		// ���H�̎d�؂�
		UnlimitedEffect* m_partition;
		// ���������̃v���b�g�t�H�[��
		UnlimitedEffect* m_oppositePlatform;
		// ��O�̓d��
		UnlimitedEffect* m_backTrain;
		// ��O�̓d�Ԃ̃X�^�[�g���W
		Vector3 m_bTrainStartPos;
		// ��O�̓d�Ԃ̃X�s�[�h
		float m_bTrainSpeed;
		// �������̓d��
		UnlimitedEffect* m_frontTrain;
		// �������̓d�Ԃ̃X�^�[�g���W
		Vector3 m_fTrainStartPos;
		// ��O�̓d�Ԃ̃X�s�[�h
		float m_fTrainSpeed;
};