#include "pch.h"
#include <Game\Locator\Locator.h>
// エフェクトの管理
EffectManager*  Locator::_EffectManager = nullptr;
// シーンの管理
SceneManager* Locator::_SceneManager = nullptr;
// 衝突判定の管理 
ObjectManager* Locator::_CollisionManager = nullptr;
// コントローラの管理
ControllerManager* Locator::_ControllerManager = nullptr;
// サウンドマネージャー 
SoundManager* Locator::_SoundManager = nullptr;
// デバイスリソース 
DX::DeviceResources* Locator::_DeviceResources = nullptr;