#pragma once
#include <Game\Scene\Interface\IScene.h>
#include <Game\Locator\Locator.h>
//#include <Game\Effect\MoveNode.h>
//#include <Game\Effect\GearCursor.h>
#include <GeometricPrimitive.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <memory>
#include <Audio.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class MenuScene :	public IScene
{
public:
	MenuScene();
	~MenuScene() override;

	// 初期化処理
	void initialize() override;

	// 生成処理
	virtual void Create() override;

	// 更新処理
	void Update(DX::StepTimer const& timer) override;

	// 裏更新処理
	void BackUpdate(DX::StepTimer const& timer) override;

	// 描画処理
	void Render() override;

	// 終了処理
	void Finalize() override;

private:
	enum Mode
	{
		Training,
		Battle,
		Option
	};

	const float NODE_MOVE_INTERVAL = 0.1f;

	float m_nowTime;

	int m_count;

	bool m_selectStart = false;

	bool m_sceneMove = false;

	bool m_returnTitle = false;

	Mode m_selectMode;
};

