#pragma once
#include <Game\Shader\Geometry\Interface\GSInterface.h>
#include <SimpleMath.h>

// �G�C���A�X�錾
using namespace DirectX;
using namespace DirectX::SimpleMath;

// ��{�ƂȂ�W�I���g���V�F�[�_(�����`�ɕ\��)
class DefaultGS final : public IGShader
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
	DefaultGS();
	// �萔�o�b�t�@�̍X�V
	void UpdateCBuffer() override;
	// �V�F�[�_�[�̎擾
	ID3D11GeometryShader* GetShader() override;
	// �萔�o�b�t�@�p�̃f�[�^�擾
	DefaultGS::ConstBuffer& GetCBuffer();
private:
	// �o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// �萔�o�b�t�@
	DefaultGS::ConstBuffer m_cbuff;
	// �V�F�[�_
	static Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_shader;
};

