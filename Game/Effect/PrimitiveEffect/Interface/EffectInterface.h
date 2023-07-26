#pragma once

#include <Game\3rdParty\Timer\StepTimer.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class EffectInterface 
{
	public:
		// コンストラクタ
		EffectInterface() = default;
		// 初期化処理
		virtual void Initialize() = 0;
		// 更新処理
		virtual void Update(const DX::StepTimer& timer) = 0;
		// 描画処理
		virtual void Render(Matrix world, Matrix view, Matrix proj) = 0;
		// 消える条件定義
		virtual bool Death() = 0;
};