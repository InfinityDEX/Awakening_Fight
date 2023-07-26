#include "pch.h"
#include "FadeEffect.h"
#include <Game\Locator\Locator.h>
#include <assert.h>

// コンストラクタ
FadeEffect::FadeEffect(Vector3 pos, Vector3 scale, unsigned int fadeIn, unsigned  int fadeOut, int totalLife, const wchar_t * path)
	: m_life(totalLife)
	, m_startLife(totalLife)
	, m_fadeIn(fadeIn)
	, m_fadeOut(fadeOut)
	, m_scale(scale)
	, m_position(pos)
{
	assert(totalLife >= fadeIn + fadeOut && "フェードイン・アウトの合計時間が、エフェクトの寿命を超えています。");

	// デバイス取得
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// 頂点の作成
	m_vertex.push_back(VertexPositionColorTexture(Vector4::Zero, Vector4::Zero, Vector2::Zero));

	// テクスチャの作成
	CreateWICTextureFromFile(device, path, nullptr, m_texture.GetAddressOf());
}

// 初期化処理
void FadeEffect::Initialize()
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
	ColorPS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// 画像の色は白色(加算しない)
	pCBuffer.Color = Colors::White;
}

// 更新処理
void FadeEffect::Update(const DX::StepTimer & timer)
{
	// ライフが0(以下)だったら
	if (m_life <= 0)
	{
		// 処理を終える
		return;
	}

	// ピクセルシェーダ用の定数バッファ
	ColorPS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// (|-------- total ---------|)
	// (|- fadein -|-|- fadeout -|)
	// フェードインの期間中だったら
	if (m_startLife - m_fadeIn <= m_life)
	{
		// アルファ値を少しずつ増やす
		pCBuffer.Color.w = static_cast<float>(m_startLife - m_life) / static_cast<float>(m_fadeIn);
	}
	// フェードアウトの期間中だったら
	if (m_life <= m_fadeOut)
	{
		// アルファ値を少しずつ減らす
		pCBuffer.Color.w = static_cast<float>(m_life) / static_cast<float>(m_fadeOut);
	}

	// 時間をセット
	pCBuffer.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);

	// ライフを減らす
	m_life--;
}

// 描画処理
void FadeEffect::Render(Matrix world, Matrix view, Matrix proj)
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
	ColorPS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// ジオメトリシェーダ用の定数バッファ
	DefaultVS::ConstBuffer& vCBuffer = m_vShader.GetCBuffer();

	// ワールド行列をセット
	gCBuffer.matWorld = pCBuffer.matWorld = vCBuffer.matWorld = (Matrix::CreateScale(m_scale) * Matrix::CreateTranslation(m_position)).Transpose();
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
bool FadeEffect::Death()
{
	// エフェクトの寿命が尽きた時
	return m_life <= 0;
}
