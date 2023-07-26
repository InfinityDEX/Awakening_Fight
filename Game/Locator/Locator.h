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
		// �o�^�֐�
		//
		/////////////////////////////////////

		// �G�t�F�N�g�}�l�[�W���[�̓o�^
		static void SetEffectManager(EffectManager* service)
		{
 			if (_EffectManager == nullptr)
				delete _EffectManager;
			_EffectManager = service;
		}
		// ���f���}�l�[�W���̓o�^
		static void SetSceneManager(SceneManager* service)
		{
			if (_SceneManager == nullptr)
				delete _SceneManager;
			_SceneManager = service;
		};
		// �Փ˔���}�l�[�W���̓o�^
		static void SetCollisionManager(ObjectManager* service)
		{
			if (_CollisionManager == nullptr)
				delete _CollisionManager;
			_CollisionManager = service;
		};
		// �R���g���[���}�l�[�W���̓o�^
		static void SetControllerManager(ControllerManager* service)
		{
			if (_ControllerManager == nullptr)
				delete _ControllerManager;
			_ControllerManager = service;
		};
		// �T�E���h�}�l�[�W���[�̓o�^
		static void SetSoundManager(SoundManager* service)
		{
			if (_SoundManager == nullptr)
				delete _SoundManager;
			_SoundManager = service;
		};
		// �f�o�C�X���\�[�X�̓o�^
		static void SetDeviceResources(DX::DeviceResources* service)
		{
			if (_DeviceResources == nullptr)
				delete _DeviceResources;
			_DeviceResources = service;
		};

		/////////////////////////////////////
		//
		// �Ăяo���֐�
		//
		/////////////////////////////////////
		
		//�@�G�t�F�N�g�}�l�[�W���[�̎擾
		static EffectManager* const GetEffectManager()
		{
			assert(_EffectManager);
			return _EffectManager;
		};
		//�@�V�[���}�l�[�W���̎擾
		static SceneManager* const GetSceneManager()
		{
			assert(_SceneManager);
			return _SceneManager;
		};
		// ���f���}�l�[�W���̎擾
		static ObjectManager* const GetModelManager()
		{
			assert(_ModelManager);
			return _ModelManager;
		};
		//�@�Փ˔���}�l�[�W���̎擾
		static ObjectManager* const GetCollisionManager()
		{
			assert(_CollisionManager);
			return _CollisionManager;
		};
		// �R���g���[���[�}�l�[�W���̎擾
		static ControllerManager* const GetControllerManager()
		{
			assert(_ControllerManager);
			return _ControllerManager;
		};
		// �T�E���h�}�l�[�W���[�̎擾
		static SoundManager* const GetSoundManager()
		{
			assert(_SoundManager);
			return _SoundManager;
		};
		//�@�f�o�C�X���\�[�X�̎擾
		static DX::DeviceResources* const GetDeviceResources()
		{
			assert(_DeviceResources);
			return _DeviceResources;
		};

	private:
		// �G�t�F�N�g�̊Ǘ�
		static EffectManager* _EffectManager;
		// �V�[���̊Ǘ�
		static SceneManager* _SceneManager;
		// ���f���̊Ǘ�
		static ObjectManager* _ModelManager;
		// �Փ˔���n�̊Ǘ� 
		static ObjectManager* _CollisionManager;
		// �R���g���[���̊Ǘ�
		static ControllerManager* _ControllerManager;
		// �T�E���h�}�l�[�W���[
		static SoundManager* _SoundManager;
		// �f�o�C�X���\�[�X
		static DX::DeviceResources* _DeviceResources;
	private:
		Locator() = default;
};