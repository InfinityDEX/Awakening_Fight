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
	// �����Ă���Ƃ��̑̂̑傫��
	const Vector2 StandBodySize;
	// �����Ă���Ƃ��̑̂̑傫��
	const Vector2 SquatBodySize;
	// �󒆂ɂ���Ƃ��̑̂̑傫��
	const Vector2 AerialBodySize;
};