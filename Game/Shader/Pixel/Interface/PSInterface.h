#pragma once
#include <wrl\client.h>
#include <d3d11.h>

// �s�N�Z���V�F�[�_�̃C���^�[�t�F�[�X	
class IPShader
{
public:
	// �萔�o�b�t�@�̍X�V
	virtual void UpdateCBuffer() = 0;
	// �V�F�[�_�[�̎擾
	virtual ID3D11PixelShader* GetShader() = 0;
};