#include "pch.h"
#include "SlashEffect.h"


#include <Game\3rdParty\StepTimer.h>
#include <Game\3rdParty\BinaryFile.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

SlashEffect::SlashEffect(float life, Vector3 pos, Vector3 vel, Matrix rotato,  wchar_t* path)
	:AnimationEffect(life, Vector2(9, 1), pos , rotato, path)
{
	GetScale() = Vector3(2);
	m_velocity = vel;
}
