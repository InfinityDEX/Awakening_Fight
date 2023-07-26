#include "pch.h"
#include <Game\Scene\Duel\DuelScene.h>
#include <Game\3rdParty\Debug\Font\DebugFont.h>
#include <Game\3rdParty\Debug\GridFloor\GridFloor.h>
#include <Game\Locator\Locator.h>
#include <Game\GameObject\Fighter\Strategy\Brave\Brave.h>
#include <Game\Scene\Result\ResultScene.h>
#include <Game\Effect\EffectObject\Stage\Stage1_Subway\Subway.h>
#include "WICTextureLoader.h"

const float DuelScene::READY_END_MOVE_NODE	= 2.0f;
const float DuelScene::START_END_MOVE_NODE	= 0.8f;
const float DuelScene::JUDGE_END_MOVE_NODE	= 1.4f;
const float DuelScene::READY_STOP_INTERVAL	= 0.5f;
const float DuelScene::START_STOP_INTERVAL	= 0.3f;
const float DuelScene::JUDGE_STOP_INTERVAL	= 1.0f;
const float DuelScene::RESULT_INTERVAL		= 2.5f;
const float DuelScene::CAMERA_STOP_INTERVAL = 1.0f;
const float DuelScene::CANGE_CAMERA_NUM     = 3;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                        
// public                                                                                                                 
//                                                                                                                        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// コンストラクタ
DuelScene::DuelScene(int p1, int p2)
	: IScene()
	, P1CNumber(p1)
	, P2CNumber(p2)
{
	m_fastRender = 1;
}

// デストラクタ
DuelScene::~DuelScene()
{
}

// 初期化処理
void DuelScene::initialize()
{
	m_intervalTime = 0;
	m_limitTime = 60;
	m_cameraNum = 0;
	// 最初の状態を決める
	m_nowState = &DuelScene::StartState;
	// カメラのX座標の初期値を設定
	m_nowCameraXposition = (P1->GetTransform()->position.x + P2->GetTransform()->position.x) / 2;
	// カメラの近さの初期値を設定
	m_lastDepth = abs(P1->GetTransform()->position.x - P2->GetTransform()->position.x);
}

// 生成処理
void DuelScene::Create()
{
	// エフェクトマネージャー取得
	auto manager = Locator::GetEffectManager();
	// ステージの作成
	m_stage = std::make_unique<Subway>();
	// 時計を生成
	m_segment = std::make_unique<SegmentCounter>();
	// プレイヤー１のHPバー作成
	m_p1HP = new RectangleGage(
		Vector3(-0.55f, 0.9f, 0),
		Vector3(0.4f, 0.2f, 0.2f),
		L"Resources\\Sprite\\UI\\HPGage\\Body.png",
		L"Resources\\Sprite\\UI\\HPGage\\gage.png",
		L"Resources\\Sprite\\UI\\HPGage\\GageMask.png"
	);
	// プレイヤー２のHPバー作成
	m_p2HP = new RectangleGage(
		Vector3(0.55f, 0.9f, 0),
		Vector3(-0.4f, 0.2f, 0.2f),
		L"Resources\\Sprite\\UI\\HPGage\\Body.png",
		L"Resources\\Sprite\\UI\\HPGage\\gage.png",
		L"Resources\\Sprite\\UI\\HPGage\\GageMask.png"
	);
	// プレイヤー２のHPバー作成
	manager->Register(DrawingOrder::Front, m_p1HP);
	manager->Register(DrawingOrder::Front, m_p2HP);
	// プレイヤーを設定する
	P1 = new FighterFrame(new Brave(), &P2, P1CNumber, 1, Vector3(-3, 0, 0));
	P2 = new FighterFrame(new Brave(), &P1, P2CNumber, 2, Vector3(3, 0, 0));
	// オブジェクト生成
	P1->Create();
	P2->Create();
	// 初期化
	P1->Initialize();
	P2->Initialize();
	// 開始時に出る「Ready?」のエフェクトを作成
	EffectInterface* effect = new FadeEffect(Vector3::Zero, Vector3(1, 0.5f, 1), 5, 5, 30, L"Resources\\Sprite\\EffectParts\\Announce\\AYR.png");
	manager->Register(DrawingOrder::Front, effect);
}

// 更新処理
void DuelScene::Update(const DX::StepTimer& timer)
{
	// タイマー
	timer;
	// 状態遷移
	(this->*m_nowState)(timer);
	// ステージの更新
	StageUpdate(timer);
	// 操作キャラクターの更新
	FightersUpdate(timer);
	// マネージャの更新
	ManagersUpdate(timer);
	// 現在の時間を時計に送る
	CountDownClock(timer);
}

// 描画処理
void DuelScene::Render()
{
	RECT size = Locator::GetDeviceResources()->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);
	float forAngleY = XMConvertToRadians(45.0f);
	float cameraXpos = (P1->GetTransform()->position.x + P2->GetTransform()->position.x) / 2;
	m_nowCameraXposition += (cameraXpos - m_nowCameraXposition) * 0.1f;
	float depth = 6 * (abs(P1->GetTransform()->position.x - P2->GetTransform()->position.x) / FighterFrame::PLAYER_INTERVAL_LIMIT);
	m_nowCameraXposition = std::fmaxf(std::fminf(m_nowCameraXposition, FighterFrame::PLAYER_INTERVAL_LIMIT / 2), -FighterFrame::PLAYER_INTERVAL_LIMIT / 2);
	depth = std::fmaxf(std::fminf(depth, 6.6), 5.5f);
	m_lastDepth = m_lastDepth + 0.01f * (depth - m_lastDepth);
	float height = 2;
	Matrix view = Matrix::CreateLookAt(Vector3(m_nowCameraXposition, height, m_lastDepth), Vector3(m_nowCameraXposition, height, 0), Vector3::UnitY);
	Matrix proj = Matrix::CreatePerspectiveFieldOfView(
		forAngleY,
		aspectRatio,
		0.01f,
		1000.0f
	);
	// エフェクトマネージャーの更新
	Locator::GetEffectManager()->Render(DrawingOrder::Back, Matrix::Identity, view, proj);
	// ファイターの描画
	switch (m_fastRender)
	{
	case 1:
		P1->Render(view, proj);
		P2->Render(view, proj);
		break;
	case 2:
		P2->Render(view, proj);
		P1->Render(view, proj);
		break;
	default:
		break;
	}
	// エフェクトマネージャーの更新
	Locator::GetEffectManager()->Render(DrawingOrder::Front, Matrix::Identity, view, proj);
}

// 終了処理
void DuelScene::Finalize()
{}

// 一番最初に呼び出す
void DuelScene::ReadyState(const DX::StepTimer& timer)
{
	if (m_intervalTime >= READY_END_MOVE_NODE)
	{
		m_intervalTime = 0;
		// システムエフェクトを作成
		EffectInterface* effect = new FadeEffect(Vector3::Zero, Vector3(1, 1, 1), 5, 10, 60, L"Resources\\Sprite\\EffectParts\\Announce\\Fight.png");
		Locator::GetEffectManager()->Register(DrawingOrder::Front, effect);
		m_nowState = &DuelScene::StartState;
		return;
	}
	m_intervalTime += timer.GetElapsedSeconds();
	FighterFrame::ControlLock();
}

// ラウンド？コール
void DuelScene::StartState(const DX::StepTimer& timer)
{
	if (m_intervalTime >= START_END_MOVE_NODE)
	{
		m_intervalTime = 0;
		m_nowState = &DuelScene::FightingState;
		Locator::GetSoundManager()->Volume(5, 10);
		Locator::GetSoundManager()->OneShot(5);
		return;
	}
	m_intervalTime += timer.GetElapsedSeconds();
	FighterFrame::ControlLock();
}

// ファイタ操作ができる状態
void DuelScene::FightingState(const DX::StepTimer& timer)
{		
	// 格ゲーはゲームスピードの影響で時間が早く感じるので時間経過をゆっくりにする
	m_limitTime -= timer.GetElapsedSeconds() * 0.7f;
	m_limitTime = std::fmax(m_limitTime, 0);
	if (m_limitTime == 0)
	{
		// タイムアップ演出の呼び出し
		m_nowState = &DuelScene::TimeUpState;
		// タイムアップのエフェクトを作成
		EffectInterface* effect = new FadeEffect(Vector3::Zero, Vector3(1, 0.5f, 1), 10, 10, 60, L"Resources\\Sprite\\EffectParts\\Announce\\TimeUp.png");
		Locator::GetEffectManager()->Register(DrawingOrder::Front, effect);
	}
	if (P1->KO() || P2->KO())
	{
		// 決着演出の呼び出し
		m_nowState = &DuelScene::DramaticState;
		// KOのエフェクトを作成
		EffectInterface* effect = new FadeEffect(Vector3::Zero, Vector3(1, 0.5f, 1), 10, 10, 60, L"Resources\\Sprite\\EffectParts\\Announce\\KO.png");
		Locator::GetEffectManager()->Register(DrawingOrder::Front, effect);
	}
	FighterFrame::ControlOpen();
}

// 時間切れになったら呼び出す
void DuelScene::TimeUpState(const DX::StepTimer& timer)
{
	m_intervalTime += timer.GetElapsedSeconds();
	if (m_intervalTime >= JUDGE_END_MOVE_NODE)
	{
		m_nowState = &DuelScene::BeforeResultState;
	}
	FighterFrame::ControlLock();
}

// どっちかがKOした時に呼び出す
void DuelScene::DramaticState(DX::StepTimer const & timer)
{
	FighterFrame::ControlLock();
	m_dramaticTrigger = true;
	m_intervalTime += timer.GetElapsedSeconds();
	if (m_intervalTime >= CAMERA_STOP_INTERVAL)
	{
		m_cameraNum++;
		m_intervalTime = 0;
	}
	if (m_cameraNum >= CANGE_CAMERA_NUM)
	{
		m_cameraNum = 0;
		m_intervalTime = 0;
		m_nowState = &DuelScene::KOState;
	}
}

// ↑の演出が終わったら呼び出す
void DuelScene::KOState(const DX::StepTimer& timer)
{
	FighterFrame::ControlLock();
	m_intervalTime += timer.GetElapsedSeconds();
	if (m_intervalTime >= JUDGE_END_MOVE_NODE)
	{
		m_intervalTime = 0;
		m_nowState = &DuelScene::BeforeResultState;
	}
}

// リザルト前に呼び出す
void DuelScene::BeforeResultState(DX::StepTimer const & timer)
{
	FightersUpdate(timer);
	m_intervalTime += timer.GetElapsedSeconds();
	
	if (m_intervalTime >= RESULT_INTERVAL)
	{
		Locator::GetCollisionManager()->Finalize();

		ResultScene::Judge jugge;
		if (P1->KO() || P1->HPReference() < P2->HPReference())
		{
			jugge = ResultScene::Judge::P2;
		}
		if (P2->KO() || P1->HPReference() > P2->HPReference())
		{
			jugge = ResultScene::Judge::P1;
		}
		if ((P1->KO() && P2->KO()) || P1->HPReference() == P2->HPReference())
		{
			jugge = ResultScene::Judge::Draw;
		}
		Locator::GetSceneManager()->RequestScene_Add(std::make_unique<ResultScene>(jugge));
	}
	FighterFrame::ControlLock();
}

// ステージの更新
void DuelScene::StageUpdate(DX::StepTimer const & timer)
{
	// ステージの更新
	m_stage->Update(timer);
}

void DuelScene::FightersUpdate(DX::StepTimer const & timer)
{
	// HPバーを管理する
	m_p1HP->Amount(static_cast<float>(P1->HPReference()) / static_cast<float>(P1->MAXHP));
	m_p2HP->Amount(static_cast<float>(P2->HPReference()) / static_cast<float>(P2->MAXHP));


	if (m_nowState != &DuelScene::DramaticState)
	{
		// ファイターの更新
		P1->Update(timer);
		P2->Update(timer);
	}
}

void DuelScene::ManagersUpdate(DX::StepTimer const & timer)
{
	// コライダーマネージャーの更新
	Locator::GetCollisionManager()->Update(timer);
}

void DuelScene::CountDownClock(DX::StepTimer const & timer)
{
	// 時間が完全に「0」になったタイミングでカウンターを「00」にしたいので少し細工する
	if (m_limitTime == int(m_limitTime))
	{
		// ぴったりだったらそのまま
		m_segment->SetTime(m_limitTime);
	}
	else
	{
		// 小数点単位で余剰があれば１増やす
		m_segment->SetTime(m_limitTime + 1);
	}
}
