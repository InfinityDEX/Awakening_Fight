#pragma once
#include <Game\3rdParty\Timer\StepTimer.h>

class IStage
{
	public:
		// ‰Šú‰»ˆ—
		virtual void Initialize() = 0;
		// XVˆ—
		virtual void Update(const DX::StepTimer& timer) = 0;
};