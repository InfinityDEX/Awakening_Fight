#pragma once
#include <Game\Shader\Pixel\Interface\PSInterface.h>
#include <SimpleMath.h>

// �G�C���A�X�錾
using namespace DirectX;
using namespace DirectX::SimpleMath;

// ��{�ƂȂ�W�I���g���V�F�[�_(�����`�ɕ\��)
class GagePS final : public IPShader
{
public:
	// �萔�o�b�t�@�̍\����
	struct ConstBuffer
	{
		Matrix matWorld;
		Matrix matView;
		Matrix matProj;
		Vector4	Time;
		Vector4 Color;
		Vector4 Value;
	};
	// �R���X�g���N�^
	GagePS();
	// �萔�o�b�t�@�̍X�V
	void UpdateCBuffer() override;
	// �V�F�[�_�[�̎擾
	ID3D11PixelShader* GetShader() override;
	// �萔�o�b�t�@�p�̃f�[�^�擾
	GagePS::ConstBuffer& GetCBuffer();
private:
	// �o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// �萔�o�b�t�@
	GagePS::ConstBuffer m_cbuff;
	// �V�F�[�_
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
};
