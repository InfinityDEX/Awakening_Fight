#include "pch.h"
#include "ShockWaveEffect.h"


#include <Game\3rdParty\StepTimer.h>
#include <Game\3rdParty\BinaryFile.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

ShockWaveEffect::ShockWaveEffect(int life, Vector3 pos, Matrix rotato, wchar_t* path)
	:AnimationEffect(life, Vector2(7, 1), pos, rotato, path)
{}
