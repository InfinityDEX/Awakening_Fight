#pragma once
#include <Game\3rdParty\Timer\StepTimer.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Wave\WavePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class TitleBackGrandEffect final : public EffectInterface
{
public:
	// �R���X�g���N�^
	TitleBackGrandEffect();
	// ����������
	void Initialize() override;
	// �X�V����
	void Update(const DX::StepTimer& timer) override;
	// �`�揈��
	void Render(Matrix world, Matrix view, Matrix proj) override;
	// �����������`
	bool Death() override;
private:
	// ���[�v�g���K�[
	bool m_loop;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �R�����X�e�[�g
	std::unique_ptr<CommonStates> m_states;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// ���_�C���f�b�N�X
	std::vector<DirectX::VertexPositionColorTexture> m_vertex;

	/// �V�F�[�_
	// �W�I���g���V�F�[�_
	DefaultGS m_gShader;
	// �s�N�Z���V�F�[�_
	WavePS m_pShader;
	// ���_�V�F�[�_�[
	DefaultVS m_vShader;
};