#include "pch.h"
#include "RectangleGage.h"
#include <Game\Locator\Locator.h>
#include <assert.h>

// コンストラクタ
RectangleGage::RectangleGage(Vector3 pos, Vector3 scale, const wchar_t * flamePath, const wchar_t * gagePath, const wchar_t * maskPath)
	: m_position(pos)
	, m_scale(scale)
{
	// デバイス取得
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// テクスチャの作成
	CreateWICTextureFromFile(device, flamePath, nullptr, m_frame.GetAddressOf());
	// ゲージ部の作成
	CreateWICTextureFromFile(device, gagePath, nullptr, m_gage.GetAddressOf());
	// マスクの作成
	CreateWICTextureFromFile(device, maskPath, nullptr, m_mask.GetAddressOf());
}

// 初期化処理
void RectangleGage::Initialize()
{
	// デバイス取得
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// デバイスコンテキスト取得
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートの作成
	m_states = std::make_unique<CommonStates>(device);
	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context);
	// 頂点の作成
	m_vertex.push_back(VertexPositionColorTexture(Vector4::Zero, Vector4::Zero, Vector2::Zero));
}

// 更新処理
void RectangleGage::Update(const DX::StepTimer & timer)
{
	// ピクセルシェーダ用の定数バッファ
	GagePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// 画像の色は白色(加算しない)
	pCBuffer.Color = Colors::White;

	// 時間を設定
	pCBuffer.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);
}

// 描画処理
void RectangleGage::Render(Matrix world, Matrix view, Matrix proj)
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
	// ピクセルシェーダ用の定数バッファ
	GagePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// 頂点シェーダ用の定数バッファ
	DefaultVS::ConstBuffer& vCBuffer = m_vShader.GetCBuffer();

	// ワールド行列をセット
	gCBuffer.matWorld = pCBuffer.matWorld = vCBuffer.matWorld = (
		Matrix::CreateScale(m_scale) *
		Matrix::CreateTranslation(m_position)
		).Transpose();
	// ビュー行列をセット
	gCBuffer.matView = pCBuffer.matView = vCBuffer.matView = Matrix::Identity.Transpose();
	// プロジェクション行列をセット
	gCBuffer.matProj = pCBuffer.matProj = vCBuffer.matProj = Matrix::Identity.Transpose();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファは参照のみ
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// 裏返るのでで両面カリング
	context->RSSetState(m_states->CullNone());
	// テクスチャをセット
	context->PSSetShaderResources(0, 1, m_frame.GetAddressOf());
	// マスクをセット
	context->PSSetShaderResources(1, 1, m_mask.GetAddressOf());
	// ゲージ部をセット
	context->PSSetShaderResources(2, 1, m_gage.GetAddressOf());
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
bool RectangleGage::Death()
{
	// マネージャが消滅するかリセットされるまで消えない
	return false;
}

// ゲージの残量指定(MAXは1)
void RectangleGage::Amount(float size)
{
	// ピクセルシェーダ用の定数バッファ
	GagePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// ０以下は０、１以上は１
	size = std::fmaxf(0.0f, std::fminf(size, 1.0f));
	// ゲージの長さを変更する
	pCBuffer.Value.x = size;
}
