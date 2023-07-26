#include "pch.h"
#include "SwichableAnimation.h"
#include <Game\Locator\Locator.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Sprite\SpritePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>
#include <WICTextureLoader.h>

// �R���X�g���N�^
SwichableAnimation::SwichableAnimation(const Vector3& targetPos, const Vector3 scale, char* path)
	: m_currentAnimation(-1)
	, m_position(targetPos)
	, m_scale(scale)
	, m_life(60)
	, m_cycle(60)
{
	// �A�j���[�V�����f�[�^�ǂݍ���
	m_animations = LoadAnimationData(path);
}

// ����������
void SwichableAnimation::Initialize()
{
	// �f�o�C�X�擾
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g�擾
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	// �R�����X�e�[�g�̍쐬
	m_states = std::make_unique<CommonStates>(device);
	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context);

	// ���_�̍쐬
	m_vertex.push_back(VertexPositionColorTexture(Vector4::Zero, Vector4::Zero, Vector2::Zero));

	// �폜�\����؂�
	m_kill = false;
}

// �X�V����
void SwichableAnimation::Update(const DX::StepTimer & timer)
{
	// ���C�t��0(�ȉ�)��������
	if (m_life <= 0)
	{
		m_life = m_cycle;
	}

	// �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@
	SpritePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// �X�v���b�g���W�̍X�V
	pCBuffer.Pos.x = static_cast<int>(pCBuffer.Split.x - pCBuffer.Split.x * (static_cast<float>(m_life) * pCBuffer.Split.y / static_cast<float>(m_cycle)));
	pCBuffer.Pos.y = static_cast<int>(pCBuffer.Split.y - pCBuffer.Split.y * (static_cast<float>(m_life) / static_cast<float>(m_cycle)));

	// ���Ԃ��Z�b�g
	pCBuffer.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);
	// ���C�t�����炷
	m_life--;
}

// �`�揈��
void SwichableAnimation::Render(Matrix world, Matrix view, Matrix proj)
{
	// �f�o�C�X�̎擾
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// �R���e�L�X�g�̎擾
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	// �u�����h���擾
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	// �T���v���̔z��擾
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };

	// �W�I���g���V�F�[�_�p�̒萔�o�b�t�@
	DefaultGS::ConstBuffer& gCBuffer = m_gShader.GetCBuffer();
	// �W�I���g���V�F�[�_�p�̒萔�o�b�t�@
	SpritePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// �W�I���g���V�F�[�_�p�̒萔�o�b�t�@
	DefaultVS::ConstBuffer& vCBuffer = m_vShader.GetCBuffer();

	// ���[���h�s����Z�b�g
	gCBuffer.matWorld = pCBuffer.matWorld = vCBuffer.matWorld = (Matrix::CreateScale(m_scale) * Matrix::CreateTranslation(m_position)).Transpose();
	// �r���[�s����Z�b�g
	gCBuffer.matView = pCBuffer.matView = vCBuffer.matView = view.Transpose();
	// �v���W�F�N�V�����s����Z�b�g
	gCBuffer.matProj = pCBuffer.matProj = vCBuffer.matProj = proj.Transpose();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�͎Q�Ƃ̂�
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// ���ʃJ�����O
	context->RSSetState(m_states->CullNone());
	// �e�N�X�`�����Z�b�g
	context->PSSetShaderResources(0, 1, m_animations[m_currentAnimation].texture.GetAddressOf());
	// �e�N�X�`���T���v���̓o�^
	context->PSSetSamplers(0, 1, sampler);

	// �萔�o�b�t�@��GPU�ɑ���
	m_gShader.UpdateCBuffer();
	m_pShader.UpdateCBuffer();
	m_vShader.UpdateCBuffer();
	// ���̓��C�A�E�g�𔽉f
	m_vShader.IASetInputLayout();

	// �`��̊J�n
	m_batch->Begin();
	// �`��̊J�n
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex[0], m_vertex.size());
	// �`��̏I��
	m_batch->End();
}

// �����������`
bool SwichableAnimation::Death()
{
	// �폜�\�����󂯂�
	return m_kill;
}

// �A�j���[�V�����̐؂�ւ�
void SwichableAnimation::SwitchAnimetion(int oneCycle, int ID)
{
	m_currentAnimation = ID;
	SpritePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	pCBuffer.Split = static_cast<Vector4>(m_animations[ID].split);

	m_cycle = m_life = oneCycle;
}

// �����̐ݒ�
void SwichableAnimation::Direction(Dir dir)
{
	// �������̑傫����ύX
	m_scale.x = dir;
}
