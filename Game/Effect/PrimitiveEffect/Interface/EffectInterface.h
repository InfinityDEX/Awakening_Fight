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
		// �R���X�g���N�^
		EffectInterface() = default;
		// ����������
		virtual void Initialize() = 0;
		// �X�V����
		virtual void Update(const DX::StepTimer& timer) = 0;
		// �`�揈��
		virtual void Render(Matrix world, Matrix view, Matrix proj) = 0;
		// �����������`
		virtual bool Death() = 0;
};