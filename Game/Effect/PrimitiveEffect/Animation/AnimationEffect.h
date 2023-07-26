#pragma once
#include <Game\3rdParty\Timer\StepTimer.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Sprite\SpritePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class AnimationEffect final: public EffectInterface
{
public:
	// コンストラクタ
	AnimationEffect(int cycle, Vector2 Split, Vector3 scale, Vector3 pos, wchar_t* path, bool loop = false);
	// 初期化処理
	void Initialize() override;
	// 更新処理
	void Update(const DX::StepTimer& timer) override;
	// 描画処理
	void Render(Matrix world, Matrix view, Matrix proj) override;
	// 消える条件定義
	bool Death() override;
private:
	// ループトリガー
	bool m_loop;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// コモンステート
	std::unique_ptr<CommonStates> m_states;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// 頂点インデックス
	std::vector<DirectX::VertexPositionColorTexture> m_vertex;
	
	/// シェーダ
	// ジオメトリシェーダ
	DefaultGS m_gShader;
	// ピクセルシェーダ
	SpritePS m_pShader;
	// 頂点シェーダー
	DefaultVS m_vShader;

	// 座標
	Vector3 m_position;
	// スケール
	Vector3 m_scale;

	// 残りフレーム
	int	m_life;
	// アニメーション１サイクル分のフレーム
	const int m_cycle;
};