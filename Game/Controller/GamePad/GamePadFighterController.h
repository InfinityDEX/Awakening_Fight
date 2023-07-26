#pragma once

#include <Game\Controller\Interface\IController.h>
#include <GamePad.h>

using namespace DirectX;

// ゲームパッドのラッパー
class GamePadFighterController final: public IController
{
	public:
		// コンストラクタ
		GamePadFighterController(int num = 0);
		// デストラクタ
		~GamePadFighterController() override;
		// 入力データの取得
		virtual int GetInput() override;
	private:
		// ゲームパッド
		static GamePad GAMEPAD;
		// ゲームパッドのトラッカー
		GamePad::ButtonStateTracker m_tracker;
		// ゲームパッドの個別番号
		int m_padNum;
};

