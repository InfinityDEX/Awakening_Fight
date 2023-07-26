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

// コンストラクタ
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

// デストラクタ
FighterFrame::~FighterFrame()
{
}

// 生成処理
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

	//// アニメーションを初期化
	//m_beastEffect = new AnimationEffect(10, Vector2(5, 2), Vector3::One, GetTransform()->position, L"Resources\\Sprite\\EffectParts\\FightingEffect\\Aura.png", true);
	//Locator::GetEffectManager()->Register(DrawingOrder::Back, m_beastEffect);
	//m_beastEffect->DrawMode(false);
}

// 初期化処理
void FighterFrame::Initialize()
{
	// 座標データの結びつけ
	m_plug.transform = GetTransform();
	// プラグの初期化
	m_identity->Initialize(&m_plug);

	// 立ちアニメーションを設定する
	m_plug.Animation->SwitchAnimetion(60, 0);
	
	// 向きを決める
	float vec = (*m_plug.Rival)->GetTransform()->position.x - GetTransform()->position.x;
	if (vec != 0)
	{
		m_turning = 0;
		vec = vec / abs(vec);
		m_plug.Dir = vec;
	}

	// 初期座標を調整
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

	// 暴走モード初期化
	PeacefulHeart();
}

// 更新処理
void FighterFrame::Update(DX::StepTimer const& timer)
{
	// 振り向き処理
	TurningAround();
	// 技更新
	TechniqueUpdate();
	// キャラがコントロールできるか？
	if (!m_controlLock)
	{
		// キャラ別の更新処理
		UpdateIdentity(timer);
		// 硬直中か判定
		if (RigorDecision())
		{
			// 投げ判定
			Throw();
			// 共通操作
			CommonOperation(timer);
			// 技判定
			TechniqueDecision();
		}

	}
	// 物理処理
	PhysicalProcess();
	// アニメーション遷移
	AnimationTransition();
	// 状態処理
	StateTransition(timer);
	// 状態変更
	StancesChange(timer);
	// 位置調整
	PositionAdjustment();
}

// 描画関数
void FighterFrame::Render(const Matrix view, const Matrix proj)
{
	Matrix position;
	Matrix rotation;

	auto deviceResouces = Locator::GetDeviceResources();
	position = Matrix::CreateTranslation(GetTransform()->position);
	rotation = Matrix::CreateRotationY(XMConvertToRadians(90) * m_plug.Dir);
}

// ヒットボックスの描画関数
void FighterFrame::IsHitBoxRender(const Matrix view, const Matrix proj)
{
	Matrix world;
	world = Matrix::CreateScale(*m_plug.Size);
	world *= Matrix::CreateTranslation(m_plug.transform->position);
	m_box->Draw(world, view, proj, Colors::White, nullptr, true);
}

// リセット関数
void FighterFrame::Reset()
{

}

// 攻撃判定
bool FighterFrame::Attacked(DamageData damageData, Vector3 collisionCenter)
{
	// 飛ぶ方向の計算
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

	// ガードに失敗するか硬直中なら
	if(!judge || m_rigor > 0)
	{
		m_rigor = (*m_plug.Rival)->GetRigor() + damageData.HitStunTime_Difference;
		effectPos = Vector3(GetTransform()->position.x, collisionCenter.y, 0);
		rotato = atan2(attackToMe.x, -attackToMe.y);
		effectRotato = Matrix::CreateRotationZ(rotato);
		// パーティクルエフェクト
		//Spark* spark = new Spark(Vector3(GetTransform()->position.x, collisionCenter.y, 0), Vector3(attackToMe.x, 0, 0), XMConvertToRadians(120), 5, 3.0f, 2.0f);
		//Locator::GetEffectManager()->Register(DrawingOrder::Front, spark);
		//// 爆発エフェクト
		AnimationEffect* explosion = new AnimationEffect(15, Vector2(5, 1), Vector3::One * 0.5f, effectPos, L"Resources\\Sprite\\EffectParts\\FeightingEffect\\Explosion.png");
		Locator::GetEffectManager()->Register(DrawingOrder::Front, explosion);
		//// スラッシュエフェクト
		//SlashEffect* slash = new SlashEffect(15, effectPos, Vector3::Zero, Matrix::CreateRotationZ((XM_PIDIV2 * -m_plug.Dir)), L"Resources\\Sprite\\EffectParts\\RedSlash1.png");
		//Locator::GetEffectManager()->Register(DrawingOrder::Front, slash);

		Locator::GetSoundManager()->OneShot(103);

		// やられモーション
		m_plug.Animation->SwitchAnimetion(m_rigor, 5);
		// やられ状態
		m_plug.fighterState = FighterState::ATTACKED;
		// ダメージ処理
		Damage(damageData.Damage * ((*m_plug.Rival)->Emotions() + 1));
		// 発動中の技は消す
		m_skillData = nullptr;
		
		// 押し出し処理
		if (abs(GetTransform()->position.x + (damageData.Hit_Impact.x * dir)) < RIGHT_LIMIT)
			GetTransform()->velocity = Vector3(damageData.Hit_Impact.x * dir, damageData.Hit_Impact.y, 0);
		else
		{
			(*m_plug.Rival)->GetTransform()->velocity = Vector3(damageData.Hit_Impact.x * -dir, 0, 0);
			m_plug.transform->velocity = Vector3(0, damageData.Hit_Impact.y, 0);
		}
	}
	// 成功したら
	else
	{
		m_rigor = (*m_plug.Rival)->GetRigor() + damageData.GuardStunTime_Difference;
		effectPos = (GetTransform()->position + collisionCenter) / 2;
		rotato = atan2(-m_plug.Dir, 0);
		effectRotato = Matrix::CreateRotationZ(rotato);
		//// 防御エフェクト
		//ShockWaveEffect* effect = new ShockWaveEffect(m_rigor, effectPos, effectRotato, L"Resources\\Sprite\\Shock.png");
		//Locator::GetEffectManager()->Register(DrawingOrder::Front, effect);
		
		Locator::GetSoundManager()->OneShot(101);

		// ダメージ処理
		Damage(damageData.BlockDamage);

		if (abs(GetTransform()->position.x + (damageData.Guard_Impact.x * dir)) < RIGHT_LIMIT)
			GetTransform()->velocity = Vector3(damageData.Guard_Impact.x * dir, damageData.Guard_Impact.y, 0);
		else
			(*m_plug.Rival)->GetTransform()->velocity = Vector3(damageData.Guard_Impact.x * -dir, damageData.Guard_Impact.y, 0);
	}

	return false;
}

//// 暴走モード
//void FighterFrame::BeastMode()
//{
//	// 平穏でなければ発動できない
//	if (m_beastLevel != BeastLevel::Peaceful)
//		return;
//	
//	// 体力を半分に
//	int afterHP = m_plug.HP;
//	afterHP /= 2;
//	// 端数を切り上げる
//	if (afterHP * 2 < m_plug.HP)
//		afterHP++;
//	// 代入する
//	m_plug.HP = afterHP;
//
//	//// 音を出す
//	//m_beastModeSoundInstance->Play();
//	//// ビーストモード突入アニメーション
//	//m_plug.Animation->NowAnimation(12, 90,true);
//	//m_rigor = 90;
//	//// 暴走エフェクト描画開始
//	//m_beastEffect->DrawMode(true);
//
//
//	// 体力の残り残量(割合)
//	float ratio = static_cast<float>(m_plug.HP) / static_cast<float>(MAXHP);
//	// 体力が最大値の50％～31％
//	if (ratio > 0.31f)
//	{
//		// 怒り
//		m_beastLevel = BeastLevel::Rage;
//	}
//	// 体力が最大値の30％～6％
//	else if (ratio <= 0.3f && ratio > 0.06f)
//	{
//		// 獰猛
//		m_beastLevel = BeastLevel::Ferocious;
//	}
//	// 体力が最大値の5％～1％
//	else if (ratio <= 0.05f)
//	{
//		// 狂乱
//		m_beastLevel = BeastLevel::Frenzy;
//	}
//}

// リスタート
void FighterFrame::Restart()
{
	m_plug.HP = MAXHP;
	m_plug.JumpCount = 0;
	m_turning = 0;

	PeacefulHeart();
}

// 振り向き処理
void FighterFrame::TurningAround()
{
	// 技モーション中でなく、相手が着地したか、向いている方向の逆側に相手が行ってからTURNINGFRAMEフレーム経ったら
	if (!m_skillData && m_turning > TURNING_FRAME)
	{
		// 向きを決める
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

// 物理処理
void FighterFrame::PhysicalProcess()
{
	// ゲーム内の物理法則を状態によって変動させる
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

	// 地面に着地時
	if (GetTransform()->velocity.y <= 0 && GetTransform()->position.y - (m_size.y / 2) <= 0)
	{
		GetTransform()->velocity.y = 0;
		GetTransform()->position.y = m_size.y / 2;
		if (m_plug.JumpCount  > 0)
		{
			m_plug.JumpCount = 0;
			m_plug.Rigidity = 2;
		}
		//技が発動中だった時、もしこの技が空中状態の技だったら
		if (m_skillData && m_skillData->Condition == FighterState::FLY)
			m_skillData = nullptr; // ポインタを開ける
	}

	// 加算
	GetTransform()->position += GetTransform()->velocity;

	// 相手との判定
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

// 状態遷移
void FighterFrame::StateTransition(DX::StepTimer const& timer)
{
	// ステート更新
	m_identity->StateUpdata(timer);
}

// アニメーション遷移
void FighterFrame::AnimationTransition()
{
	//アニメーションの中心座標を移動させる
	m_plug.AnimationCenter.x = GetTransform()->position.x;
	// 画像のサイズから
	m_plug.AnimationCenter.y = (GetTransform()->position.y -m_size.y / 2) + 1;
	// 向きの変更
	m_plug.Animation->Direction(static_cast<SwichableAnimation::Dir>(m_plug.Dir));

	// 技が出ていない状態でファイターのスタンスが前のフレームと変わっていたら
	// アニメーションを切り替える
	if ((!m_skillData && m_plug.beforeState != m_plug.fighterState))
	{
		// 向いている方向をキーデータの方向に変換する
		KeyData data = DirectionToKeyData(m_plug.Dir);
		switch (m_plug.fighterState)
		{
		case FighterState::STAND:
			// 立ちアニメーションを設定する
			m_plug.Animation->SwitchAnimetion(60, 0);
			break;
		case FighterState::FRONTWALK:
			// 前歩きアニメーション
			m_plug.Animation->SwitchAnimetion(60, 1);
			break;
		case FighterState::BACKWALK:
			// 後歩きアニメーション
			m_plug.Animation->SwitchAnimetion(60, 2);
			break;
		case FighterState::SQUAT:
			// しゃがみアニメーション
			m_plug.Animation->SwitchAnimetion(60, 3);
			break;
		case FighterState::FLY:
			// 空中アニメーション
			m_plug.Animation->SwitchAnimetion(60, 4);
			break;
		case FighterState::KO:
			// 空中アニメーション
			m_plug.Animation->SwitchAnimetion(60, 6);
			break;
		default:
			break;
		}
	}

	// 前のステート情報を更新する
	m_plug.beforeState = m_plug.fighterState;
}

void FighterFrame::CommonOperation(DX::StepTimer const& timer)
{
	// 技が発動中でなければ
	if (!m_skillData)
	{
		if (
			m_plug.fighterState == FighterState::STAND ||// 立ち状態または
			m_plug.fighterState == FighterState::FRONTWALK	 || 
			m_plug.fighterState == FighterState::BACKWALK	 // 歩き状態で
			)
		{
			// 移動
			Move();
			// ジャンプ
			Jump();
		}
	}
}

void FighterFrame::Throw()
{
	// 方向
	int dir;
	// 左右どちらかの入力がされている
	if (m_identity->PushJudgment(KeyData::RIGHT))
	{
		dir = KeyDataToDirection(KeyData::RIGHT);
	}
	else if (m_identity->PushJudgment(KeyData::LEFT))
	{
		dir = KeyDataToDirection(KeyData::LEFT);
	}
	// 入力されていない
	else
		// 処理を終える
		return;

	// 中攻撃ボタンと同時押し
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
	// キャラ別の更新処理
	m_identity->Update(timer);
}

void FighterFrame::TechniqueDecision()
{
	// 発動中の技が無ければ
	if (!m_skillData)
	{
		// コマンドを調べて発動可能な技を調べる(ポインターの先を消しちゃダメ)
		if (m_skillData = m_identity->Find_Matched_Skill())
		{
			// ダメージデータ
			ImpactData impact = m_skillData->Impact;
			// 移動データ
			MoveMentData movement = m_skillData->Movement;
			// この技が終わるまでの時間を設定
			m_rigor = impact.StartUpTime + impact.ActiveTime + impact.RigidityTime;
			//// アニメーション開始
			m_plug.Animation->SwitchAnimetion(m_rigor, m_skillData->AnimationID);
		}
	}
}

void FighterFrame::TechniqueUpdate()
{
	// 技が発動可能なら
	if (m_skillData)
	{
		// 衝撃波データ
		ImpactData impact = m_skillData->Impact;
		// 移動データ
		MoveMentData movement = m_skillData->Movement;
		// ダメージデータ
		DamageData damage = impact.Damage;

		// キャンセルされなかったときに技が費やす総時間
		int totalTime = impact.StartUpTime + impact.ActiveTime + impact.RigidityTime;

		// 発生が終わったら
		if (m_rigor == totalTime - impact.StartUpTime)
		{
			// 移動(地上か、空中にいて、かつ技による移動がない時)
			if (GetTransform()->position.y == m_plug.Size->y / 2 || (GetTransform()->position.y > m_plug.Size->y / 2 && movement.constant.Length() > 0))
				GetTransform()->velocity += Vector3(movement.constant.x * m_plug.Dir, movement.constant.y, 0);

			// 攻撃コライダーの作成
			AttackedCollider* collision = new AttackedCollider(
				(*m_plug.Rival),
				&GetTransform()->position,
				impact
			);
			Locator::GetCollisionManager()->Register(collision);
		}

		// 攻撃中
		m_plug.fighterState = FighterState::ATTACK;
	}
}

bool FighterFrame::RigorDecision()
{
	// 硬直カウンターを減らす
	if (m_rigor > 0)
		m_rigor--;

	// カウンターが0だったら
	if (m_rigor == 0)
		m_skillData = nullptr; // ポインタを開ける(消さない)

	return m_rigor == 0;
}

void FighterFrame::PositionAdjustment()
{
	// 左右の可動限界
	// ステージサイズの限界
	GetTransform()->position.x = std::fminf(std::fmaxf(LEFT_LIMIT, GetTransform()->position.x), RIGHT_LIMIT);
	// ライバルから離れられる限界
	float rivalX = (*m_plug.Rival)->GetTransform()->position.x;
	GetTransform()->position.x = std::fminf(std::fmaxf(rivalX - PLAYER_INTERVAL_LIMIT, GetTransform()->position.x), rivalX + PLAYER_INTERVAL_LIMIT);

	//m_playerNumberIcon->SetPosition(GetTransform()->position);
}

// 暴走モード初期化
void FighterFrame::PeacefulHeart()
{
	// ビーストモードオフ
	m_beastLevel = BeastLevel::Peaceful;
	//// 描画しない
	//m_beastEffect->DrawMode(false);
}

void FighterFrame::Move()
{
	// 移動
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
	// ジャンプ
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
	// ガードの方向の計算
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
			m_plug.JumpCount == 0 && // ジャンプ中で
			damageData.DamageType == AttackedCollider::Type::Throw // 投げ判定だったら
		) ||
		(
			m_plug.JumpCount == 0 && // ジャンプ中でない
			m_identity->PushJudgment(dir) && // ガードはしている
			(
				(
					m_identity->PushJudgment(KeyData::DOWN) &&
					(damageData.DamageType == AttackedCollider::Type::High || damageData.DamageType == AttackedCollider::Type::Low) //しゃがみガード
					) ||
					(
					!(m_identity->PushJudgment(KeyData::DOWN)) &&
					(damageData.DamageType == AttackedCollider::Type::High || damageData.DamageType == AttackedCollider::Type::OverHead) // 立ちガード
				)
			)
		);
}

void FighterFrame::StandPhysics()
{
	// 加速度減衰
	GetTransform()->velocity.x *= 0.5f;
	m_jumpClock = 0;
}

void FighterFrame::SquatPhysics()
{
	// 加速度減衰
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
	// 加速度減衰
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
	// 飛ぶ
	else if (GetTransform()->position.y > m_size.y / 2)
	{
		m_plug.fighterState = FighterState::FLY;
	}
	// しゃがむ
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
		// 向いている方向へのキーが押されていたら
		if (m_identity->PushJudgment(DirectionToKeyData(m_plug.Dir)))
		{
			// 逆向きのキーが押されていなければ
			if(!m_identity->PushJudgment(DirectionToKeyData(m_plug.Dir * -1)))
				m_plug.fighterState = FighterState::FRONTWALK; // 前に歩く
			else
				m_plug.fighterState = FighterState::STAND; // 立つ(その場で硬直する)
		}
		else
		{
			m_plug.fighterState = FighterState::BACKWALK; // 後ろに歩く
		}
	}
	// 立つ
	else
	{
		m_plug.fighterState = FighterState::STAND;
	}
}

