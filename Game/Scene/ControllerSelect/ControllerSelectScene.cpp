#include "pch.h"
#include <Game\Controller\GamePad\GamePadFighterController.h>
#include <Game\Controller\Keyboard\KeyboardFighterController.h>
#include <Game\Scene\Duel\DuelScene.h>
#include "ControllerSelectScene.h"
#include <WICTextureLoader.h>

// コンストラクタ
ControllerSelectScene::ControllerSelectScene()
{
	// -1はCPU
	m_p1 = m_p2 = -1;
}

// デストラクタ
ControllerSelectScene::~ControllerSelectScene()
{
	
}

// 初期化処理
void ControllerSelectScene::initialize()
{
	m_p1CIconPos = Vector3(-0.5f, -0.4f, 0);
	m_p2CIconPos = Vector3( 0.5f, -0.4f, 0);
}

// 生成処理
void ControllerSelectScene::Create()
{
	auto deviceResouces = Locator::GetDeviceResources();
	auto device = deviceResouces->GetD3DDevice();
	auto context = deviceResouces->GetD3DDeviceContext();
	auto effectManager = Locator::GetEffectManager();
	m_backGrand = new UnlimitedEffect(Vector3::One, Vector3::Zero, L"Resources\\Sprite\\Black.png");
	m_p1Icon = new UnlimitedEffect(Vector3(0.5f), Vector3(-0.5f, 0.4f, 0), L"Resources\\Sprite\\Logo\\Player1.png");
	m_p2Icon = new UnlimitedEffect(Vector3(0.5f), Vector3(0.5f, 0.4f, 0), L"Resources\\Sprite\\Logo\\Player2.png");
	m_p1ControllerIcon = new SwichableAnimation(m_p1CIconPos, Vector3(0.5f), "Resources\\Data\\UI\\ControllerSelect\\Controller.json");
	m_p2ControllerIcon = new SwichableAnimation(m_p2CIconPos, Vector3(0.5f),"Resources\\Data\\UI\\ControllerSelect\\Controller.json");
	effectManager->Register(DrawingOrder::Front, m_backGrand);
	effectManager->Register(DrawingOrder::Front, m_p1Icon);
	effectManager->Register(DrawingOrder::Front, m_p2Icon);
	effectManager->Register(DrawingOrder::Front, m_p1ControllerIcon);
	effectManager->Register(DrawingOrder::Front, m_p2ControllerIcon);
	m_p1ControllerIcon->SwitchAnimetion(1,-1);
	m_p2ControllerIcon->SwitchAnimetion(1,-1);
}

// 更新処理
void ControllerSelectScene::Update(DX::StepTimer const & timer)
{
	// キーボードの更新
	KeyboardUpdate();
	// ゲームパッド１の更新
	GamePad1Update();
	// ゲームパッド２の更新
	GamePad2Update();

	m_p1ControllerIcon->SwitchAnimetion(1, m_p1);
	m_p2ControllerIcon->SwitchAnimetion(1, m_p2);
}

// キーボードの更新
void ControllerSelectScene::KeyboardUpdate()
{
	// キーボードの識別番号
	int id = 2;
	// キーボードの取得
	CommandData Command = Locator::GetControllerManager()->CommandHistory(id);

	// 左ボタンを押していたら
	if (Command[0] & KeyData::LEFT)
	{
		// カーソル移動音
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == -1)
			m_p1 = id;
		if (m_p2 == id)
			m_p2 = -1;
	}

	// 右ボタンを押していたら
	if (Command[0] & KeyData::RIGHT)
	{
		// カーソル移動音
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == id)
			m_p1 = -1;
		if (m_p2 == -1)
			m_p2 = id;
	}

	// キャンセルボタンを押していたら
	if (Command[0] & KeyData::CANCEL)
	{
		// キャンセル音
		Locator::GetSoundManager()->OneShot(1);
		if (m_p1 == id)
			m_p1 = -1;
		else if (m_p2 == id)
			m_p2 = -1;
		else
			Locator::GetSceneManager()->PopScene(1);
	}

	// 決定ボタンを押していたら
	if (Command[0] & KeyData::DICISION)
	{
		// どちらかのコントローラに登録されていたら
		if (m_p1 == id || m_p2 == id)
		{
			// 決定音を再生
			Locator::GetSoundManager()->OneShot(0);
			// シーンを呼び出す
			Locator::GetSceneManager()->RequestScene_Reset(std::make_unique<DuelScene>(m_p1, m_p2));
		}
		// 押されていなかったら
		else
		{
			// カーソル移動音
			Locator::GetSoundManager()->OneShot(2);
			// 右側優先
			if (m_p1 == -1)
				m_p1 = id;
			else if (m_p2 == -1)
				m_p2 = id;
		}
	}
}

// ゲームパッド１の更新
void ControllerSelectScene::GamePad1Update()
{
	// ゲームパッド１の識別番号
	int id = 0;
	// ゲームパッド１の取得
	CommandData Command = Locator::GetControllerManager()->CommandHistory(id);

	// 左ボタンを押していたら
	if (Command[0] & KeyData::LEFT)
	{
		// カーソル移動音
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == -1)
			m_p1 = id;
		if (m_p2 == id)
			m_p2 = -1;
	}

	// 右ボタンを押していたら
	if (Command[0] & KeyData::RIGHT)
	{
		// カーソル移動音
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == id)
			m_p1 = -1;
		if (m_p2 == -1)
			m_p2 = id;
	}

	// キャンセルボタンを押していたら
	if (Command[0] & KeyData::CANCEL)
	{
		// キャンセル音
		Locator::GetSoundManager()->OneShot(1);
		if (m_p1 == id)
			m_p1 = -1;
		else if (m_p2 == id)
			m_p2 = -1;
		else
			Locator::GetSceneManager()->PopScene(1);
	}

	// 決定ボタンを押していたら
	if (Command[0] & KeyData::DICISION)
	{
		// どちらかのコントローラに登録されていたら
		if (m_p1 == id || m_p2 == id)
		{
			// 決定音を再生
			Locator::GetSoundManager()->OneShot(0);
			// シーンを呼び出す
			Locator::GetSceneManager()->RequestScene_Reset(std::make_unique<DuelScene>(m_p1, m_p2));
		}
		// 押されていなかったら
		else
		{
			// カーソル移動音
			Locator::GetSoundManager()->OneShot(2);
			// 右側優先
			if (m_p1 == -1)
				m_p1 = id;
			else if (m_p2 == -1)
				m_p2 = id;
		}
	}
}

// ゲームパッド２の更新
void ControllerSelectScene::GamePad2Update()
{
	// ゲームパッド２の識別番号
	int id = 1;
	// ゲームパッド２の取得
	CommandData Command = Locator::GetControllerManager()->CommandHistory(id);

	// 左ボタンを押していたら
	if (Command[0] & KeyData::LEFT)
	{
		// カーソル移動音
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == -1)
			m_p1 = id;
		if (m_p2 == id)
			m_p2 = -1;
	}

	// 右ボタンを押していたら
	if (Command[0] & KeyData::RIGHT)
	{
		// カーソル移動音
		Locator::GetSoundManager()->OneShot(2);
		if (m_p1 == id)
			m_p1 = -1;
		if (m_p2 = -1)
			m_p2 = id;
	}

	// キャンセルボタンを押していたら
	if (Command[0] & KeyData::CANCEL)
	{
		// キャンセル音
		Locator::GetSoundManager()->OneShot(1);
		if (m_p1 == id)
			m_p1 = -1;
		else if (m_p2 == id)
			m_p2 = -1;
		else
			Locator::GetSceneManager()->PopScene(1);
	}

	// 決定ボタンを押していたら
	if (Command[0] & KeyData::DICISION)
	{
		// どちらかのコントローラに登録されていたら
		if (m_p1 == id || m_p2 == id)
		{
			// 決定音を再生
			Locator::GetSoundManager()->OneShot(0);
			// シーンを呼び出す
			Locator::GetSceneManager()->RequestScene_Reset(std::make_unique<DuelScene>(m_p1, m_p2));
		}
		// 押されていなかったら
		else
		{
			// 右側優先
			if (m_p1 == -1)
				m_p1 = id;
			else if (m_p2 == -1)
				m_p2 = id;
		}
	}
}

// 描画処理
void ControllerSelectScene::Render()
{
	Matrix ide = Matrix::Identity;
	Locator::GetEffectManager()->Render(DrawingOrder::Back, ide, ide, ide);
	Locator::GetEffectManager()->Render(DrawingOrder::Front, ide, ide, ide);
}

// 終了処理
void ControllerSelectScene::Finalize()
{
	m_backGrand->Kill();
	m_p1Icon->Kill();
	m_p2Icon->Kill();
	m_p1ControllerIcon->Kill();
	m_p2ControllerIcon->Kill();
}