#pragma once
#include <Game\Scene\Interface\IScene.h>
#include <Game\GameObject\Fighter\FighterFrame\Fighter.h>
#include <GeometricPrimitive.h>
#include <Game\Effect\EffectObject\Stage\Interface\IStage.h>
#include <Game\Effect\PrimitiveEffect\Fade\FadeEffect.h>
#include <Game\Effect\PrimitiveEffect\Gage\RectangleGage.h>
#include <Game\Effect\UI\Counter\SegmentCounter.h>
#include <Audio.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <memory>
using namespace DirectX;
using namespace DirectX::SimpleMath;
class DuelScene : public IScene
{
	public:
		DuelScene(int p1, int p2);
		~DuelScene() override;
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
		/// �X�e�[�g
		// ����
		void ReadyState(const DX::StepTimer& timer);
		// �X�^�[�g
		void StartState(const DX::StepTimer& timer);
		// �퓬
		void FightingState(const DX::StepTimer& timer);
		// ���Ԑ؂�
		void TimeUpState(const DX::StepTimer& timer);
		// �h���}�`�b�N���o
		void DramaticState(const DX::StepTimer& timer);
		// KO
		void KOState(const DX::StepTimer& timer);
		// ���U���g�O
		void BeforeResultState(const DX::StepTimer& timer);
		/// �X�V
		// �X�e�[�W�̃A�b�v�f�[�g
		void StageUpdate(const DX::StepTimer& timer);
		// �t�@�C�^�[�̃A�b�v�f�[�g
		void FightersUpdate(const DX::StepTimer& timer);
		// �}�l�[�W���̃A�b�v�f�[�g
		void ManagersUpdate(const DX::StepTimer& timer);
		// �J�E���g�_�E���N���b�N�̃A�b�v�f�[�g
		void CountDownClock(const DX::StepTimer& timer);
	private:
		static const float START_END_MOVE_NODE;
		static const float READY_END_MOVE_NODE;
		static const float JUDGE_END_MOVE_NODE;
		static const float RESULT_INTERVAL;
		static const float START_STOP_INTERVAL;
		static const float READY_STOP_INTERVAL;
		static const float JUDGE_STOP_INTERVAL;
		static const float CAMERA_STOP_INTERVAL;
		static const float CANGE_CAMERA_NUM;
		float m_intervalTime;
		// �X�e�[�g�Ǘ��p�̃|�C���^
		void(DuelScene::*m_nowState)(const DX::StepTimer&);
		// �v���C���[1(����)
		FighterFrame* P1;
		// �v���C���[2(�E��)
		FighterFrame* P2;
		// �v���C���[1�̃R���g���[���ԍ�
		int P1CNumber;
		// �v���C���[2�̃R���g���[���ԍ�
		int P2CNumber;
		// �R�����X�e�[�g
		CommonStates* m_commonState;
		// �v���C���[1�̗̑�
		RectangleGage* m_p1HP;
		// �v���C���[2�̗̑�
		RectangleGage* m_p2HP;
		// �c�莞��
		float m_limitTime;
		// �J������X���W
		float m_nowCameraXposition;
		// �t�@�C�^�[�̕`�揇���Ǘ�����ϐ�
		int m_fastRender;
		//�X�e�[�W
		std::unique_ptr<IStage> m_stage;
		// �c�莞�ԃ^�C�}�[
		std::unique_ptr<SegmentCounter> m_segment;
		// ��ɕ`�悷��v���C���[
		float m_lastDepth = 0;
		// FPS�l
		float m_fps;
		// �G�t�F�N�g
		std::unique_ptr<BasicEffect> m_effect;
		// �h���}�`�b�N�g���K�[
		bool m_dramaticTrigger;
		// �J�����ԍ�
		int m_cameraNum;
};

