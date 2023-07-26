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
		// 初期化処理
		void initialize() override;
		// 生成処理
		void Create() override;
		// 更新処理
		void Update(DX::StepTimer const& timer) override;
		// 描画処理
		void Render() override;
		// 終了処理
		void Finalize() override;
	private:
		/// ステート
		// 準備
		void ReadyState(const DX::StepTimer& timer);
		// スタート
		void StartState(const DX::StepTimer& timer);
		// 戦闘
		void FightingState(const DX::StepTimer& timer);
		// 時間切れ
		void TimeUpState(const DX::StepTimer& timer);
		// ドラマチック演出
		void DramaticState(const DX::StepTimer& timer);
		// KO
		void KOState(const DX::StepTimer& timer);
		// リザルト前
		void BeforeResultState(const DX::StepTimer& timer);
		/// 更新
		// ステージのアップデート
		void StageUpdate(const DX::StepTimer& timer);
		// ファイターのアップデート
		void FightersUpdate(const DX::StepTimer& timer);
		// マネージャのアップデート
		void ManagersUpdate(const DX::StepTimer& timer);
		// カウントダウンクロックのアップデート
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
		// ステート管理用のポインタ
		void(DuelScene::*m_nowState)(const DX::StepTimer&);
		// プレイヤー1(左側)
		FighterFrame* P1;
		// プレイヤー2(右側)
		FighterFrame* P2;
		// プレイヤー1のコントローラ番号
		int P1CNumber;
		// プレイヤー2のコントローラ番号
		int P2CNumber;
		// コモンステート
		CommonStates* m_commonState;
		// プレイヤー1の体力
		RectangleGage* m_p1HP;
		// プレイヤー2の体力
		RectangleGage* m_p2HP;
		// 残り時間
		float m_limitTime;
		// カメラのX座標
		float m_nowCameraXposition;
		// ファイターの描画順を管理する変数
		int m_fastRender;
		//ステージ
		std::unique_ptr<IStage> m_stage;
		// 残り時間タイマー
		std::unique_ptr<SegmentCounter> m_segment;
		// 先に描画するプレイヤー
		float m_lastDepth = 0;
		// FPS値
		float m_fps;
		// エフェクト
		std::unique_ptr<BasicEffect> m_effect;
		// ドラマチックトリガー
		bool m_dramaticTrigger;
		// カメラ番号
		int m_cameraNum;
};

