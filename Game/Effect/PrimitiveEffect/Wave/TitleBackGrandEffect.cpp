#include "pch.h"
#include "TitleBackGrandEffect.h"
#include <Game\Locator\Locator.h>
#include <WICTextureLoader.h>

// �R���X�g���N�^
TitleBackGrandEffect::TitleBackGrandEffect()
{
	// ���_�̍쐬
	m_vertex.push_back(VertexPositionColorTexture(Vector4::Zero, Vector4::Zero, Vector2::Zero));
}

// ����������
void TitleBackGrandEffect::Initialize()
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
	WavePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// �O���[�X�P�[�����쐬
	CreateWICTextureFromFile(device, L"Resources\\Sprite\\EffectParts\\WaveEffect\\GrayScale.png", nullptr, m_texture.GetAddressOf());

	// �F��ݒ�
	pCBuffer.MainColor = Colors::DeepSkyBlue;
	pCBuffer.AlphaColor = Colors::Aqua;
}

// �X�V����
void TitleBackGrandEffect::Update(const DX::StepTimer & timer)
{
	// �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@
	WavePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	//// �X�v���b�g���W�̍X�V
	//pCBuffer.Pos.x = static_cast<int>(pCBuffer.Split.x - pCBuffer.Split.x * (static_cast<float>(m_life) * pCBuffer.Split.y / static_cast<float>(m_cycle)));
	//pCBuffer.Pos.y = static_cast<int>(pCBuffer.Split.y - pCBuffer.Split.y * (static_cast<float>(m_life) / static_cast<float>(m_cycle)));

	// ���Ԃ��Z�b�g
	pCBuffer.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);
}

// �`�揈��
void TitleBackGrandEffect::Render(Matrix world, Matrix view, Matrix proj)
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
	WavePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// �W�I���g���V�F�[�_�p�̒萔�o�b�t�@
	DefaultVS::ConstBuffer& vCBuffer = m_vShader.GetCBuffer();

	// ���[���h�s����Z�b�g
	gCBuffer.matWorld = pCBuffer.matWorld = vCBuffer.matWorld = Matrix::Identity.Transpose();
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
bool TitleBackGrandEffect::Death()
{
	// �}�l�[�W�������ł��邩���Z�b�g�����܂ŏ����Ȃ�
	return false;
}
