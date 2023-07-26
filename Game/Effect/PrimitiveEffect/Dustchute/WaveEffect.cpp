#include "pch.h"
#include "WaveEffect.h"


#include <Game\3rdParty\StepTimer.h>
#include <Game\3rdParty\BinaryFile.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

WaveEffect::WaveEffect(int life, Vector3 pos, Vector3 vel, Vector4 color)
	: EffectInterface(pos, vel, nullptr)
	, m_color(color)
	, m_life(life)
{
	m_vertex.push_back(VertexPositionColorTexture(pos, Vector4::Zero, Vector2::Zero));
}

void WaveEffect::Update(DX::StepTimer timer)
{
	m_vertex[0] = VertexPositionColorTexture(m_position, Vector4::Zero, Vector2::Zero);
	m_position += m_velocity;
	m_life--;
	m_cbuff.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(),0);
}

void WaveEffect::Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj)
{
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	auto context = deviceResourcese->GetD3DDeviceContext();
	
	m_cbuff.Color = m_color;
	m_cbuff.matWorld = Matrix::Identity;
	m_cbuff.matView = view;
	m_cbuff.matProj = proj;
	
	//定数バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_cbuff, 0, 0);
	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファは参照のみ
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	//// カリングは反時計回り
	//context->RSSetState(m_states->CullCounterClockwise());
	// 両面カリング
	context->RSSetState(m_states->CullNone());

	//定数バッファをシェーダに渡す（とりあえずPSは要らないのでコメントアウト）
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// 両面カリング
	context->PSSetSamplers(0, 1, sampler);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}

void WaveEffect::Create(DX::DeviceResources * deviceResources)
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
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/BackGrand/Wave.cso");

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ジオメトリシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
}

void WaveEffect::ChangeColor(Vector4 color)
{
	m_color = color;
}
