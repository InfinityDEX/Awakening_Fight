#pragma once
#include <wrl\client.h>
#include <d3d11.h>

// �W�I���g���V�F�[�_�̃C���^�[�t�F�[�X	
class IGShader
{
public:
	// �萔�o�b�t�@�̍X�V
	virtual void UpdateCBuffer() = 0;
	// �V�F�[�_�[�̎擾
	virtual ID3D11GeometryShader* GetShader() = 0;
};