#include "pch.h"
#include "UnlimitedEffect.h"

#include <Game\3rdParty\Timer\StepTimer.h>
#include <Game\3rdParty\BinaryFile\BinaryFile.h>
#include <Game\Locator\Locator.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>

// コンストラクタ
UnlimitedEffect::UnlimitedEffect(Vector3 scale, Vector3 pos, const wchar_t* path)
	: m_scale(scale)
	, m_position(pos)
{
	// デバイス取得
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// 頂点の作成
	m_vertex.push_back(VertexPositionColorTexture(Vector4::Zero, Vector4::Zero, Vector2::Zero));
	
	// テクスチャの作成
	CreateWICTextureFromFile(device, path, nullptr, m_texture.GetAddressOf());
}

// 初期化処理
void UnlimitedEffect::Initialize()
{
	// デバイス取得
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// デバイスコンテキスト取得
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	// コモンステートの作成
	m_states = std::make_unique<CommonStates>(device);
	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context);
	// キルトリガーをオフ
	m_killTrigger = false;
}

// 更新処理
void UnlimitedEffect::Update(const DX::StepTimer & timer)
{
	// ピクセルシェーダ用の定数バッファ
	DefaultPS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// 時間をセット
	pCBuffer.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);
}

// 描画処理
void UnlimitedEffect::Render(Matrix world, Matrix view, Matrix proj)
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
	DefaultPS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// ジオメトリシェーダ用の定数バッファ
	DefaultVS::ConstBuffer& vCBuffer = m_vShader.GetCBuffer();

	// ワールド行列をセット
	gCBuffer.matWorld = pCBuffer.matWorld = vCBuffer.matWorld = (Matrix::CreateScale(m_scale) * Matrix::CreateTranslation(m_position)).Transpose();
	// ビュー行列をセット
	gCBuffer.matView = pCBuffer.matView = vCBuffer.matView = view.Transpose();
	// プロジェクション行列をセット
	gCBuffer.matProj = pCBuffer.matProj = vCBuffer.matProj = proj.Transpose();

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
bool UnlimitedEffect::Death()
{
	// マネージャがリセットされるか、外部から削除要請を受けていたら
	return m_killTrigger;
}
