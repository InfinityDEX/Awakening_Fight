#include "pch.h"
#include "Fighter.h"
#include <WICTextureLoader.h>


#include <Game\GameObject\Physics\Collider\AttackedCollider\AttackedCollider.h>
#include <Game\Controller\CPU\FighterCPU.h>
//#include <Game\Effect\ShockWaveEffect.h>
//#include <Game\Effect\SlashEffect.h>
//#include <Game\Effect\FlashEffect.h>
#include <Game\Locator\Locator.h>
#include <algorithm>
#include <iterator>

bool FighterFrame::m_controlLock = false;

const int FighterFrame::MAXHP = 100;
const int FighterFrame::MAXSP = 100;
const int FighterFrame::JUMP_INTERVAL= 40;
const int FighterFrame::JUMP_HEIGHT = 2;
const int FighterFrame::TURNING_FRAME = 12;
const float FighterFrame::LEFT_LIMIT = -5;
const float FighterFrame::RIGHT_LIMIT = 5;
const float FighterFrame::PLAYER_INTERVAL_LIMIT = 5;

// �R���X�g���N�^
FighterFrame::FighterFrame(FighterIdentity* identity, FighterFrame** rival, int controllerNum, int playerNumber, Vector3 pos)
	: m_playerNumber(playerNumber)
{
	std::unique_ptr<FighterIdentity> tmp(identity);
	m_identity = std::move(tmp);

	auto deviceResouces = Locator::GetDeviceResources();
	m_box = GeometricPrimitive::CreateBox(deviceResouces->GetD3DDeviceContext(), Vector3::One);

	m_factory = std::make_unique<EffectFactory>(deviceResouces->GetD3DDevice());
	m_state = std::make_unique<CommonStates>(deviceResouces->GetD3DDevice());

	GetTransform()->position = pos;


	m_plug.Me = this;
	m_plug.Rival = rival;
	m_plug.HP = MAXHP;
	m_plug.ControllerNum = controllerNum;
	m_plug.Size = &m_size;
	m_plug.Speed = 0.1f;
	m_plug.JumpCount = 0;
	m_turning = 0;
	m_skillData = nullptr;
}

// �f�X�g���N�^
FighterFrame::~FighterFrame()
{
}

// ��������
void FighterFrame::Create()
{
	switch (m_playerNumber)
	{
	case 1:
		//m_playerNumberIcon = new ChaseEffect(GetTransform()->position, L"Resources\\Sprite\\1P.png");
		break;
	case 2:
		//m_playerNumberIcon = new ChaseEffect(GetTransform()->position, L"Resources\\Sprite\\2P.png");
		break;
	default:
		break;
	}
	//Locator::GetEffectManager()->Register(DrawingOrder::Front, m_playerNumberIcon);

	//// �A�j���[�V������������
	//m_beastEffect = new AnimationEffect(10, Vector2(5, 2), Vector3::One, GetTransform()->position, L"Resources\\Sprite\\EffectParts\\FightingEffect\\Aura.png", true);
	//Locator::GetEffectManager()->Register(DrawingOrder::Back, m_beastEffect);
	//m_beastEffect->DrawMode(false);
}

// ����������
void FighterFrame::Initialize()
{
	// ���W�f�[�^�̌��т�
	m_plug.transform = GetTransform();
	// �v���O�̏�����
	m_identity->Initialize(&m_plug);

	// �����A�j���[�V������ݒ肷��
	m_plug.Animation->SwitchAnimetion(60, 0);
	
	// ���������߂�
	float vec = (*m_plug.Rival)->GetTransform()->position.x - GetTransform()->position.x;
	if (vec != 0)
	{
		m_turning = 0;
		vec = vec / abs(vec);
		m_plug.Dir = vec;
	}

	// �������W�𒲐�
	GetTransform()->position.y = m_plug.Size->y / 2;
	//m_playerNumberIcon->SetPosition(GetTransform()->position);
	m_plug.AnimationCenter = GetTransform()->position;

	//if (!m_controller)
	//{
	//	m_controller = std::make_unique<FighterCPU>(m_plug);
	//}
	wchar_t* path = L"Resources\\Sprite\\1P.png";
	m_plug.Rigidity = 0;
	m_rigor = 0;

	// �\�����[�h������
	PeacefulHeart();
}

// �X�V����
void FighterFrame::Update(DX::StepTimer const& timer)
{
	// �U���������
	TurningAround();
	// �Z�X�V
	TechniqueUpdate();
	// �L�������R���g���[���ł��邩�H
	if (!m_controlLock)
	{
		// �L�����ʂ̍X�V����
		UpdateIdentity(timer);
		// �d����������
		if (RigorDecision())
		{
			// ��������
			Throw();
			// ���ʑ���
			CommonOperation(timer);
			// �Z����
			TechniqueDecision();
		}

	}
	// ��������
	PhysicalProcess();
	// �A�j���[�V�����J��
	AnimationTransition();
	// ��ԏ���
	StateTransition(timer);
	// ��ԕύX
	StancesChange(timer);
	// �ʒu����
	PositionAdjustment();
}

// �`��֐�
void FighterFrame::Render(const Matrix view, const Matrix proj)
{
	Matrix position;
	Matrix rotation;

	auto deviceResouces = Locator::GetDeviceResources();
	position = Matrix::CreateTranslation(GetTransform()->position);
	rotation = Matrix::CreateRotationY(XMConvertToRadians(90) * m_plug.Dir);
}

// �q�b�g�{�b�N�X�̕`��֐�
void FighterFrame::IsHitBoxRender(const Matrix view, const Matrix proj)
{
	Matrix world;
	world = Matrix::CreateScale(*m_plug.Size);
	world *= Matrix::CreateTranslation(m_plug.transform->position);
	m_box->Draw(world, view, proj, Colors::White, nullptr, true);
}

// ���Z�b�g�֐�
void FighterFrame::Reset()
{

}

// �U������
bool FighterFrame::Attacked(DamageData damageData, Vector3 collisionCenter)
{
	// ��ԕ����̌v�Z
	float vec = 0;
	vec = GetTransform()->position.x - collisionCenter.x;
	int dir;
	if (vec < 0)
	{
		dir = -1;
	}
	if (0 <= vec)
	{
		dir = 1;
	}

	bool judge = JudgeGuard(damageData, collisionCenter);
	
	Vector3 effectPos;
	Vector2 attackToMe = GetTransform()->position - collisionCenter;
	float rotato = atan2(attackToMe.x, -attackToMe.y);
	Matrix effectRotato;

	// �K�[�h�Ɏ��s���邩�d�����Ȃ�
	if(!judge || m_rigor > 0)
	{
		m_rigor = (*m_plug.Rival)->GetRigor() + damageData.HitStunTime_Difference;
		effectPos = Vector3(GetTransform()->position.x, collisionCenter.y, 0);
		rotato = atan2(attackToMe.x, -attackToMe.y);
		effectRotato = Matrix::CreateRotationZ(rotato);
		// �p�[�e�B�N���G�t�F�N�g
		//Spark* spark = new Spark(Vector3(GetTransform()->position.x, collisionCenter.y, 0), Vector3(attackToMe.x, 0, 0), XMConvertToRadians(120), 5, 3.0f, 2.0f);
		//Locator::GetEffectManager()->Register(DrawingOrder::Front, spark);
		//// �����G�t�F�N�g
		AnimationEffect* explosion = new AnimationEffect(15, Vector2(5, 1), Vector3::One * 0.5f, effectPos, L"Resources\\Sprite\\EffectParts\\FeightingEffect\\Explosion.png");
		Locator::GetEffectManager()->Register(DrawingOrder::Front, explosion);
		//// �X���b�V���G�t�F�N�g
		//SlashEffect* slash = new SlashEffect(15, effectPos, Vector3::Zero, Matrix::CreateRotationZ((XM_PIDIV2 * -m_plug.Dir)), L"Resources\\Sprite\\EffectParts\\RedSlash1.png");
		//Locator::GetEffectManager()->Register(DrawingOrder::Front, slash);

		Locator::GetSoundManager()->OneShot(103);

		// ���ꃂ�[�V����
		m_plug.Animation->SwitchAnimetion(m_rigor, 5);
		// ������
		m_plug.fighterState = FighterState::ATTACKED;
		// �_���[�W����
		Damage(damageData.Damage * ((*m_plug.Rival)->Emotions() + 1));
		// �������̋Z�͏���
		m_skillData = nullptr;
		
		// �����o������
		if (abs(GetTransform()->position.x + (damageData.Hit_Impact.x * dir)) < RIGHT_LIMIT)
			GetTransform()->velocity = Vector3(damageData.Hit_Impact.x * dir, damageData.Hit_Impact.y, 0);
		else
		{
			(*m_plug.Rival)->GetTransform()->velocity = Vector3(damageData.Hit_Impact.x * -dir, 0, 0);
			m_plug.transform->velocity = Vector3(0, damageData.Hit_Impact.y, 0);
		}
	}
	// ����������
	else
	{
		m_rigor = (*m_plug.Rival)->GetRigor() + damageData.GuardStunTime_Difference;
		effectPos = (GetTransform()->position + collisionCenter) / 2;
		rotato = atan2(-m_plug.Dir, 0);
		effectRotato = Matrix::CreateRotationZ(rotato);
		//// �h��G�t�F�N�g
		//ShockWaveEffect* effect = new ShockWaveEffect(m_rigor, effectPos, effectRotato, L"Resources\\Sprite\\Shock.png");
		//Locator::GetEffectManager()->Register(DrawingOrder::Front, effect);
		
		Locator::GetSoundManager()->OneShot(101);

		// �_���[�W����
		Damage(damageData.BlockDamage);

		if (abs(GetTransform()->position.x + (damageData.Guard_Impact.x * dir)) < RIGHT_LIMIT)
			GetTransform()->velocity = Vector3(damageData.Guard_Impact.x * dir, damageData.Guard_Impact.y, 0);
		else
			(*m_plug.Rival)->GetTransform()->velocity = Vector3(damageData.Guard_Impact.x * -dir, damageData.Guard_Impact.y, 0);
	}

	return false;
}

//// �\�����[�h
//void FighterFrame::BeastMode()
//{
//	// �����łȂ���Δ����ł��Ȃ�
//	if (m_beastLevel != BeastLevel::Peaceful)
//		return;
//	
//	// �̗͂𔼕���
//	int afterHP = m_plug.HP;
//	afterHP /= 2;
//	// �[����؂�グ��
//	if (afterHP * 2 < m_plug.HP)
//		afterHP++;
//	// �������
//	m_plug.HP = afterHP;
//
//	//// �����o��
//	//m_beastModeSoundInstance->Play();
//	//// �r�[�X�g���[�h�˓��A�j���[�V����
//	//m_plug.Animation->NowAnimation(12, 90,true);
//	//m_rigor = 90;
//	//// �\���G�t�F�N�g�`��J�n
//	//m_beastEffect->DrawMode(true);
//
//
//	// �̗͂̎c��c��(����)
//	float ratio = static_cast<float>(m_plug.HP) / static_cast<float>(MAXHP);
//	// �̗͂��ő�l��50���`31��
//	if (ratio > 0.31f)
//	{
//		// �{��
//		m_beastLevel = BeastLevel::Rage;
//	}
//	// �̗͂��ő�l��30���`6��
//	else if (ratio <= 0.3f && ratio > 0.06f)
//	{
//		// �֖�
//		m_beastLevel = BeastLevel::Ferocious;
//	}
//	// �̗͂��ő�l��5���`1��
//	else if (ratio <= 0.05f)
//	{
//		// ����
//		m_beastLevel = BeastLevel::Frenzy;
//	}
//}

// ���X�^�[�g
void FighterFrame::Restart()
{
	m_plug.HP = MAXHP;
	m_plug.JumpCount = 0;
	m_turning = 0;

	PeacefulHeart();
}

// �U���������
void FighterFrame::TurningAround()
{
	// �Z���[�V�������łȂ��A���肪���n�������A�����Ă�������̋t���ɑ��肪�s���Ă���TURNINGFRAME�t���[���o������
	if (!m_skillData && m_turning > TURNING_FRAME)
	{
		// ���������߂�
		float dir = (*m_plug.Rival)->GetTransform()->position.x - GetTransform()->position.x;
		if (dir != 0)
		{
			m_turning = 0;
			dir = dir / abs(dir);
			m_plug.Dir = dir;
			m_modelDir = dir;
		}
		m_turning = 0;
	}
	else m_turning++;
}

// ��������
void FighterFrame::PhysicalProcess()
{
	// �Q�[�����̕����@������Ԃɂ���ĕϓ�������
	switch (m_plug.fighterState)
	{
	case STAND:
	case FRONTWALK:
	case BACKWALK:
	case FighterState::KO:
		StandPhysics();
		break;
	case SQUAT:
		SquatPhysics();
		break;
	case FLY:
		FlyPhysics();
		break;
	case ATTACK:
		AttackPhysics();
	default:
		break;
	}

	// �n�ʂɒ��n��
	if (GetTransform()->velocity.y <= 0 && GetTransform()->position.y - (m_size.y / 2) <= 0)
	{
		GetTransform()->velocity.y = 0;
		GetTransform()->position.y = m_size.y / 2;
		if (m_plug.JumpCount  > 0)
		{
			m_plug.JumpCount = 0;
			m_plug.Rigidity = 2;
		}
		//�Z�����������������A�������̋Z���󒆏�Ԃ̋Z��������
		if (m_skillData && m_skillData->Condition == FighterState::FLY)
			m_skillData = nullptr; // �|�C���^���J����
	}

	// ���Z
	GetTransform()->position += GetTransform()->velocity;

	// ����Ƃ̔���
	if (isHittingRight((*m_plug.Rival)))
	{
		if ((*m_plug.Rival)->GetTransform()->position.x == LEFT_LIMIT)
		{
			GetTransform()->position.x = (*m_plug.Rival)->GetTransform()->position.x + ((*m_plug.Rival)->GetSize().x + m_size.x) * 0.5f;
		}
		else
		{
			(*m_plug.Rival)->GetTransform()->position.x = GetTransform()->position.x - ((*m_plug.Rival)->GetSize().x + m_size.x) * 0.5f;
		}
	}
	if (isHittingLeft((*m_plug.Rival)))
	{
		if ((*m_plug.Rival)->GetTransform()->position.x == RIGHT_LIMIT)
		{
			GetTransform()->position.x = (*m_plug.Rival)->GetTransform()->position.x - ((*m_plug.Rival)->GetSize().x + m_size.x) * 0.5f;
		}
		else
		{
			(*m_plug.Rival)->GetTransform()->position.x = GetTransform()->position.x + ((*m_plug.Rival)->GetSize().x + m_size.x) * 0.5f;
		}
	}
}

// ��ԑJ��
void FighterFrame::StateTransition(DX::StepTimer const& timer)
{
	// �X�e�[�g�X�V
	m_identity->StateUpdata(timer);
}

// �A�j���[�V�����J��
void FighterFrame::AnimationTransition()
{
	//�A�j���[�V�����̒��S���W���ړ�������
	m_plug.AnimationCenter.x = GetTransform()->position.x;
	// �摜�̃T�C�Y����
	m_plug.AnimationCenter.y = (GetTransform()->position.y -m_size.y / 2) + 1;
	// �����̕ύX
	m_plug.Animation->Direction(static_cast<SwichableAnimation::Dir>(m_plug.Dir));

	// �Z���o�Ă��Ȃ���ԂŃt�@�C�^�[�̃X�^���X���O�̃t���[���ƕς���Ă�����
	// �A�j���[�V������؂�ւ���
	if ((!m_skillData && m_plug.beforeState != m_plug.fighterState))
	{
		// �����Ă���������L�[�f�[�^�̕����ɕϊ�����
		KeyData data = DirectionToKeyData(m_plug.Dir);
		switch (m_plug.fighterState)
		{
		case FighterState::STAND:
			// �����A�j���[�V������ݒ肷��
			m_plug.Animation->SwitchAnimetion(60, 0);
			break;
		case FighterState::FRONTWALK:
			// �O�����A�j���[�V����
			m_plug.Animation->SwitchAnimetion(60, 1);
			break;
		case FighterState::BACKWALK:
			// ������A�j���[�V����
			m_plug.Animation->SwitchAnimetion(60, 2);
			break;
		case FighterState::SQUAT:
			// ���Ⴊ�݃A�j���[�V����
			m_plug.Animation->SwitchAnimetion(60, 3);
			break;
		case FighterState::FLY:
			// �󒆃A�j���[�V����
			m_plug.Animation->SwitchAnimetion(60, 4);
			break;
		case FighterState::KO:
			// �󒆃A�j���[�V����
			m_plug.Animation->SwitchAnimetion(60, 6);
			break;
		default:
			break;
		}
	}

	// �O�̃X�e�[�g�����X�V����
	m_plug.beforeState = m_plug.fighterState;
}

void FighterFrame::CommonOperation(DX::StepTimer const& timer)
{
	// �Z���������łȂ����
	if (!m_skillData)
	{
		if (
			m_plug.fighterState == FighterState::STAND ||// ������Ԃ܂���
			m_plug.fighterState == FighterState::FRONTWALK	 || 
			m_plug.fighterState == FighterState::BACKWALK	 // ������Ԃ�
			)
		{
			// �ړ�
			Move();
			// �W�����v
			Jump();
		}
	}
}

void FighterFrame::Throw()
{
	// ����
	int dir;
	// ���E�ǂ��炩�̓��͂�����Ă���
	if (m_identity->PushJudgment(KeyData::RIGHT))
	{
		dir = KeyDataToDirection(KeyData::RIGHT);
	}
	else if (m_identity->PushJudgment(KeyData::LEFT))
	{
		dir = KeyDataToDirection(KeyData::LEFT);
	}
	// ���͂���Ă��Ȃ�
	else
		// �������I����
		return;

	// ���U���{�^���Ɠ�������
	if (m_identity->PushJudgment(KeyData::MODERATO))
	{
		DamageData data;
		data.Damage = data.BlockDamage = 10;
		data.StunDamage = 10;
		data.Guard_Impact = data.Hit_Impact = Vector2(dir, 0.3f);
		data.DamageType = DamageData::Type::Throw;
		data.GuardStunTime_Difference = data.HitStunTime_Difference = 30;
		m_rigor = 5;

		if (abs((*m_plug.Rival)->GetTransform()->position.x - GetTransform()->position.x) < m_size.x / 0.75f)
		{
			(*m_plug.Rival)->Attacked(data, (*m_plug.Rival)->GetTransform()->position);
			(*m_plug.Rival)->GetTransform()->position.x = GetTransform()->position.x + m_size.x * 0.5f * dir;
		}
	}
}

void FighterFrame::UpdateIdentity(DX::StepTimer const& timer)
{
	// �L�����ʂ̍X�V����
	m_identity->Update(timer);
}

void FighterFrame::TechniqueDecision()
{
	// �������̋Z���������
	if (!m_skillData)
	{
		// �R�}���h�𒲂ׂĔ����\�ȋZ�𒲂ׂ�(�|�C���^�[�̐����������_��)
		if (m_skillData = m_identity->Find_Matched_Skill())
		{
			// �_���[�W�f�[�^
			ImpactData impact = m_skillData->Impact;
			// �ړ��f�[�^
			MoveMentData movement = m_skillData->Movement;
			// ���̋Z���I���܂ł̎��Ԃ�ݒ�
			m_rigor = impact.StartUpTime + impact.ActiveTime + impact.RigidityTime;
			//// �A�j���[�V�����J�n
			m_plug.Animation->SwitchAnimetion(m_rigor, m_skillData->AnimationID);
		}
	}
}

void FighterFrame::TechniqueUpdate()
{
	// �Z�������\�Ȃ�
	if (m_skillData)
	{
		// �Ռ��g�f�[�^
		ImpactData impact = m_skillData->Impact;
		// �ړ��f�[�^
		MoveMentData movement = m_skillData->Movement;
		// �_���[�W�f�[�^
		DamageData damage = impact.Damage;

		// �L�����Z������Ȃ������Ƃ��ɋZ����₷������
		int totalTime = impact.StartUpTime + impact.ActiveTime + impact.RigidityTime;

		// �������I�������
		if (m_rigor == totalTime - impact.StartUpTime)
		{
			// �ړ�(�n�ォ�A�󒆂ɂ��āA���Z�ɂ��ړ����Ȃ���)
			if (GetTransform()->position.y == m_plug.Size->y / 2 || (GetTransform()->position.y > m_plug.Size->y / 2 && movement.constant.Length() > 0))
				GetTransform()->velocity += Vector3(movement.constant.x * m_plug.Dir, movement.constant.y, 0);

			// �U���R���C�_�[�̍쐬
			AttackedCollider* collision = new AttackedCollider(
				(*m_plug.Rival),
				&GetTransform()->position,
				impact
			);
			Locator::GetCollisionManager()->Register(collision);
		}

		// �U����
		m_plug.fighterState = FighterState::ATTACK;
	}
}

bool FighterFrame::RigorDecision()
{
	// �d���J�E���^�[�����炷
	if (m_rigor > 0)
		m_rigor--;

	// �J�E���^�[��0��������
	if (m_rigor == 0)
		m_skillData = nullptr; // �|�C���^���J����(�����Ȃ�)

	return m_rigor == 0;
}

void FighterFrame::PositionAdjustment()
{
	// ���E�̉����E
	// �X�e�[�W�T�C�Y�̌��E
	GetTransform()->position.x = std::fminf(std::fmaxf(LEFT_LIMIT, GetTransform()->position.x), RIGHT_LIMIT);
	// ���C�o�����痣�������E
	float rivalX = (*m_plug.Rival)->GetTransform()->position.x;
	GetTransform()->position.x = std::fminf(std::fmaxf(rivalX - PLAYER_INTERVAL_LIMIT, GetTransform()->position.x), rivalX + PLAYER_INTERVAL_LIMIT);

	//m_playerNumberIcon->SetPosition(GetTransform()->position);
}

// �\�����[�h������
void FighterFrame::PeacefulHeart()
{
	// �r�[�X�g���[�h�I�t
	m_beastLevel = BeastLevel::Peaceful;
	//// �`�悵�Ȃ�
	//m_beastEffect->DrawMode(false);
}

void FighterFrame::Move()
{
	// �ړ�
	if (m_identity->PushJudgment(KeyData::LEFT))
	{
		m_plug.transform->velocity.x -= m_plug.Speed;
		if (m_plug.transform->velocity.x <= -m_plug.Speed)
		{
			m_plug.transform->velocity.x = -m_plug.Speed;
		}
	}
	if (m_identity->PushJudgment(KeyData::RIGHT))
	{
		m_plug.transform->velocity.x += m_plug.Speed;
		if (m_plug.transform->velocity.x >= m_plug.Speed)
		{
			m_plug.transform->velocity.x = m_plug.Speed;
		}
	}
}

void FighterFrame::Jump()
{
	// �W�����v
	if (m_identity->PushJudgment(KeyData::UP))
	{
		m_jumpClock = 0;
		m_plug.fighterState = FighterState::FLY;
		Locator::GetSoundManager()->OneShot(100);
	
		//Locator::GetEffectManager()->Register(DrawingOrder::Front, new ShockWaveEffect(6, m_plug.transform->position, Matrix::Identity, L"Resources\\Sprite\\Shock.png"));
	}
}

void FighterFrame::Damage(int damage)
{
	m_plug.HP -= damage;
}

bool FighterFrame::JudgeGuard(DamageData damageData, Vector3 collisionCenter)
{
	// �K�[�h�̕����̌v�Z
	float vec = 0;
	vec = GetTransform()->position.x - collisionCenter.x;
	KeyData dir;
	if (vec < 0)
	{
		dir = KeyData::LEFT;
	}
	if (0 < vec)
	{
		dir = KeyData::RIGHT;
	}

	return 
		(
			m_plug.JumpCount == 0 && // �W�����v����
			damageData.DamageType == AttackedCollider::Type::Throw // �������肾������
		) ||
		(
			m_plug.JumpCount == 0 && // �W�����v���łȂ�
			m_identity->PushJudgment(dir) && // �K�[�h�͂��Ă���
			(
				(
					m_identity->PushJudgment(KeyData::DOWN) &&
					(damageData.DamageType == AttackedCollider::Type::High || damageData.DamageType == AttackedCollider::Type::Low) //���Ⴊ�݃K�[�h
					) ||
					(
					!(m_identity->PushJudgment(KeyData::DOWN)) &&
					(damageData.DamageType == AttackedCollider::Type::High || damageData.DamageType == AttackedCollider::Type::OverHead) // �����K�[�h
				)
			)
		);
}

void FighterFrame::StandPhysics()
{
	// �����x����
	GetTransform()->velocity.x *= 0.5f;
	m_jumpClock = 0;
}

void FighterFrame::SquatPhysics()
{
	// �����x����
	GetTransform()->velocity.x = GetTransform()->velocity.z = 0.0f;
	m_jumpClock = 0;
}

void FighterFrame::FlyPhysics()
{
	m_jumpClock++;
	if (m_plug.transform->velocity.y == 0)
		m_plug.transform->position.y = (sin((XM_PI / JUMP_INTERVAL) * m_jumpClock) * JUMP_HEIGHT) + m_plug.Size->y / 2;
	else
	{
		m_plug.transform->velocity.x *= 0.8f;
		m_plug.transform->velocity.y -= GRAVITY;
	}
}

void FighterFrame::AttackPhysics()
{
	// �����x����
	if(GetTransform()->position.y == m_size.y / 2)
		GetTransform()->velocity.x *= 0.5f;
	m_jumpClock = 0;
}

void FighterFrame::StancesChange(DX::StepTimer const& timer)
{
	// KO
	if (KO())
	{
		m_plug.fighterState = FighterState::KO;
	}
	// ���
	else if (GetTransform()->position.y > m_size.y / 2)
	{
		m_plug.fighterState = FighterState::FLY;
	}
	// ���Ⴊ��
	else if (m_identity->PushJudgment(KeyData::DOWN) && !(m_plug.fighterState == FighterState::FLY))
	{
		m_plug.fighterState = FighterState::SQUAT;
		m_plug.transform->position.y = m_size.y / 2;
	}
	else if (
		m_identity->PushJudgment(KeyData::RIGHT) ||
		m_identity->PushJudgment(KeyData::LEFT)
		)
	{
		// �����Ă�������ւ̃L�[��������Ă�����
		if (m_identity->PushJudgment(DirectionToKeyData(m_plug.Dir)))
		{
			// �t�����̃L�[��������Ă��Ȃ����
			if(!m_identity->PushJudgment(DirectionToKeyData(m_plug.Dir * -1)))
				m_plug.fighterState = FighterState::FRONTWALK; // �O�ɕ���
			else
				m_plug.fighterState = FighterState::STAND; // ����(���̏�ōd������)
		}
		else
		{
			m_plug.fighterState = FighterState::BACKWALK; // ���ɕ���
		}
	}
	// ����
	else
	{
		m_plug.fighterState = FighterState::STAND;
	}
}

