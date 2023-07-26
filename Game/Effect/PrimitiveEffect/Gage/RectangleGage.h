#pragma once
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <CommonStates.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Gage\GagePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

class RectangleGage final : public EffectInterface
{
	public:
		// コンストラクタ
		RectangleGage(Vector3 pos, Vector3 scale, const wchar_t * flamePath, const wchar_t * gagePath, const wchar_t * maskPath);
		// 初期化処理
		void Initialize() override;
		// 更新処理
		void Update(const DX::StepTimer& timer) override;
		// 描画処理
		void Render(Matrix world, Matrix view, Matrix proj) override;
		// 消える条件定義
		bool Death() override;
		// ゲージの残量指定(MAXは1)
		void Amount(float size);
	private:
		// テクスチャ
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_frame;
		// ゲージ部
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gage;
		// ゲージマスク
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_mask;
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
		GagePS m_pShader;
		// 頂点シェーダー
		DefaultVS m_vShader;

		// 座標
		Vector3 m_position;
		// スケール
		Vector3 m_scale;

};