//
// Game.h
//

#pragma once
#include <memory>
#include <Game\3rdParty\Timer\StepTimer.h>
#include <Game\Locator\Locator.h>
#include <Audio.h>

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);

	~Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

	// Finalize
	void Finalize();
private:
    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources> m_deviceResources;
	// サウンドマネージャー
	std::unique_ptr<SoundManager> m_soundManager;
	// シーンマネージャー
	std::unique_ptr<SceneManager> m_sceneManager;
	// コントローラマネージャ
	std::unique_ptr<ControllerManager> m_controllerManager;
	// 当たり判定マネージャ
	std::unique_ptr<ObjectManager> m_collisionManager;
	// エフェクトマネージャー
	std::unique_ptr<EffectManager> m_effectManager;
    // Rendering loop timer.
    DX::StepTimer m_timer;
};