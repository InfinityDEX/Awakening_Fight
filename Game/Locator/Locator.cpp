#include "pch.h"
#include <Game\Locator\Locator.h>
// �G�t�F�N�g�̊Ǘ�
EffectManager*  Locator::_EffectManager = nullptr;
// �V�[���̊Ǘ�
SceneManager* Locator::_SceneManager = nullptr;
// �Փ˔���̊Ǘ� 
ObjectManager* Locator::_CollisionManager = nullptr;
// �R���g���[���̊Ǘ�
ControllerManager* Locator::_ControllerManager = nullptr;
// �T�E���h�}�l�[�W���[ 
SoundManager* Locator::_SoundManager = nullptr;
// �f�o�C�X���\�[�X 
DX::DeviceResources* Locator::_DeviceResources = nullptr;