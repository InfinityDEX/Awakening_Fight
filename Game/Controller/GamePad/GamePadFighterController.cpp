#include "pch.h"
#include "GamePadFighterController.h"

// ゲームパッドの生成
GamePad GamePadFighterController::GAMEPAD;

// コンストラクタ
GamePadFighterController::GamePadFighterController(int num)
	: m_padNum(num)
{}

// デストラクタ
GamePadFighterController::~GamePadFighterController()
{}

// 入力データの取得
int GamePadFighterController::GetInput()
{
	auto gp = GamePad::Get().GetState(m_padNum);
	m_tracker.Update(gp);

	// 初期化
	int command = 0;

	// レバー入力
	if (gp.IsLeftThumbStickRight())
	{
		command |= KeyData::RIGHT;
	}
	if (gp.IsLeftThumbStickLeft())
	{
		command |= KeyData::LEFT;
	}
	if (gp.IsLeftThumbStickUp())
	{
		command |= KeyData::UP;
	}
	if (gp.IsLeftThumbStickDown())
	{
		command |= KeyData::DOWN;
	}
	if (
		!gp.IsLeftThumbStickRight() &&
		!gp.IsLeftThumbStickLeft() &&
		!gp.IsLeftThumbStickUp() &&
		!gp.IsLeftThumbStickDown()
		)
	{
		command |= KeyData::NEUTRAL;
	}

	// ボタン入力
	if (m_tracker.b == GamePad::ButtonStateTracker::PRESSED)
	{
		command |= KeyData::CANCEL;
	}
	else if (m_tracker.a == GamePad::ButtonStateTracker::PRESSED)
	{
		command |= KeyData::DICISION;
	}

	if (m_tracker.x == GamePad::ButtonStateTracker::PRESSED)
	{
		command |= KeyData::WEAK;
	}
	if (m_tracker.y == GamePad::ButtonStateTracker::PRESSED)
	{
		command |= KeyData::MODERATO;
	}
	if (m_tracker.rightShoulder == GamePad::ButtonStateTracker::PRESSED)
	{
		command |= KeyData::HARD;
	}
	
	return command;
}
