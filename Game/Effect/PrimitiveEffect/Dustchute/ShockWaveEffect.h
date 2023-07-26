#pragma once


#include <Game\3rdParty\StepTimer.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "AnimationEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
class ShockWaveEffect : public AnimationEffect
{
public:
	ShockWaveEffect(int life, Vector3 pos, Matrix rotato, wchar_t* path);
};