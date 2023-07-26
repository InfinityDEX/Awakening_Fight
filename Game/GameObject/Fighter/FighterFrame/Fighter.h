#pragma once
#ifndef FIGHTER
#define FIGHTER

#include <GeometricPrimitive.h>
#include <SimpleMath.h>
#include <Audio.h>
#include <list>
#include <Game\GameObject\Physics\PrimitivePhysics\Box\Box.h>
#include <Game\GameObject\Fighter\Strategy\Interface\FighterIdentity.h>
#include <Game\GameObject\Physics\Collider\AttackedCollider\AttackedCollider.h>
#include <Game\Controller\Interface\IController.h>
#include <Game\Controller\CommandData\CommandData.h>
#include <Game\Effect\PrimitiveEffect\SwichableAnimation\SwichableAnimation.h>
#include <Game\Effect\PrimitiveEffect\Animation\AnimationEffect.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;
static float GRAVITY = 0.05f;

// �t�@�C�^�[�̏��
enum FighterState
{
	STAND, // ����
	SQUAT, // ���Ⴊ��
	FLY, // ��
	FRONTWALK, // �O����
	BACKWALK, // �����
	ATTACK, // �U������
	ATTACKED, // �U�����ꂽ
	KO, // KO����
};
// �t�@�C�^�[�̌��̃N���X�Ƌ��L����t�@�C�^�[�̏��
struct FighterShareData
{
	// �t�@�C�^�[�̏��
	FighterState fighterState = FighterState::STAND;
	// �O��̃t�@�C�^�[�̏��
	FighterState beforeState = fighterState;
	// ����
	FighterFrame* Me; 
	// �ΐ푊��
	FighterFrame** Rival; 
	// �R���g���[���[�ԍ�
	int ControllerNum;
	// �t�@�C�^�[�̃A�j���[�V����
	SwichableAnimation* Animation;
	// �A�j���[�V�������W
	Vector3 AnimationCenter; 
	// ���W�n
	Transform* transform;
	// �傫��
	Vector3* Size; 
	// �d������
	int Rigidity;
	// �ړ��X�s�[�h
	float Speed;
 	// �W�����v��
   	int JumpCount;
	// �̗�
	int HP;
	// �L�����N�^�[�̌���
	int Dir;
};

// �\�����x��
enum BeastLevel
{
	Peaceful, // ����
	Rage, // ���{
	Ferocious, // �֖�
	Frenzy, // ����
};

// �t�@�C�^�[�̍��i
class FighterFrame : public Box
{	
	public:
		// �R���X�g���N�^
		FighterFrame(FighterIdentity* identity, FighterFrame** rival, int controllerNum, int playerNumber, Vector3 pos = Vector3::Zero);
		// �f�X�g���N�^
		~FighterFrame();
		// �����֐�
		virtual void Create();
		// �������֐�
		virtual void Initialize();
		// �X�V�֐�
		virtual void Update(DX::StepTimer const& timer) override;
		// �`��֐�
		virtual void Render(const Matrix view, const Matrix proj) override;
		// �q�b�g�{�b�N�X�̕`��֐�
		virtual void IsHitBoxRender(const Matrix view, const Matrix proj);
		// ���Z�b�g�֐�
		virtual void Reset();
		// ���X�^�[�g
		void Restart();
		// �U������
		bool Attacked(DamageData damageData,Vector3 collisionCenter);
		//// �r�[�X�g���[�h�I��
		//void BeastMode();
		// �\�����x���̎擾
		const BeastLevel Emotions() { return m_beastLevel; };

		//////////////////////////////////
		//
		// �X�V����
		//
		//////////////////////////////////
		
		// �U���������
		void TurningAround();
		// ��������
		void PhysicalProcess();
		// ��ԑJ��
		void StateTransition(DX::StepTimer const& timer);
		// �A�j���[�V�����J��
		void AnimationTransition();
		// �d������
		bool RigorDecision();
		// ���ʑ���
		void CommonOperation(DX::StepTimer const& timer);
		// ��������
		void Throw();
		// �L�����ʂ̍X�V����
		void UpdateIdentity(DX::StepTimer const& timer);
		// �Z����
		void TechniqueDecision();
		// �Z�X�V
		void TechniqueUpdate();
		// ��ԍX�V
		void StancesChange(DX::StepTimer const& timer);
		// �ʒu����
		void PositionAdjustment();

		//////////////////////////////////

		// �U����
		bool Attacking() { return m_skillData != nullptr; };
		// HP�̎Q��
		const int& HPReference() { return m_plug.HP; };
		//FighterState GetFighterState() { return m_plug.fighterState;};
		// KO�̎Q��
		bool KO() { return m_plug.HP <= 0; };
		// ����s�\
		static void ControlLock() { m_controlLock = true; };
		// ����\
		static void ControlOpen() { m_controlLock = false; };
		// �d�����Ԃ�Ԃ�
		int GetRigor() { return m_rigor; };
		// �U���������
		static const int TURNING_FRAME;
		// �W�����v�̑΋󎞊�
		static const int JUMP_INTERVAL;
		// �W�����v�̍���
		static const int JUMP_HEIGHT;
		// ���͂��󂯕t���Ȃ�
		static bool m_controlLock;
		static const int MAXHP;
		static const int MAXSP;
		// ���̕�
		static const float LEFT_LIMIT;
		// �E�̕�
		static const float RIGHT_LIMIT;
		// �v���C���[���m��������鋗��
		static const float PLAYER_INTERVAL_LIMIT;
	private:
		// �\�����[�h������
		void PeacefulHeart();
		void Move();
		void Jump();
		void Damage(int damage);
		bool JudgeGuard(DamageData damageData, Vector3 collisionCenter);
		void StandPhysics();
		void SquatPhysics();
		void FlyPhysics();
		void AttackPhysics();

		// �t�@�C�^�[�̌�
		std::unique_ptr<FighterIdentity> m_identity;

		// �v���C���[�ԍ�
		const int m_playerNumber;
		int m_rigor;
		std::unique_ptr<GeometricPrimitive>	m_box;
		Matrix m_rotate;
		// �������̋Z�f�[�^
		const SkillData* m_skillData;
		int m_turning;
		int m_modelDir;
		int m_jumpClock;
		// �\�����x��
		BeastLevel m_beastLevel;
		// �\���G�t�F�N�g
		AnimationEffect* m_beastEffect;
		// �L�����ʏ����N���X�ɓn���v���O
		FighterShareData m_plug;
		std::unique_ptr<EffectFactory> m_factory;
		std::unique_ptr<CommonStates> m_state;
};
#endif // FIGHTER