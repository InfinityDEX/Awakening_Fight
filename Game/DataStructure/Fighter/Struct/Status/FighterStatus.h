#pragma once

#include <SimpleMath.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;

struct StatusData
{
	StatusData(
		Vector2 Stand,
		Vector2 Squat,
		Vector2 Aerial
	)
		: StandBodySize(Stand)
		, SquatBodySize(Squat)
		, AerialBodySize(Aerial)
	{}
	// —§‚Á‚Ä‚¢‚é‚Æ‚«‚Ì‘Ì‚Ì‘å‚«‚³
	const Vector2 StandBodySize;
	// À‚Á‚Ä‚¢‚é‚Æ‚«‚Ì‘Ì‚Ì‘å‚«‚³
	const Vector2 SquatBodySize;
	// ‹ó’†‚É‚¢‚é‚Æ‚«‚Ì‘Ì‚Ì‘å‚«‚³
	const Vector2 AerialBodySize;
};