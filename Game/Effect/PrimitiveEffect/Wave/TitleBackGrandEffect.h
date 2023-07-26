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
#include <Game\Shader\Pixel\Wave\WavePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class TitleBackGrandEffect final : public EffectInterface
{
public:
	// コンストラクタ
	TitleBackGrandEffect();
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
	WavePS m_pShader;
	// 頂点シェーダー
	DefaultVS m_vShader;
};