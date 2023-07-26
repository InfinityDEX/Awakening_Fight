//
// Game.cpp
//

#include <pch.h>
#include "Game.h"
#include <Game\Scene\Title\TitleScene.h>
#include <Game\3rdParty\Debug\Font\DebugFont.h>
#include <Game\DataStructure\Sound\SoundDataLoader.h>
#include <Game\Controller\GamePad\GamePadFighterController.h>
#include <Game\Controller\Keyboard\KeyboardFighterController.h>
#include <Game\Locator\Locator.h>

extern void ExitGame();

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
	Finalize();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	// コントローラの更新
	Locator::GetControllerManager()->Update();

	// シーンの更新
	m_sceneManager->Update(timer);

	// エフェクトマネージャーの更新
	Locator::GetEffectManager()->Update(timer);

	// エスケープボタンが押されたら
	if (Locator::GetControllerManager()->GetMomentInputData() & KeyData::ESCAPE)
	{
		ExitGame();
	}
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

	// シーンマネージャーの描画
	m_sceneManager->Render();

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1200;
    height = 900;
}
void Game::Finalize()
{
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;
	context;

	/// サービス作成
	// サウンドマネージャーを作成
	m_soundManager = std::make_unique<SoundManager>();
	// シーンマネージャを作成
	m_sceneManager = std::make_unique<SceneManager>();
	// オープニングシーンの登録
	m_sceneManager->RequestScene_Add(std::make_unique<TitleScene>());
	// 当たり判定マネージャを作成
	m_collisionManager = std::make_unique<ObjectManager>();
	// エフェクトマネージャを作成
	m_effectManager = std::make_unique<EffectManager>();
	// コントローラマネージャを作成
	m_controllerManager = std::make_unique<ControllerManager>();
	/// ロケーターに登録
	Locator::SetSoundManager(m_soundManager.get());
	Locator::SetCollisionManager(m_collisionManager.get());
	Locator::SetSceneManager(m_sceneManager.get());
	Locator::SetEffectManager(m_effectManager.get());
	Locator::SetControllerManager(m_controllerManager.get());
	Locator::SetDeviceResources(m_deviceResources.get());

	// コントローラを登録
	m_controllerManager->Register(0, std::make_unique<GamePadFighterController>(0)); // ゲームパッド１
	m_controllerManager->Register(1, std::make_unique<GamePadFighterController>(1)); // ゲームパッド２
	m_controllerManager->Register(2, std::make_unique<KeyboardFighterController>()); // キーボード

	// 音データを登録
	std::vector<SoundData> list = CreatSoundDataList("Resources\\Data\\SoundData\\SoundPathList.json");
	for (auto data : list)
	{
		// マネージャーにデータを登録
		m_soundManager->Register(data.ID, data.path);
	}
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
