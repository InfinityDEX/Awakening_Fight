#include "pch.h"
#include <Game\Locator\Locator.h>
#include <Game\Scene\Title\TitleScene.h>
#include <Game\Scene\ControllerSelect\ControllerSelectScene.h>
#include <Game\Effect\PrimitiveEffect\Unlimited\UnlimitedEffect.h>

// コンストラクタ
TitleScene::TitleScene()
{
}

// デストラクタ
TitleScene::~TitleScene()
{
	// メインテーマを止める
	Locator::GetSoundManager()->LoopStop(200);
}

// 初期化処理
void TitleScene::initialize()
{
	// メインテーマを流す
	Locator::GetSoundManager()->Loop(200);
}

// 生成処理
void TitleScene::Create()
{
	auto effectManager = Locator::GetEffectManager();
	m_backColor = Vector4(1.0f, 0.0f, 0.1f, 0);
	m_backGrand = new TitleBackGrandEffect();
	effectManager->Register(DrawingOrder::Front, m_backGrand);
	effectManager->Register(DrawingOrder::Front, new UnlimitedEffect(Vector3(0.8f), Vector3::Zero, L"Resources\\Sprite\\Logo\\TitleLogo.png"));
	Vector3 nodeScale = Vector3::One;
}

// 更新処理
void TitleScene::Update(DX::StepTimer const& timer)
{
	// コントローラーのどれかが決定を押したら
	if (Locator::GetControllerManager()->GetInputData() & KeyData::ANY)
	{
		// 決定音を流す
		Locator::GetSoundManager()->OneShot(0);
		// コントローラ選択シーン呼び出し
		Locator::GetSceneManager()->RequestScene_Add(std::make_unique<ControllerSelectScene>());
	}
}

// バック更新処理
void TitleScene::BackUpdate(DX::StepTimer const & timer)
{
	auto effectManager = Locator::GetEffectManager();
	effectManager->Update(timer);
}

// 描画処理
void TitleScene::Render()
{
	auto objectManager = Locator::GetCollisionManager();
	auto effectManager = Locator::GetEffectManager();
	effectManager->Render(DrawingOrder::Front, Matrix::Identity, Matrix::Identity, Matrix::Identity);
}

// 終了処理
void TitleScene::Finalize()
{
	// エフェクトをリセットする
	Locator::GetEffectManager()->Reset();
}
