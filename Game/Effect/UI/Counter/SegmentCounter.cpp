#include "pch.h"
#include "SegmentCounter.h"
#include <Game\Locator\Locator.h>

// コンストラクタ
SegmentCounter::SegmentCounter()
{
	// カウンターの画像を取得
	wchar_t * path = L"Resources\\Sprite\\UI\\SegmentTimer\\Segment.png";
	// 大きさ
	float scale = 0.2f;
	// 座標
	Vector3 position = Vector3(0, 0.79f, 0);
	// 十の位のセグメントを作成
	m_10_Place = new Split(position + Vector3(-0.5f * scale, 0, 0), Vector3(0.5f, 1, 1) * scale, Vector2(4, 4), path);
	// 一の位のセグメントを作成
	m_1_Place = new Split(position + Vector3(0.5f * scale, 0, 0), Vector3(0.5f, 1, 1) * scale, Vector2(4, 4), path);
	// 時計版を作成
	m_clockBoard = new Split(position, Vector3::One * scale, Vector2(4, 4), path);
	// 時計版のスプライト座標を決める
	m_clockBoard->SplitPosition(Vector2(0, 3));
	// マネージャーを呼び出し
	auto manager = Locator::GetEffectManager();
	// マネージャーに先に描画されるものから順に登録
	manager->Register(DrawingOrder::Front, m_clockBoard);
	manager->Register(DrawingOrder::Front, m_10_Place);
	manager->Register(DrawingOrder::Front, m_1_Place);
}

// タイマーをセット
void SegmentCounter::SetTime(const int time)
{
	// 0以下は0、99以上は99
	int num = std::min(std::max(time, 0), 99);
	// 十の位
	int tenth = num / 10;
	// 一の位
	int first = num % 10;
	// 十の位をセット
	m_10_Place->SplitPosition(Vector2(tenth % 4, tenth / 4));
	// 一の位をセット
	m_1_Place->SplitPosition(Vector2(first % 4, first / 4));
}
