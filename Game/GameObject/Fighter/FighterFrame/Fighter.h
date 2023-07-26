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

// ファイターの状態
enum FighterState
{
	STAND, // 立ち
	SQUAT, // しゃがみ
	FLY, // 空中
	FRONTWALK, // 前歩き
	BACKWALK, // 後歩き
	ATTACK, // 攻撃した
	ATTACKED, // 攻撃された
	KO, // KOした
};
// ファイターの個性のクラスと共有するファイターの情報
struct FighterShareData
{
	// ファイターの状態
	FighterState fighterState = FighterState::STAND;
	// 前回のファイターの状態
	FighterState beforeState = fighterState;
	// 自分
	FighterFrame* Me; 
	// 対戦相手
	FighterFrame** Rival; 
	// コントローラー番号
	int ControllerNum;
	// ファイターのアニメーション
	SwichableAnimation* Animation;
	// アニメーション座標
	Vector3 AnimationCenter; 
	// 座標系
	Transform* transform;
	// 大きさ
	Vector3* Size; 
	// 硬直時間
	int Rigidity;
	// 移動スピード
	float Speed;
 	// ジャンプ回数
   	int JumpCount;
	// 体力
	int HP;
	// キャラクターの向き
	int Dir;
};

// 暴走レベル
enum BeastLevel
{
	Peaceful, // 平穏
	Rage, // 激怒
	Ferocious, // 獰猛
	Frenzy, // 狂乱
};

// ファイターの骨格
class FighterFrame : public Box
{	
	public:
		// コンストラクタ
		FighterFrame(FighterIdentity* identity, FighterFrame** rival, int controllerNum, int playerNumber, Vector3 pos = Vector3::Zero);
		// デストラクタ
		~FighterFrame();
		// 生成関数
		virtual void Create();
		// 初期化関数
		virtual void Initialize();
		// 更新関数
		virtual void Update(DX::StepTimer const& timer) override;
		// 描画関数
		virtual void Render(const Matrix view, const Matrix proj) override;
		// ヒットボックスの描画関数
		virtual void IsHitBoxRender(const Matrix view, const Matrix proj);
		// リセット関数
		virtual void Reset();
		// リスタート
		void Restart();
		// 攻撃判定
		bool Attacked(DamageData damageData,Vector3 collisionCenter);
		//// ビーストモードオン
		//void BeastMode();
		// 暴走レベルの取得
		const BeastLevel Emotions() { return m_beastLevel; };

		//////////////////////////////////
		//
		// 更新処理
		//
		//////////////////////////////////
		
		// 振り向き処理
		void TurningAround();
		// 物理処理
		void PhysicalProcess();
		// 状態遷移
		void StateTransition(DX::StepTimer const& timer);
		// アニメーション遷移
		void AnimationTransition();
		// 硬直判定
		bool RigorDecision();
		// 共通操作
		void CommonOperation(DX::StepTimer const& timer);
		// 投げ判定
		void Throw();
		// キャラ別の更新処理
		void UpdateIdentity(DX::StepTimer const& timer);
		// 技判定
		void TechniqueDecision();
		// 技更新
		void TechniqueUpdate();
		// 状態更新
		void StancesChange(DX::StepTimer const& timer);
		// 位置調整
		void PositionAdjustment();

		//////////////////////////////////

		// 攻撃中
		bool Attacking() { return m_skillData != nullptr; };
		// HPの参照
		const int& HPReference() { return m_plug.HP; };
		//FighterState GetFighterState() { return m_plug.fighterState;};
		// KOの参照
		bool KO() { return m_plug.HP <= 0; };
		// 操作不能
		static void ControlLock() { m_controlLock = true; };
		// 操作可能
		static void ControlOpen() { m_controlLock = false; };
		// 硬直時間を返す
		int GetRigor() { return m_rigor; };
		// 振り向き時間
		static const int TURNING_FRAME;
		// ジャンプの対空時間
		static const int JUMP_INTERVAL;
		// ジャンプの高さ
		static const int JUMP_HEIGHT;
		// 入力を受け付けない
		static bool m_controlLock;
		static const int MAXHP;
		static const int MAXSP;
		// 左の壁
		static const float LEFT_LIMIT;
		// 右の壁
		static const float RIGHT_LIMIT;
		// プレイヤー同士が離れられる距離
		static const float PLAYER_INTERVAL_LIMIT;
	private:
		// 暴走モード初期化
		void PeacefulHeart();
		void Move();
		void Jump();
		void Damage(int damage);
		bool JudgeGuard(DamageData damageData, Vector3 collisionCenter);
		void StandPhysics();
		void SquatPhysics();
		void FlyPhysics();
		void AttackPhysics();

		// ファイターの個性
		std::unique_ptr<FighterIdentity> m_identity;

		// プレイヤー番号
		const int m_playerNumber;
		int m_rigor;
		std::unique_ptr<GeometricPrimitive>	m_box;
		Matrix m_rotate;
		// 発動中の技データ
		const SkillData* m_skillData;
		int m_turning;
		int m_modelDir;
		int m_jumpClock;
		// 暴走レベル
		BeastLevel m_beastLevel;
		// 暴走エフェクト
		AnimationEffect* m_beastEffect;
		// キャラ別処理クラスに渡すプラグ
		FighterShareData m_plug;
		std::unique_ptr<EffectFactory> m_factory;
		std::unique_ptr<CommonStates> m_state;
};
#endif // FIGHTER