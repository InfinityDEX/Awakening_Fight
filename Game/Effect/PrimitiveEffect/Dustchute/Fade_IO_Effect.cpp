#include "pch.h"
#include "Fade_IO_Effect.h"


#include <Game\3rdParty\StepTimer.h>
#include <Game\3rdParty\BinaryFile.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

FadeEffect::FadeEffect(Vector3 start, Vector3 end, Vector3 vel, float moveTime, Vector3 scale, wchar_t* path)
	: EffectInterface(start, vel, path)
	, MoveTime(moveTime)
	, StartPos(start)
	, EndPos(end)
	, m_thickness(Vector4::One)
	, m_route(GOING::Fort)
	, m_beforeRoute(GOING::Fort)
	, m_scale(scale)
{
	m_vertex.push_back(VertexPositionColorTexture(start, Vector4::Zero, Vector2::Zero));
}

void FadeEffect::Update(DX::StepTimer timer)
{
	if (m_route != m_beforeRoute)
	{
		switch (m_route)
		{
		case FadeEffect::Fort:
			m_nowTime = 0;
			break;
		case FadeEffect::Back:
			m_nowTime = MoveTime;
			break;
		default:
			break;
		}
	}
	m_nowTime += timer.GetElapsedSeconds() * m_route;
	if (m_nowTime > MoveTime) m_nowTime = MoveTime;
	m_ratio = std::fmaxf(std::fminf(m_nowTime / MoveTime, 1), 0);
	m_vertex[0] = VertexPositionColorTexture(Vector3::Lerp(StartPos, EndPos, m_ratio), Vector4::Zero, Vector2::Zero);
	switch (m_route)
	{
	case FadeEffect::Fort:
		m_cbuff.thickness = Vector4(1, 1, 1, m_ratio);
		break;
	case FadeEffect::Back:
		m_cbuff.thickness = Vector4(1, 1, 1, 1 - m_ratio);
		break;
	default:
		break;
	}
}

void FadeEffect::Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj)
{
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	auto context = deviceResourcese->GetD3DDeviceContext();
	
	m_cbuff.thickness = m_thickness;
	m_cbuff.matWorld = Matrix::CreateScale(m_scale);
	m_cbuff.matView = view;
	m_cbuff.matProj = proj;
	
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

	//�萔�o�b�t�@���V�F�[�_�ɓn���i�Ƃ肠����PS�͗v��Ȃ��̂ŃR�����g�A�E�g�j
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// ���ʃJ�����O
	context->PSSetSamplers(0, 1, sampler);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}

void FadeEffect::Create(DX::DeviceResources * deviceResources)
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
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Transmission/TransmissionPS.cso");

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

bool FadeEffect::MoveEnd()
{
	switch (m_route)
	{
	case FadeEffect::Fort:
		return m_ratio == 1;
	case FadeEffect::Back:
		return m_ratio == 0;
	}
}
