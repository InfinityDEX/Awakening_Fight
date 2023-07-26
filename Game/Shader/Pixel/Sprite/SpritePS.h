#pragma once
#include <Game\Shader\Pixel\Interface\PSInterface.h>
#include <SimpleMath.h>

// �G�C���A�X�錾
using namespace DirectX;
using namespace DirectX::SimpleMath;

// �؂蔲���ł���s�N�Z���V�F�[�_
class SpritePS final : public IPShader
{
public:
	// �萔�o�b�t�@�̍\����
	struct ConstBuffer
	{
		Matrix		matWorld;
		Matrix		matView;
		Matrix		matProj;
		Vector4		Time;
		Vector4		Split;
		Vector4		Pos;
	};
	// �R���X�g���N�^
	SpritePS();
	// �萔�o�b�t�@�̍X�V
	void UpdateCBuffer() override;
	// �V�F�[�_�[�̎擾
	ID3D11PixelShader* GetShader() override;
	// �萔�o�b�t�@�p�̃f�[�^�擾
	SpritePS::ConstBuffer& GetCBuffer();
private:
	// �o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// �萔�o�b�t�@
	SpritePS::ConstBuffer m_cbuff;
	// �V�F�[�_
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
};

