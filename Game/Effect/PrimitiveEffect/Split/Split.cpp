#include "pch.h"
#include "Split.h"
#include <Game\Locator\Locator.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Sprite\SpritePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>
#include <WICTextureLoader.h>

// コンストラクタ
Split::Split(const Vector3 pos, const Vector3 scale, const Vector2 split, wchar_t* path)
	: m_position(pos)
	, m_scale(scale)
{
	// デバイス取得
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	// テクスチャの作成
	CreateWICTextureFromFile(device, path, nullptr, m_texture.GetAddressOf());
	// ピクセルシェーダ用の定数バッファ
	SpritePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	// 分割数をセット
	pCBuffer.Split = Vector4(split);
	m_kill = false;
}

// 初期化処理
void Split::Initialize()
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
void Split::Update(const DX::StepTimer & timer)
{

}

// 描画処理
void Split::Render(Matrix world, Matrix view, Matrix proj)
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
	SpritePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
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
bool Split::Death()
{
	// 削除申請が出されたら消す
	return m_kill;
}

// 表示する画像の場所を指定する
void Split::SplitPosition(const Vector2 splitPos)
{
	// ピクセルシェーダ用の定数バッファ
	SpritePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	pCBuffer.Pos = Vector4(splitPos);
}

// 削除申請
void Split::Kill()
{
	m_kill = true;
}
