#pragma once
#ifndef ISCENE
#define  ISCENE

#include <Game\3rdParty\Timer\StepTimer.h>
#include <Keyboard.h>
#include <GamePad.h>

class IScene
{
public:
	// コンストラクタ
	IScene() = default;
	// デストラクタ
	virtual ~IScene() = default;

	// 初期化処理
	virtual void initialize() = 0;

	// 生成処理
	virtual void Create() = 0;

	// 更新処理
	virtual void Update(DX::StepTimer const& timer) = 0;

	// 裏更新処理
	virtual void BackUpdate(DX::StepTimer const& timer) {};

	// 描画処理
	virtual void Render() = 0;
	
	// 終了処理
	virtual void Finalize() = 0;


protected:
	// デバック表示トリガー
	static bool m_debugTrigger;
};
#endif

