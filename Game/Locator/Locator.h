#pragma once
#include <Game\Manager\Object\ObjectManager.h>
#include <Game\Manager\Effect\EffectManager.h>
#include <Game\Manager\Scene\SceneManager.h>
#include <Game\Manager\Controller\ControllerManager.h>
#include <Game\Manager\Sound\SoundManager.h>
#include <Game\3rdParty\DeviceResources\DeviceResources.h>
#include <Audio.h>

class Locator
{
	public:
		/////////////////////////////////////
		//
		// 登録関数
		//
		/////////////////////////////////////

		// エフェクトマネージャーの登録
		static void SetEffectManager(EffectManager* service)
		{
 			if (_EffectManager == nullptr)
				delete _EffectManager;
			_EffectManager = service;
		}
		// モデルマネージャの登録
		static void SetSceneManager(SceneManager* service)
		{
			if (_SceneManager == nullptr)
				delete _SceneManager;
			_SceneManager = service;
		};
		// 衝突判定マネージャの登録
		static void SetCollisionManager(ObjectManager* service)
		{
			if (_CollisionManager == nullptr)
				delete _CollisionManager;
			_CollisionManager = service;
		};
		// コントローラマネージャの登録
		static void SetControllerManager(ControllerManager* service)
		{
			if (_ControllerManager == nullptr)
				delete _ControllerManager;
			_ControllerManager = service;
		};
		// サウンドマネージャーの登録
		static void SetSoundManager(SoundManager* service)
		{
			if (_SoundManager == nullptr)
				delete _SoundManager;
			_SoundManager = service;
		};
		// デバイスリソースの登録
		static void SetDeviceResources(DX::DeviceResources* service)
		{
			if (_DeviceResources == nullptr)
				delete _DeviceResources;
			_DeviceResources = service;
		};

		/////////////////////////////////////
		//
		// 呼び出し関数
		//
		/////////////////////////////////////
		
		//　エフェクトマネージャーの取得
		static EffectManager* const GetEffectManager()
		{
			assert(_EffectManager);
			return _EffectManager;
		};
		//　シーンマネージャの取得
		static SceneManager* const GetSceneManager()
		{
			assert(_SceneManager);
			return _SceneManager;
		};
		// モデルマネージャの取得
		static ObjectManager* const GetModelManager()
		{
			assert(_ModelManager);
			return _ModelManager;
		};
		//　衝突判定マネージャの取得
		static ObjectManager* const GetCollisionManager()
		{
			assert(_CollisionManager);
			return _CollisionManager;
		};
		// コントローラーマネージャの取得
		static ControllerManager* const GetControllerManager()
		{
			assert(_ControllerManager);
			return _ControllerManager;
		};
		// サウンドマネージャーの取得
		static SoundManager* const GetSoundManager()
		{
			assert(_SoundManager);
			return _SoundManager;
		};
		//　デバイスリソースの取得
		static DX::DeviceResources* const GetDeviceResources()
		{
			assert(_DeviceResources);
			return _DeviceResources;
		};

	private:
		// エフェクトの管理
		static EffectManager* _EffectManager;
		// シーンの管理
		static SceneManager* _SceneManager;
		// モデルの管理
		static ObjectManager* _ModelManager;
		// 衝突判定系の管理 
		static ObjectManager* _CollisionManager;
		// コントローラの管理
		static ControllerManager* _ControllerManager;
		// サウンドマネージャー
		static SoundManager* _SoundManager;
		// デバイスリソース
		static DX::DeviceResources* _DeviceResources;
	private:
		Locator() = default;
};