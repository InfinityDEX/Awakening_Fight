#include "pch.h"
#include "MoveNode.h"
#include <WICTextureLoader.h>

#include <Game\3rdParty\BinaryFile.h>
#include "d3d11.h"
#include <Effects.h>

const std::vector<D3D11_INPUT_ELEMENT_DESC> MoveIcon::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

MoveIcon::MoveIcon(Vector2 start, Vector2 end, wchar_t* path, Vector3 scale, float moveTime)
	: StartPos(start) 
	, EndPos(end)
	, MoveTime(moveTime)
	, m_scale(scale)
{
	m_deviceResources = GameContext<DX::DeviceResources>::Get();
	auto device = m_deviceResources->GetD3DDevice();
	DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), path, nullptr, m_texture.GetAddressOf());
	DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Resources\\Textures\\image01.png", nullptr, m_texture2.GetAddressOf());
	m_states = std::make_unique<CommonStates>(device);

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticlePS.cso");

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

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"Resources");
	m_route = GOING::Fort;
}


MoveIcon::~MoveIcon()
{
}

void MoveIcon::Create()
{
}

void MoveIcon::Initialize()
{
	m_ratio = 0;
	m_nowTime = 0;
	switch (m_route)
	{
	case MoveIcon::Fort:
		m_nowTime = 0;
		break;
	case MoveIcon::Back:
		m_nowTime = MoveTime;
		break;
	default:
		break;
	}
	m_ratio = std::fmaxf(std::fminf(m_nowTime / MoveTime, 1), 0);
	m_transform.position = Vector3::Lerp(StartPos, EndPos, m_ratio);
}

void MoveIcon::Reset()
{
}

void MoveIcon::Update(DX::StepTimer const & timer)
{
	if (m_route != m_beforeRoute)
	{
		switch (m_route)
		{
		case MoveIcon::Fort:
			m_nowTime = 0;
			break;
		case MoveIcon::Back:
			m_nowTime = MoveTime;
			break;
		default:
			break;
		}
	}
	m_nowTime += timer.GetElapsedSeconds() * m_route;
	if (m_nowTime > MoveTime) m_nowTime = MoveTime;
	m_ratio = std::fmaxf(std::fminf(m_nowTime / MoveTime, 1), 0);
	m_transform.position = Vector3::Lerp(StartPos, EndPos, m_ratio);

	m_beforeRoute = m_route;
}

void MoveIcon::Render(const Matrix view, const Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_vertex.clear();
	//エフェクトの頂点の座標を取得する
	Vector3 pos =  m_transform.position;
	//取得した座標を登録する
	VertexPositionColorTexture vertex;
	//vertex = VertexPositionColorTexture(pos, Vector4(vel.x,vel.y,vel.z,1), Vector2(0.0f,3.0f));
	//vertex = VertexPositionColorTexture(pos, Vector4::Zero, Vector2::Zero);
	vertex = VertexPositionColorTexture(Vector3::Zero, Vector4::Zero, Vector2::Zero);
	m_vertex.push_back(vertex);

	Matrix  mat = Matrix::Identity;
	Matrix  world = Matrix::Identity;
	world *= Matrix::CreateScale(m_scale);
	world *= Matrix::CreateTranslation(pos);

	Draw(world, mat, mat);
}

void MoveIcon::Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	//定数バッファで渡す値の設定
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();

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

bool MoveIcon::MoveEnd()
{
	switch (m_route)
	{
	case MoveIcon::Fort:
		return m_ratio == 1;
	case MoveIcon::Back:
		return m_ratio == 0;
	}
}

float MoveIcon::CurrentLocation()
{
	switch (m_route)
	{
	case MoveIcon::Fort:
		return m_ratio;
	case MoveIcon::Back:
		return 1 - m_ratio;
	}
}

