#pragma once
#include <wrl\client.h>
#include <d3d11.h>

// ���_�V�F�[�_�̃C���^�[�t�F�[�X	
class IVShader
{
	public:
		// �萔�o�b�t�@�̍X�V
		virtual void UpdateCBuffer() = 0;
		// �V�F�[�_�[�̎擾
		virtual ID3D11VertexShader* GetShader() = 0;
		// �C���v�b�g���C�A�E�g����̓A�Z���u���[�X�e�[�W�ɃZ�b�g
		virtual void IASetInputLayout() = 0;
};