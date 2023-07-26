#pragma once
#include <Game\Scene\Interface\IScene.h>
#include <Game\Effect\PrimitiveEffect\Wave\TitleBackGrandEffect.h>
#include <GeometricPrimitive.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <memory>
#include <Audio.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class TitleScene :	public IScene
{
	public:
		TitleScene();
		~TitleScene() override;

		// 初期化処理
		void initialize() override;

		// 生成処理
		virtual void Create() override;

		// 更新処理
		void Update(DX::StepTimer const& timer) override;

		// 裏更新
		void BackUpdate(DX::StepTimer const& timer) override;

		// 描画処理
		void Render() override;

		// 終了処理
		void Finalize() override;
	private:
		TitleBackGrandEffect* m_backGrand;
		Vector4 m_backColor;
};

