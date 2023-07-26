#pragma once
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <CommonStates.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Gage\GagePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

class RectangleGage final : public EffectInterface
{
	public:
		// �R���X�g���N�^
		RectangleGage(Vector3 pos, Vector3 scale, const wchar_t * flamePath, const wchar_t * gagePath, const wchar_t * maskPath);
		// ����������
		void Initialize() override;
		// �X�V����
		void Update(const DX::StepTimer& timer) override;
		// �`�揈��
		void Render(Matrix world, Matrix view, Matrix proj) override;
		// �����������`
		bool Death() override;
		// �Q�[�W�̎c�ʎw��(MAX��1)
		void Amount(float size);
	private:
		// �e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_frame;
		// �Q�[�W��
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gage;
		// �Q�[�W�}�X�N
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_mask;
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
		GagePS m_pShader;
		// ���_�V�F�[�_�[
		DefaultVS m_vShader;

		// ���W
		Vector3 m_position;
		// �X�P�[��
		Vector3 m_scale;

};