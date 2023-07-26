#include "pch.h"
#include "RectangleGage.h"
#include <Game\Locator\Locator.h>
#include <assert.h>

// �R���X�g���N�^
RectangleGage::RectangleGage(Vector3 pos, Vector3 scale, const wchar_t * flamePath, const wchar_t * gagePath, const wchar_t * maskPath)
	: m_position(pos)
	, m_scale(scale)
{
	// �f�o�C�X�擾
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// �e�N�X�`���̍쐬
	CreateWICTextureFromFile(device, flamePath, nullptr, m_frame.GetAddressOf());
	// �Q�[�W���̍쐬
	CreateWICTextureFromFile(device, gagePath, nullptr, m_gage.GetAddressOf());
	// �}�X�N�̍쐬
	CreateWICTextureFromFile(device, maskPath, nullptr, m_mask.GetAddressOf());
}

// ����������
void RectangleGage::Initialize()
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
}

// �X�V����
void RectangleGage::Update(const DX::StepTimer & timer)
{
	// �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@
	GagePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// �摜�̐F�͔��F(���Z���Ȃ�)
	pCBuffer.Color = Colors::White;

	// ���Ԃ�ݒ�
	pCBuffer.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);
}

// �`�揈��
void RectangleGage::Render(Matrix world, Matrix view, Matrix proj)
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
	// �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@
	GagePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// ���_�V�F�[�_�p�̒萔�o�b�t�@
	DefaultVS::ConstBuffer& vCBuffer = m_vShader.GetCBuffer();

	// ���[���h�s����Z�b�g
	gCBuffer.matWorld = pCBuffer.matWorld = vCBuffer.matWorld = (
		Matrix::CreateScale(m_scale) *
		Matrix::CreateTranslation(m_position)
		).Transpose();
	// �r���[�s����Z�b�g
	gCBuffer.matView = pCBuffer.matView = vCBuffer.matView = Matrix::Identity.Transpose();
	// �v���W�F�N�V�����s����Z�b�g
	gCBuffer.matProj = pCBuffer.matProj = vCBuffer.matProj = Matrix::Identity.Transpose();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�͎Q�Ƃ̂�
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// ���Ԃ�̂łŗ��ʃJ�����O
	context->RSSetState(m_states->CullNone());
	// �e�N�X�`�����Z�b�g
	context->PSSetShaderResources(0, 1, m_frame.GetAddressOf());
	// �}�X�N���Z�b�g
	context->PSSetShaderResources(1, 1, m_mask.GetAddressOf());
	// �Q�[�W�����Z�b�g
	context->PSSetShaderResources(2, 1, m_gage.GetAddressOf());
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
bool RectangleGage::Death()
{
	// �}�l�[�W�������ł��邩���Z�b�g�����܂ŏ����Ȃ�
	return false;
}

// �Q�[�W�̎c�ʎw��(MAX��1)
void RectangleGage::Amount(float size)
{
	// �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@
	GagePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// �O�ȉ��͂O�A�P�ȏ�͂P
	size = std::fmaxf(0.0f, std::fminf(size, 1.0f));
	// �Q�[�W�̒�����ύX����
	pCBuffer.Value.x = size;
}
