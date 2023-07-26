#include "pch.h"
#include "MenuScene.h"
#include <Game\3rdParty\Debug\Font\DebugFont.h>
#include <Game\3rdParty\Debug\GridFloor\GridFloor.h>
#include <Game\Locator\Locator.h>
#include <Game\Scene\ControllerSelect\ControllerSelectScene.h>

MenuScene::MenuScene()
{

}

MenuScene::~MenuScene()
{
}

void MenuScene::initialize()
{
	m_nowTime = 0;
	m_count = 0;
	m_selectMode = Mode::Battle;
}

void MenuScene::Create()
{
	auto deviceResources = Locator::GetDeviceResources();
	Vector3 nodeScale = Vector3(0.7f);


	//m_manager->Register(m_nodes[m_count++]);
	m_nowTime = 0;
}

void MenuScene::Update(DX::StepTimer const& timer)
{
	timer;

	// エフェクトマネージャー更新
	Locator::GetEffectManager()->Update(timer);

	if (m_returnTitle)
	{
		Locator::GetSceneManager()->PopScene(1);
		return;
	}

	// シーン遷移
	if (m_sceneMove)
	{
		switch (m_selectMode)
		{
		case MenuScene::Training:
			//Locator::GetSceneManager()->RequestScene_Reset("Training_Scene");
			break;
		case MenuScene::Battle:
			Locator::GetSceneManager()->RequestScene_Add(std::make_unique<ControllerSelectScene>());
			m_sceneMove = false;
			break;
		default:
			break;
		}
		return;
	}

	//auto kb = Keyboard::Get().GetState();
	//auto gp1 = GamePad::Get().GetState(0);
	//auto gp2 = GamePad::Get().GetState(1);
	//m_kbTracker.Update(kb);
	//m_gp1Tracker.Update(gp1);
	//m_gp2Tracker.Update(gp2);

	Locator::GetCollisionManager()->Update(timer);

	if (m_nowTime > NODE_MOVE_INTERVAL)
	{
		//m_manager->Register(m_nodes[m_count++]);
		m_nowTime = 0;
	}

	//// 全てのNodeの移動が完了したらセレクトできるようにする
	//for (auto Node : m_nodes)
	//{
	//	if (!Node->MoveEnd())
	//	{
	//		m_selectStart = false;
	//		break;
	//	}
	//	m_selectStart = true;
	//}

	//// カーソルを生成する
	//if(m_selectStart && !m_cursol)
	//{
	//	m_cursol = new GearCursor(&m_nodes[m_selectMode]->GetTransform()->position);
	//	m_cursol->Initialize();
	//	m_cursol->Create();
	//}

	// タイトルに戻る
	//if (m_kbTracker.pressed.Back)
	//{
	//	m_cancelSound->Play();
	//	m_returnTitle = true;
	//	for (auto Node : m_nodes)
	//	{
	//		Node->Route(MoveIcon::Back);
	//	}

	//	//Locator::GetSceneManager()->PopScene(1);
	//	return;
	//}

	//// モードを変更する
	//int tmp = m_selectMode;
	//if (m_selectStart && (m_kbTracker.pressed.Up || m_gp1Tracker.leftStickUp == GamePad::ButtonStateTracker::PRESSED || m_gp2Tracker.leftStickUp == GamePad::ButtonStateTracker::PRESSED))
	//{
	//	m_selectMode = static_cast<Mode>(std::max(m_selectMode - 1, 0));
	//	if(m_selectMode != tmp)m_selectSound->Play();
	//	else	m_selectErrorSound->Play();
	//}
	//if (m_count == m_nodes.size() && (m_kbTracker.pressed.Down || m_gp1Tracker.leftStickDown == GamePad::ButtonStateTracker::PRESSED || m_gp2Tracker.leftStickDown == GamePad::ButtonStateTracker::PRESSED))
	//{
	//	m_selectMode = static_cast<Mode>(std::min(m_selectMode + 1, static_cast<int>(Mode::Option)));
	//	if (m_selectMode != tmp)m_selectSound->Play();
	//	else	m_selectErrorSound->Play();
	//}

	//// カーソルをアップデートする
	//if (m_cursol)
	//{
	//	m_cursol->SetSelectedPostion(&m_nodes[static_cast<int>(m_selectMode)]->GetTransform()->position);
	//}

	//// モードを決定する
	//if (m_selectStart && (m_kbTracker.pressed.Space || m_gp1Tracker.a == GamePad::ButtonStateTracker::PRESSED || m_gp2Tracker.a == GamePad::ButtonStateTracker::PRESSED))
	//{
	//	if (m_selectMode == MenuScene::Battle)
	//	{
	//		m_decisionSound->Play();
	//		m_sceneMove = true;
	//	}
	//	else
	//	{
	//		m_selectErrorSound->Play();
	//	}

	//	//if (m_selectMode == MenuScene::Option)
	//	//{
	//	//	Locator::GetSceneManager()->RequestScene_Add("Option_Scene");
	//	//	m_sceneMove = false;
	//	//}
	//}
	////if (m_count < m_nodes.size())
	////{
	////	m_nowTime += timer.GetElapsedSeconds();
	////}

	//// デバックモードを切り替える
	//if (m_kbTracker.pressed.B == true)
	//{
	//	m_debugTrigger != m_debugTrigger;
	//}
}

void MenuScene::BackUpdate(DX::StepTimer const & timer)
{
	//if(m_cursol) m_cursol->Update(timer);
}

void MenuScene::Render()
{
	RECT size = Locator::GetDeviceResources()->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);
	float forAngleY = XMConvertToRadians(90.0f);
	Matrix view = Matrix::CreateLookAt(Vector3(0,2,5), Vector3(0, 2, 0), Vector3::UnitY);
	Matrix proj = Matrix::CreatePerspectiveFieldOfView(
		forAngleY,
		aspectRatio,
		0.01f,
		50.0f
	);
	
	// エフェクトマネージャー更新
	//m_effectManager->Render(DrawingOrder::Front);

	Locator::GetCollisionManager()->Render(view, proj);
	//if (m_cursol)
	//{
	//	m_cursol->Render(view, proj);
	//}
	if (!m_debugTrigger) return;

	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->print(10, 10, L"MenuState");
	debugFont->draw();
	debugFont->print(10, 30, L"Now Select Mode : %d", m_selectMode);
	debugFont->draw();
}

void MenuScene::Finalize()
{
}
