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
#include <Game\Shader\Pixel\Sprite\SpritePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class AnimationEffect final: public EffectInterface
{
public:
	// �R���X�g���N�^
	AnimationEffect(int cycle, Vector2 Split, Vector3 scale, Vector3 pos, wchar_t* path, bool loop = false);
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
	SpritePS m_pShader;
	// ���_�V�F�[�_�[
	DefaultVS m_vShader;

	// ���W
	Vector3 m_position;
	// �X�P�[��
	Vector3 m_scale;

	// �c��t���[��
	int	m_life;
	// �A�j���[�V�����P�T�C�N�����̃t���[��
	const int m_cycle;
};