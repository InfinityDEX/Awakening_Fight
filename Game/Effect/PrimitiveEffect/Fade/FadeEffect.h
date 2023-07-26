#pragma once
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Color\ColorPS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <CommonStates.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class FadeEffect final : public EffectInterface
{
	public:
		// コンストラクタ
		FadeEffect(Vector3 pos, Vector3 scale, unsigned int fadeIn, unsigned  int fadeOut, int totalLife, const wchar_t * path);
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
		ColorPS m_pShader;
		// 頂点シェーダー
		DefaultVS m_vShader;

		// 座標
		Vector3 m_position;
		// スケール
		Vector3 m_scale;

		// ライフ
		int m_life;
		
		// ライフ初期値
		const int m_startLife;

		// フェードインの時間
		const unsigned int m_fadeIn;

		// フェードアウトの時間
		const unsigned int m_fadeOut;
};