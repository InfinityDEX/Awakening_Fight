#pragma once
#include <Game\Scene\Interface\IScene.h>
#include <Game\Controller\Interface\IController.h>
#include <Game\Effect\PrimitiveEffect\Unlimited\UnlimitedEffect.h>
#include <Game\Effect\PrimitiveEffect\SwichableAnimation\SwichableAnimation.h>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <CommonStates.h>
#include <Audio.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class ControllerSelectScene : public IScene
{
	public:
		ControllerSelectScene();
		~ControllerSelectScene() override;
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
		// キーボードの更新
		void KeyboardUpdate();
		// ゲームパッド１の更新
		void GamePad1Update();
		// ゲームパッド２の更新
		void GamePad2Update();
	private:
		// プレイヤー１のコントローラ番号
		int m_p1;
		// プレイヤー２のコントローラ番号
		int m_p2;
		// メニューの下地
		UnlimitedEffect* m_backGrand;
		// プレイヤー１アイコン
		UnlimitedEffect* m_p1Icon;
		// プレイヤー２アイコン
		UnlimitedEffect* m_p2Icon;
		// プレイヤー１がどのコントローラになったかを表すアイコン
		SwichableAnimation* m_p1ControllerIcon;
		// m_p1ControllerIconの座標
		Vector3 m_p1CIconPos;
		// プレイヤー２がどのコントローラになったかを表すアイコン
		SwichableAnimation* m_p2ControllerIcon;
		// m_p2ControllerIconの座標
		Vector3 m_p2CIconPos;
};

