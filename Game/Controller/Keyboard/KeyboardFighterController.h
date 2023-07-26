#pragma once

#include <Game\Controller\Interface\IController.h>
#include <Keyboard.h>

using namespace DirectX;

// キーボードのラッパー
class KeyboardFighterController final: public IController
{
	public:
		// コンストラクタ
		KeyboardFighterController();
		// デストラクタ
		~KeyboardFighterController();
		// 入力データの取得
		virtual int GetInput() override;
	private:
		// キーボード
		static Keyboard KEYBOARD;
		// キーボードのトラッカー
		Keyboard::KeyboardStateTracker m_tracker;
};

