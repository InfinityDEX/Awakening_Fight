#pragma once
#include <Game\3rdParty\Timer\StepTimer.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <CommonStates.h>
#include <vector>
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Default\DefaultPS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class DefaultEffect final: public EffectInterface
{
	public:
		// コンストラクタ
		DefaultEffect(int life, Vector3 scale, Vector3 pos, const wchar_t * path);
		// 初期化処理
		void Initialize() override;
		// 更新処理
		void Update(const DX::StepTimer& timer) override;
		// 描画処理
		void Render(Matrix world, Matrix view, Matrix proj) override;
		// 消える条件定義
		bool Death() override;
		// 残りライフの取得
		const int Limit() { return m_life; };
		// 座標の取得
		const Vector3 Position() { return m_position; };
		// 座標移動(ワープ)
		void Teleport(Vector3 pos) { m_position = pos; };
		// 座標移動(加速)
		void Movement(Vector3 vel) { m_position += vel; };
		// スケール変更
		void ChangeScale(Vector3 scale) { m_scale = scale; };
	private:
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
		DefaultPS m_pShader;
		// 頂点シェーダー
		DefaultVS m_vShader;

		// 座標
		Vector3 m_position;
		// スケール
		Vector3 m_scale;

		// ライフ
		int m_life;
};