#pragma once
#include <SimpleMath.h>
#include <map>

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �A�j���[�V�����f�[�^
struct AnimationData
{
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	// �X�v���b�g
	Vector2 split;
};

// �A�j���[�V�����f�[�^�̂����܂�
using AnimationDataRepository = std::map<int, AnimationData>;

// �A�j���[�V�����f�[�^�̓ǂݍ���
AnimationDataRepository LoadAnimationData(char * AnimationDataPath);