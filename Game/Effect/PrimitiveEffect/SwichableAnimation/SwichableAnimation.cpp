#include "pch.h"
#include "SwichableAnimation.h"
#include <Game\Locator\Locator.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Sprite\SpritePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>
#include <WICTextureLoader.h>

// コンストラクタ
SwichableAnimation::SwichableAnimation(const Vector3& targetPos, const Vector3 scale, char* path)
	: m_currentAnimation(-1)
	, m_position(targetPos)
	, m_scale(scale)
	, m_life(60)
	, m_cycle(60)
{
	// アニメーションデータ読み込み
	m_animations = LoadAnimationData(path);
}

// 初期化処理
void SwichableAnimation::Initialize()
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

	// 削除申請を切る
	m_kill = false;
}

// 更新処理
void SwichableAnimation::Update(const DX::StepTimer & timer)
{
	// ライフが0(以下)だったら
	if (m_life <= 0)
	{
		m_life = m_cycle;
	}

	// ピクセルシェーダ用の定数バッファ
	SpritePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();

	// スプリット座標の更新
	pCBuffer.Pos.x = static_cast<int>(pCBuffer.Split.x - pCBuffer.Split.x * (static_cast<float>(m_life) * pCBuffer.Split.y / static_cast<float>(m_cycle)));
	pCBuffer.Pos.y = static_cast<int>(pCBuffer.Split.y - pCBuffer.Split.y * (static_cast<float>(m_life) / static_cast<float>(m_cycle)));

	// 時間をセット
	pCBuffer.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);
	// ライフを減らす
	m_life--;
}

// 描画処理
void SwichableAnimation::Render(Matrix world, Matrix view, Matrix proj)
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
	gCBuffer.matView = pCBuffer.matView = vCBuffer.matView = view.Transpose();
	// プロジェクション行列をセット
	gCBuffer.matProj = pCBuffer.matProj = vCBuffer.matProj = proj.Transpose();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファは参照のみ
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// 両面カリング
	context->RSSetState(m_states->CullNone());
	// テクスチャをセット
	context->PSSetShaderResources(0, 1, m_animations[m_currentAnimation].texture.GetAddressOf());
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
bool SwichableAnimation::Death()
{
	// 削除申請を受けた
	return m_kill;
}

// アニメーションの切り替え
void SwichableAnimation::SwitchAnimetion(int oneCycle, int ID)
{
	m_currentAnimation = ID;
	SpritePS::ConstBuffer& pCBuffer = m_pShader.GetCBuffer();
	pCBuffer.Split = static_cast<Vector4>(m_animations[ID].split);

	m_cycle = m_life = oneCycle;
}

// 向きの設定
void SwichableAnimation::Direction(Dir dir)
{
	// ｘ方向の大きさを変更
	m_scale.x = dir;
}
