#pragma once
#include <Game\3rdParty\Timer\StepTimer.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <CommonStates.h>
#include <vector>
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Default\DefaultPS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class DefaultEffect final: public EffectInterface
{
	public:
		// �R���X�g���N�^
		DefaultEffect(int life, Vector3 scale, Vector3 pos, const wchar_t * path);
		// ����������
		void Initialize() override;
		// �X�V����
		void Update(const DX::StepTimer& timer) override;
		// �`�揈��
		void Render(Matrix world, Matrix view, Matrix proj) override;
		// �����������`
		bool Death() override;
		// �c�胉�C�t�̎擾
		const int Limit() { return m_life; };
		// ���W�̎擾
		const Vector3 Position() { return m_position; };
		// ���W�ړ�(���[�v)
		void Teleport(Vector3 pos) { m_position = pos; };
		// ���W�ړ�(����)
		void Movement(Vector3 vel) { m_position += vel; };
		// �X�P�[���ύX
		void ChangeScale(Vector3 scale) { m_scale = scale; };
	private:
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
		DefaultPS m_pShader;
		// ���_�V�F�[�_�[
		DefaultVS m_vShader;

		// ���W
		Vector3 m_position;
		// �X�P�[��
		Vector3 m_scale;

		// ���C�t
		int m_life;
};