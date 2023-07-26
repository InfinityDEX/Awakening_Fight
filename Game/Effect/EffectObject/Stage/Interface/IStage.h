#pragma once
#include <Game\3rdParty\Timer\StepTimer.h>

class IStage
{
	public:
		// 初期化処理
		virtual void Initialize() = 0;
		// 更新処理
		virtual void Update(const DX::StepTimer& timer) = 0;
};