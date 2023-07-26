#include "pch.h"
#include "TitleBackGrandEffect.h"
#include <Game\Locator\Locator.h>
#include <WICTextureLoader.h>

// コンストラクタ
TitleBackGrandEffect::TitleBackGrandEffect()
{
	// 頂点の作成
	m_vertex.push_back(VertexPositionColorTexture(Vector4::Zero, Vector4::Zero, Vector2::Zero));
}

// 初期化処理
void TitleBackGrandEffect::Initialize()
{
	// デバイス取得
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// デバイスコンテキスト取得
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	// コモンステートの作成
	m_states = std::make_unique<CommonStates>(device);
	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context);

	// ピクセルシェーダ用の定数バッファ
	WavePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// グレースケールを作成
	CreateWICTextureFromFile(device, L"Resources\\Sprite\\EffectParts\\WaveEffect\\GrayScale.png", nullptr, m_texture.GetAddressOf());

	// 色を設定
	pCBuffer.MainColor = Colors::DeepSkyBlue;
	pCBuffer.AlphaColor = Colors::Aqua;
}

// 更新処理
void TitleBackGrandEffect::Update(const DX::StepTimer & timer)
{
	// ピクセルシェーダ用の定数バッファ
	WavePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	//// スプリット座標の更新
	//pCBuffer.Pos.x = static_cast<int>(pCBuffer.Split.x - pCBuffer.Split.x * (static_cast<float>(m_life) * pCBuffer.Split.y / static_cast<float>(m_cycle)));
	//pCBuffer.Pos.y = static_cast<int>(pCBuffer.Split.y - pCBuffer.Split.y * (static_cast<float>(m_life) / static_cast<float>(m_cycle)));

	// 時間をセット
	pCBuffer.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);
}

// 描画処理
void TitleBackGrandEffect::Render(Matrix world, Matrix view, Matrix proj)
{
	// デバイスの取得
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// コンテキストの取得
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	// ブレンド情報取得
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	// サンプラの配列取得
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };

	// ジオメトリシェーダ用の定数バッファ
	DefaultGS::ConstBuffer& gCBuffer = m_gShader.GetCBuffer();
	// ジオメトリシェーダ用の定数バッファ
	WavePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// ジオメトリシェーダ用の定数バッファ
	DefaultVS::ConstBuffer& vCBuffer = m_vShader.GetCBuffer();

	// ワールド行列をセット
	gCBuffer.matWorld = pCBuffer.matWorld = vCBuffer.matWorld = Matrix::Identity.Transpose();
	// ビュー行列をセット
	gCBuffer.matView = pCBuffer.matView = vCBuffer.matView = Matrix::Identity.Transpose();
	// プロジェクション行列をセット
	gCBuffer.matProj = pCBuffer.matProj = vCBuffer.matProj = Matrix::Identity.Transpose();
	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファは参照のみ
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// 反時計回りでカリング
	context->RSSetState(m_states->CullCounterClockwise());
	// テクスチャをセット
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	// テクスチャサンプラの登録
	context->PSSetSamplers(0, 1, sampler);

	// 定数バッファをGPUに送る
	m_gShader.UpdateCBuffer();
	m_pShader.UpdateCBuffer();
	m_vShader.UpdateCBuffer();
	// 入力レイアウトを反映
	m_vShader.IASetInputLayout();

	// 描画の開始
	m_batch->Begin();
	// 描画の開始
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex[0], m_vertex.size());
	// 描画の終了
	m_batch->End();
}

// 消える条件定義
bool TitleBackGrandEffect::Death()
{
	// マネージャが消滅するかリセットされるまで消えない
	return false;
}
