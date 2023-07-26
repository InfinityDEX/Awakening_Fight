#include "pch.h"
#include "KeyboardFighterController.h"

// キーボードの生成
Keyboard KeyboardFighterController::KEYBOARD;

// コンストラクタ
KeyboardFighterController::KeyboardFighterController()
{}

// デストラクタ
KeyboardFighterController::~KeyboardFighterController()
{
}

// 入力データの取得
int KeyboardFighterController::GetInput()
{
	auto kb = Keyboard::Get().GetState();
	m_tracker.Update(kb);

	// 初期化
	int command = 0;

	// 方向キー入力
	if (kb.IsKeyDown(Keyboard::Keys::Right))
	{
		command |= KeyData::RIGHT;
	}
	else if (kb.IsKeyDown(Keyboard::Keys::Left))
	{
		command |= KeyData::LEFT;
	}
	if (kb.IsKeyDown(Keyboard::Keys::Up))
	{
		command |= KeyData::UP;
	}
	else if (kb.IsKeyDown(Keyboard::Keys::Down))
	{
		command |= KeyData::DOWN;
	}
	if (
		!kb.IsKeyDown(Keyboard::Keys::Right) &&
		!kb.IsKeyDown(Keyboard::Keys::Left) &&
		!kb.IsKeyDown(Keyboard::Keys::Up) &&
		!kb.IsKeyDown(Keyboard::Keys::Down) 
		)
	{
		command |= KeyData::NEUTRAL;
	}

	/// システム用
	if (m_tracker.pressed.Back) // キャンセルが優先
	{
		// キャンセル
		command |= KeyData::CANCEL;
	}
	else if (m_tracker.pressed.Space)
	{
		// 決定
		command |= KeyData::DICISION;
	}

	if (m_tracker.pressed.Escape)
	{
		// プログラム終了
		command |= KeyData::ESCAPE;
	}

	// ボタン入力
	if (m_tracker.IsKeyPressed(Keyboard::Keys::A))
	{
		command |= KeyData::WEAK;
	}
	if (m_tracker.IsKeyPressed(Keyboard::Keys::S))
	{
		command |= KeyData::MODERATO;
	}
	if (m_tracker.IsKeyPressed(Keyboard::Keys::D))
	{
		command |= KeyData::HARD;
	}

	return command;
}
