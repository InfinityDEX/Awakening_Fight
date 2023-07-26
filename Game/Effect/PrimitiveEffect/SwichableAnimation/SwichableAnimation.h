#pragma once
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <SimpleMath.h>
#include <Game\DataStructure\Animation\AnimationDataLoder.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Sprite\SpritePS.h>
#include <Game\Shader\Pixel\Default\DefaultPS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class SwichableAnimation final : public EffectInterface
{
	public:
		// �R���X�g���N�^
		SwichableAnimation(const Vector3& targetPos, const Vector3 scale, char* path);
		// ����������
		void Initialize() override;
		// �X�V����
		void Update(const DX::StepTimer& timer) override;
		// �`�揈��
		void Render(Matrix world, Matrix view, Matrix proj) override;
		// �����������`
		bool Death() override;
		// �폜�\��
		void Kill() { m_kill = true; };
		// �A�j���[�V�����̐؂�ւ�
		void SwitchAnimetion(int oneCycle, int ID);
		enum Dir
		{
			RIGHT = 1, 
			LEFT = -1
		};
		// �����̐ݒ�
		void Direction(Dir dir);
	private:
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
		SpritePS m_pShader;
		// ���_�V�F�[�_�[
		DefaultVS m_vShader;
		// ���W
		const Vector3& m_position;
		// �X�P�[��
		Vector3 m_scale;
		// �c��t���[��
		int	m_life;
		// �A�j���[�V�����P�T�C�N�����̃t���[��
		int m_cycle;
		// ���ݍĐ����̃A�j���[�V����
		int m_currentAnimation;
		// �A�j���[�V�����̏W�܂�
		AnimationDataRepository m_animations;
		// �폜�\��
		bool m_kill;
};