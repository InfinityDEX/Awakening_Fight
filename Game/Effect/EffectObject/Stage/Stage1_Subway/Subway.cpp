#include "pch.h"
#include "Subway.h"
#include <Game\Locator\Locator.h>

// コンストラクタ
Subway::Subway()
{
	// エフェクトマネージャー取得
	auto effectManager = Locator::GetEffectManager();

	// 基本のスケール
	Vector3 scale = Vector3(7, 4, 0);
	// 基本の座標
	Vector3 pos = Vector3(0, 3, 0);
	m_fTrainStartPos = Vector3(-130, 5, -1);
	m_bTrainStartPos = Vector3(100, 5, -3);
	m_bTrainSpeed = -0.5f;
	m_fTrainSpeed = 0.5f;

	// プラットフォーム作成
	m_platform = new UnlimitedEffect(scale, pos - Vector3::UnitZ * 0, L"Resources\\Sprite\\Stages\\Subway\\Platform.png");
	// 仕切り壁作成
	m_partition = new UnlimitedEffect(scale, pos - Vector3::UnitZ * 2, L"Resources\\Sprite\\Stages\\Subway\\Partition.png");
	// 向こう側のプラットフォーム作成
	m_oppositePlatform = new UnlimitedEffect(scale * 1.1f, pos - Vector3::UnitZ * 4, L"Resources\\Sprite\\Stages\\Subway\\OppositePlatform.png");
	// 電車作成
	m_frontTrain = new UnlimitedEffect(scale * 2, m_fTrainStartPos, L"Resources\\Sprite\\Stages\\Subway\\Train.png");
	m_backTrain = new UnlimitedEffect(scale * 2, m_bTrainStartPos, L"Resources\\Sprite\\Stages\\Subway\\Train.png");

	// マネージャに登録
	effectManager->Register(DrawingOrder::Back, m_oppositePlatform);
	effectManager->Register(DrawingOrder::Back, m_backTrain);
	effectManager->Register(DrawingOrder::Back, m_partition);
	effectManager->Register(DrawingOrder::Back, m_frontTrain);
	effectManager->Register(DrawingOrder::Back, m_platform);

	// ステージテーマの再生
	Locator::GetSoundManager()->Loop(300);
	Locator::GetSoundManager()->Volume(300, 0.5f);
	Locator::GetSoundManager()->Loop(301);
	Locator::GetSoundManager()->Volume(301, 1);
}

// デストラクタ
Subway::~Subway()
{
	// ステージ音声を止める
	Locator::GetSoundManager()->LoopStop(300);
	Locator::GetSoundManager()->LoopStop(301);
}

// 更新処理
void Subway::Update(const DX::StepTimer & timer)
{
	// 向こう側の電車がスタート地点の対極の位置に達したら
	if (
		((m_bTrainSpeed / abs(m_bTrainSpeed)) == 1 && m_backTrain->Position().x >= -m_bTrainStartPos.x) ||
		((m_bTrainSpeed / abs(m_bTrainSpeed)) == -1 && m_backTrain->Position().x <= -m_bTrainStartPos.x)
		)
	{
		// スタート地点にワープ
		m_backTrain->Teleport(m_bTrainStartPos);
	}
	// 手前の電車がスタート地点の対極の位置に達したらたら
	if (
		((m_fTrainSpeed / abs(m_fTrainSpeed)) == 1 && m_frontTrain->Position().x >= -m_fTrainStartPos.x) ||
		((m_fTrainSpeed / abs(m_fTrainSpeed)) == -1 && m_frontTrain->Position().x <= -m_fTrainStartPos.x)
		)
	{
		// スタート地点にワープ
		m_frontTrain->Teleport(m_fTrainStartPos);
	}

	// 電車の移動
	m_backTrain->Movement(Vector3(m_bTrainSpeed, 0, 0));
	m_frontTrain->Movement(Vector3(m_fTrainSpeed, 0, 0));
}
