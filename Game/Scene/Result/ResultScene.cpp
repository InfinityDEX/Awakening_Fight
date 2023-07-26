#include "pch.h"
#include <Game\Scene\Result\ResultScene.h>
#include <Game\3rdParty\Debug\Font\DebugFont.h>
#include <Game\3rdParty\Debug\GridFloor\GridFloor.h>
#include <Game\Locator\Locator.h>
#include <Game\Effect\PrimitiveEffect\Unlimited\UnlimitedEffect.h>
#include <Game\Scene\Title\TitleScene.h>
#include <WICTextureLoader.h>

// コンストラクタ
ResultScene::ResultScene(Judge judge)
{
	Locator::GetEffectManager()->Register(DrawingOrder::Front, new UnlimitedEffect(Vector3::One, Vector3::Zero, L"Resources\\Sprite\\Black.png"));
	switch (judge)
	{
	case ResultScene::P1:
		Locator::GetEffectManager()->Register(DrawingOrder::Front, new UnlimitedEffect(Vector3::One, Vector3::Zero, L"Resources\\Sprite\\Logo\\P1Win.png"));
		break;
	case ResultScene::P2:
		Locator::GetEffectManager()->Register(DrawingOrder::Front, new UnlimitedEffect(Vector3::One, Vector3::Zero, L"Resources\\Sprite\\Logo\\P2Win.png"));
		break;
	case ResultScene::Draw:
		Locator::GetEffectManager()->Register(DrawingOrder::Front, new UnlimitedEffect(Vector3::One, Vector3::Zero, L"Resources\\Sprite\\Logo\\	Draw.png"));
		break;
	default:
		break;
	}
}

// デストラクタ
ResultScene::~ResultScene()
{
	// リセット
	Locator::GetEffectManager()->Reset();
}

void ResultScene::initialize()
{
}

void ResultScene::Create()
{
	L"Resources\\Sprite\\Black.png";
}

void ResultScene::Update(DX::StepTimer const& timer)
{
	timer;
	int input = 0;

	if (Locator::GetControllerManager()->GetMomentInputData() & KeyData::ANY)
	{
		Locator::GetSceneManager()->RequestScene_Reset(std::make_unique<TitleScene>());
	}
}

void ResultScene::Render()
{
	Matrix ide = Matrix::Identity;
	Locator::GetEffectManager()->Render(DrawingOrder::Back, ide, ide, ide);
	Locator::GetEffectManager()->Render(DrawingOrder::Front, ide, ide, ide);
}

void ResultScene::Finalize()
{
}
