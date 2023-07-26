#include "pch.h"
#include "GageLine.h"
#include <WICTextureLoader.h>

#include <Game\3rdParty\BinaryFile.h>
#include "d3d11.h"
#include <Effects.h>
#include <Game\Effect\Spark.h>
#include <Game\Locator\Locator.h>

const std::vector<D3D11_INPUT_ELEMENT_DESC> GageLine::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

GageLine::GageLine(Vector2 pos, Vector2 size, int dir, const int* value, int maxValue, Vector4 color)
	: m_size(size)
	, m_value(value)
	, m_maxValue(maxValue)
	, m_hold(1)
	, m_direction(dir)
	, m_color(color)
	, m_defaultPosition(pos)
{
	m_deviceResources = GameContext<DX::DeviceResources>::Get();
	auto device = m_deviceResources->GetD3DDevice();

	const wchar_t* name = L"Resources\\Sprite\\Segment.png";
	DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), name, nullptr, m_texture.GetAddressOf());
	DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Resources\\Textures\\image01.png", nullptr, m_texture2.GetAddressOf());

	m_states = std::make_unique<CommonStates>(device);

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/LineGage/ParticlePS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());
	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// プリミティブバッチの作成
	m_pBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_deviceResources->GetD3DDeviceContext());

	m_state = std::make_unique<CommonStates>(device);

	m_sBatch = std::make_unique<SpriteBatch>(m_deviceResources->GetD3DDeviceContext());

	m_transform.position = Vector3(pos.x, pos.y, 0);
	m_vibrationVel = Vector3::Zero;

	m_nowTime = 0;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"Resources");
}


GageLine::~GageLine()
{
}

void GageLine::Create()
{
}

void GageLine::Initialize()
{
}

void GageLine::Reset()
{
}

void GageLine::Update(DX::StepTimer const & timer)
{
	m_nowTime += timer.GetElapsedSeconds();
	
	// ゲージの増減があったら
	if (m_beforeValue != *m_value)
	{
		// 振動
		Shake();
	}
	// 座標を指定
	m_transform.position = m_defaultPosition + m_vibrationVel;

	// 振動をゼロに近づける
	m_vibrationVel = Vector3::Lerp(m_vibrationVel, Vector3::Zero, 0.1f);
	
	// ゲージ量を渡す
	m_beforeValue = *m_value;
}

void GageLine::Render(const Matrix view, const Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_vertex.clear();
	Vector3 pos =  m_transform.position;
	//取得した座標を登録する
	VertexPositionColorTexture vertex;
	vertex = VertexPositionColorTexture(pos, Vector4::Zero, Vector2::Zero);
	m_vertex.push_back(vertex);

	Matrix  mat = Matrix::Identity;
	Matrix  world = Matrix::Identity;
	world *= Matrix::CreateScale(Vector3(m_size));

	Draw(world, mat, mat);
}

void GageLine::Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	//定数バッファで渡す値の設定
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();
	// 増減分を一度にではなく、少しずつ増減させる
	m_hold -= (m_hold - (float(*m_value) / m_maxValue)) * 0.1f;
	m_hold = float(*m_value) / m_maxValue;
	/// ゲージの設定
	// ゲージ幅の計算
	cbuff.Status.x = m_hold;
	// ゲージの方向
	cbuff.Status.y = m_direction;
	// ゲージの基本色
	cbuff.Color = m_color;
	
	// 時間を設定
	cbuff.Time.x = m_nowTime;

	Vector2 aspect = Vector2(m_deviceResources->GetOutputSize().right, m_deviceResources->GetOutputSize().bottom);

	//定数バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファは参照のみ
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// カリングは反時計回り
	context->RSSetState(m_states->CullCounterClockwise());

	//バッチに頂点情報を渡す
	//定数バッファをシェーダに渡す（とりあえずPSは要らないのでコメントアウト）
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//サンプラー、シェーダ、画像をそれぞれ登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture2.GetAddressOf());

	//入力レイアウトを反映
	context->IASetInputLayout(m_inputLayout.Get());

	//m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[2], m_vertex[3]);
	
	m_pBatch->Begin();
	m_pBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex[0], m_vertex.size());
	m_pBatch->End();

	//他のモデルに影響が出る可能性があるので使い終わったらシェーダを外す
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void GageLine::FlashGage(float totalSeconds)
{
	float flash = (sin(totalSeconds * 40) + 1) / 4;
	m_color = Vector4(0.5f, 0.0f, 1.0f, 0.0f) - Vector4(flash);
}

void GageLine::Shake()
{
	m_vibrationVel.x = m_direction * 0.5f;
	m_vibrationVel.y = 0.3f;
}
