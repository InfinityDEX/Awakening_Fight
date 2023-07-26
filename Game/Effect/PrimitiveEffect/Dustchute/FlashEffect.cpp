#include "pch.h"
#include "FlashEffect.h"


#include <Game\3rdParty\StepTimer.h>
#include <Game\3rdParty\BinaryFile.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

FlashEffect::FlashEffect(int life, Vector3 pos, wchar_t* path)
	: EffectInterface(pos, Vector3::Zero, nullptr)
	, m_path(path)
	, m_life(life)
{
	m_vertex.push_back(VertexPositionColorTexture(pos, Vector4::Zero, Vector2::Zero));
	m_cbuff.Colors = Vector4(1,1,1,1);
}

void FlashEffect::Update(DX::StepTimer timer)
{
	if (m_life <= 0)
	{
		m_killSwitch = true;
		return;
	}
	EffectInterface::Update(timer);
	m_life--;
	m_cbuff.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(),0);
}

void FlashEffect::Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj)
{
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	auto device = deviceResourcese->GetD3DDevice();
	auto context = deviceResourcese->GetD3DDeviceContext();

	m_cbuff.matWorld = Matrix::Identity.Transpose();
	m_cbuff.matView = view.Transpose();
	m_cbuff.matProj = proj.Transpose();
	
	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_cbuff, 0, 0);
	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�͎Q�Ƃ̂�
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	//// �J�����O�͔����v���
	//context->RSSetState(m_states->CullCounterClockwise());
	// ���ʃJ�����O
	context->RSSetState(m_states->CullNone());
	// �e�N�X�`�����Z�b�g
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	//�萔�o�b�t�@���V�F�[�_�ɓn���i�Ƃ肠����PS�͗v��Ȃ��̂ŃR�����g�A�E�g�j
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// ���ʃJ�����O
	context->PSSetSamplers(0, 1, sampler);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}

void FlashEffect::Create(DX::DeviceResources * deviceResources)
{
	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();
	
	m_states = std::make_unique<CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticlePS.cso");
	CreateWICTextureFromFile(device, m_path, nullptr, m_texture.GetAddressOf());

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
}
