#pragma once
#include <SimpleMath.h>
#include <Game\Effect\PrimitiveEffect\Split\Split.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class SegmentCounter
{
	public:
		// コンストラクタ
		SegmentCounter();
		// カウンターの値をセット
		void SetTime(const int time);
	private:
		// 十の位
		Split* m_10_Place;
		// 一の位
		Split* m_1_Place;
		// 時計版
		Split* m_clockBoard;
};