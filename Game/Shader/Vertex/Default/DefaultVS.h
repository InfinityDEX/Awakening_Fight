#pragma once
#include <Game\Shader\Vertex\Interface\VSInterface.h>
#include <SimpleMath.h>
#include <vector>

// �G�C���A�X�錾
using namespace DirectX;
using namespace DirectX::SimpleMath;

class DefaultVS final : public IVShader
{
	public:
		// �萔�o�b�t�@�̍\����
		struct ConstBuffer
		{
			Matrix		matWorld;
			Matrix		matView;
			Matrix		matProj;
			Vector4		Time;
		};
		// �R���X�g���N�^
		DefaultVS();
		// �萔�o�b�t�@�̍X�V
		void UpdateCBuffer() override;
		// �V�F�[�_�[�̎擾
		ID3D11VertexShader* GetShader() override;
		// �C���v�b�g���C�A�E�g����̓A�Z���u���[�X�e�[�W�ɃZ�b�g
		void IASetInputLayout() override;
		// �萔�o�b�t�@�p�̃f�[�^�擾
		DefaultVS::ConstBuffer& GetCBuffer();
	private:
		// �o�b�t�@
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
		// �萔�o�b�t�@
		DefaultVS::ConstBuffer m_cbuff;
		// ���̓��C�A�E�g�f�B�X�N
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
		// ���̓��C�A�E�g
		static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		// �V�F�[�_
		static Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
};