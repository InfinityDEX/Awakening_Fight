#include "pch.h"
#include "FadeEffect.h"
#include <Game\Locator\Locator.h>
#include <assert.h>

// �R���X�g���N�^
FadeEffect::FadeEffect(Vector3 pos, Vector3 scale, unsigned int fadeIn, unsigned  int fadeOut, int totalLife, const wchar_t * path)
	: m_life(totalLife)
	, m_startLife(totalLife)
	, m_fadeIn(fadeIn)
	, m_fadeOut(fadeOut)
	, m_scale(scale)
	, m_position(pos)
{
	assert(totalLife >= fadeIn + fadeOut && "�t�F�[�h�C���E�A�E�g�̍��v���Ԃ��A�G�t�F�N�g�̎����𒴂��Ă��܂��B");

	// �f�o�C�X�擾
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// ���_�̍쐬
	m_vertex.push_back(VertexPositionColorTexture(Vector4::Zero, Vector4::Zero, Vector2::Zero));

	// �e�N�X�`���̍쐬
	CreateWICTextureFromFile(device, path, nullptr, m_texture.GetAddressOf());
}

// ����������
void FadeEffect::Initialize()
{
	// �f�o�C�X�擾
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g�擾
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	// �R�����X�e�[�g�̍쐬
	m_states = std::make_unique<CommonStates>(device);
	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context);

	// �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@
	ColorPS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// �摜�̐F�͔��F(���Z���Ȃ�)
	pCBuffer.Color = Colors::White;
}

// �X�V����
void FadeEffect::Update(const DX::StepTimer & timer)
{
	// ���C�t��0(�ȉ�)��������
	if (m_life <= 0)
	{
		// �������I����
		return;
	}

	// �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@
	ColorPS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// (|-------- total ---------|)
	// (|- fadein -|-|- fadeout -|)
	// �t�F�[�h�C���̊��Ԓ���������
	if (m_startLife - m_fadeIn <= m_life)
	{
		// �A���t�@�l�����������₷
		pCBuffer.Color.w = static_cast<float>(m_startLife - m_life) / static_cast<float>(m_fadeIn);
	}
	// �t�F�[�h�A�E�g�̊��Ԓ���������
	if (m_life <= m_fadeOut)
	{
		// �A���t�@�l�����������炷
		pCBuffer.Color.w = static_cast<float>(m_life) / static_cast<float>(m_fadeOut);
	}

	// ���Ԃ��Z�b�g
	pCBuffer.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);

	// ���C�t�����炷
	m_life--;
}

// �`�揈��
void FadeEffect::Render(Matrix world, Matrix view, Matrix proj)
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
	ColorPS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// �W�I���g���V�F�[�_�p�̒萔�o�b�t�@
	DefaultVS::ConstBuffer& vCBuffer = m_vShader.GetCBuffer();

	// ���[���h�s����Z�b�g
	gCBuffer.matWorld = pCBuffer.matWorld = vCBuffer.matWorld = (Matrix::CreateScale(m_scale) * Matrix::CreateTranslation(m_position)).Transpose();
	// �r���[�s����Z�b�g
	gCBuffer.matView = pCBuffer.matView = vCBuffer.matView = Matrix::Identity.Transpose();
	// �v���W�F�N�V�����s����Z�b�g
	gCBuffer.matProj = pCBuffer.matProj = vCBuffer.matProj = Matrix::Identity.Transpose();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�͎Q�Ƃ̂�
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// �����v���ŃJ�����O
	context->RSSetState(m_states->CullCounterClockwise());
	// �e�N�X�`�����Z�b�g
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
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
bool FadeEffect::Death()
{
	// �G�t�F�N�g�̎������s������
	return m_life <= 0;
}
