#pragma once
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <SimpleMath.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Sprite\SpritePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class Split final : public EffectInterface
{
	public:
		// �R���X�g���N�^
		Split(const Vector3 pos, const Vector3 scale, const Vector2 split, wchar_t* path);
		// ����������
		void Initialize() override;
		// �X�V����
		void Update(const DX::StepTimer& timer) override;
		// �`�揈��
		void Render(Matrix world, Matrix view, Matrix proj) override;
		// �����������`
		bool Death() override;
		// �\������摜�̏ꏊ���w�肷��
		void SplitPosition(const Vector2 splitPos);
		// �폜�\��
		void Kill();
	private:
		// �R�����X�e�[�g
		std::unique_ptr<CommonStates> m_states;
		// �v���~�e�B�u�o�b�`
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
		// ���_�C���f�b�N�X
		std::vector<DirectX::VertexPositionColorTexture> m_vertex;
		// ���W
		const Vector3 m_position;
		// �X�P�[��
		Vector3 m_scale;
		// �폜�\��
		bool m_kill;
		/// �V�F�[�_
		// �W�I���g���V�F�[�_
		DefaultGS m_gShader;
		// �s�N�Z���V�F�[�_
		SpritePS m_pShader;
		// ���_�V�F�[�_�[
		DefaultVS m_vShader;
		// �e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

};