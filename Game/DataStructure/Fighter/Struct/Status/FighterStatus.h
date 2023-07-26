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
	// 立っているときの体の大きさ
	const Vector2 StandBodySize;
	// 座っているときの体の大きさ
	const Vector2 SquatBodySize;
	// 空中にいるときの体の大きさ
	const Vector2 AerialBodySize;
};