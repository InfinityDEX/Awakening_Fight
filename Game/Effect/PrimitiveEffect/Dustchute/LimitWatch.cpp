#include "pch.h"
#include <Game\Effect\LimitWatch.h>
#include <WICTextureLoader.h>

#include <Game\3rdParty\BinaryFile.h>
#include "d3d11.h"
#include <Effects.h>

const std::vector<D3D11_INPUT_ELEMENT_DESC> LimitWatch::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

LimitWatch::LimitWatch(Vector2 pos, Vector2 dir, const int* value, int maxValue)
	: m_value(value)
	, m_maxValue(maxValue)
	, m_hold(1)
{
	m_deviceResources = GameContext<DX::DeviceResources>::Get();
	auto device = m_deviceResources->GetD3DDevice();

	const wchar_t* name = L"Resources\\Sprite\\17_ggrev2 (1).jpg";
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


LimitWatch::~LimitWatch()
{
}

void LimitWatch::Create()
{
}

void LimitWatch::Initialize()
{
}

void LimitWatch::Reset()
{
}

void LimitWatch::Update(DX::StepTimer const & timer)
{
	//VertexPositionColorTexture vertex = VertexPositionColorTexture(Vector3(0, 0, 0), Colors::Red,Vector2::Zero);
	///// 頂点座標の設定
	////色変更
	//vertex.color = XMFLOAT4(Colors::Blue);

	////vertex.color = XMFLOAT4(Colors::Blue);
	//vertex.position = Vector3(-0.5f, -0.5f, 0);// 前左下 0
	//m_vertexPoints.push_back(vertex);

	////vertex.color = XMFLOAT4(Colors::Red);
	//vertex.position = Vector3( 0.5f, -0.5f, 0);// 前右下 1
	//m_vertexPoints.push_back(vertex);	

	////vertex.color = XMFLOAT4(Colors::Green);
	//vertex.position = Vector3(-0.5f,  0.5f, 0);// 前左上 2
	//m_vertexPoints.push_back(vertex);
	//
	////vertex.color = XMFLOAT4(Colors::Black);
	//vertex.position = Vector3( 0.5f,  0.5f, 0);// 前右上 3
	//m_vertexPoints.push_back(vertex);
	//
	//m_indexList = std::vector<uint16_t>{
	//	0,3,2,
	//	3,0,1
	//};
}

void LimitWatch::Render(const Matrix view, const Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_vertex.clear();
	//エフェクトの頂点の座標を取得する
	Vector3 pos =  m_transform.position;
	//取得した座標を登録する
	VertexPositionColorTexture vertex;
	//vertex = VertexPositionColorTexture(pos, Vector4(vel.x,vel.y,vel.z,1), Vector2(0.0f,3.0f));
	vertex = VertexPositionColorTexture(pos, Vector4::Zero, Vector2::Zero);
	m_vertex.push_back(vertex);

	Matrix  mat = Matrix::Identity;
	Matrix  world = Matrix::Identity;
	world *= Matrix::CreateScale(0.4f, 0.1f, 0.1f);

	Draw(world, mat, mat);

	//auto context = GameContext<DX::DeviceResources>::Get()->GetD3DDeviceContext();

	//context->OMSetBlendState(m_state->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	//context->OMSetDepthStencilState(m_state->DepthNone(), 0);
	//context->RSSetState(m_state->CullCounterClockwise());
	////context->RSSetState(m_state->CullNone());

	//context->IASetInputLayout(m_inputLayout.Get());

	//m_effect->SetWorld(
	//	Matrix::CreateTranslation(m_transform.position)
	//);

	//m_effect->SetView(view);
	//m_effect->SetProjection(proj);
	//m_effect->Apply(context);

	//m_batch->Begin();
	////m_batch->DrawQuad(m_vertexPoints[0], m_vertexPoints[1], m_vertexPoints[2], m_vertexPoints[3]);
	//
	//m_batch->DrawIndexed(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_indexList.data(), m_indexList.size(), m_vertexPoints.data(), m_vertexPoints.size());
	////m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vertexPoints[0], m_vertexPoints.size());
	//m_batch->End();
}

void LimitWatch::Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	//定数バッファで渡す値の設定
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();
	// 増減分を一度にではなく、少しずつ増減させる
	m_hold -= (m_hold - (float(*m_value) / m_maxValue)) * 0.1f;
	cbuff.ratio.x = m_hold;

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
