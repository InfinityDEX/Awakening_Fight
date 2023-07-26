#pragma once
#include <Game\3rdParty\Timer\StepTimer.h>

class IStage
{
	public:
		// ����������
		virtual void Initialize() = 0;
		// �X�V����
		virtual void Update(const DX::StepTimer& timer) = 0;
};