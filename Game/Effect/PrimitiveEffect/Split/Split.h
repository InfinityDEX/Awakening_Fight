#pragma once
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <SimpleMath.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Sprite\SpritePS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class Split final : public EffectInterface
{
	public:
		// コンストラクタ
		Split(const Vector3 pos, const Vector3 scale, const Vector2 split, wchar_t* path);
		// 初期化処理
		void Initialize() override;
		// 更新処理
		void Update(const DX::StepTimer& timer) override;
		// 描画処理
		void Render(Matrix world, Matrix view, Matrix proj) override;
		// 消える条件定義
		bool Death() override;
		// 表示する画像の場所を指定する
		void SplitPosition(const Vector2 splitPos);
		// 削除申請
		void Kill();
	private:
		// コモンステート
		std::unique_ptr<CommonStates> m_states;
		// プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
		// 頂点インデックス
		std::vector<DirectX::VertexPositionColorTexture> m_vertex;
		// 座標
		const Vector3 m_position;
		// スケール
		Vector3 m_scale;
		// 削除申請
		bool m_kill;
		/// シェーダ
		// ジオメトリシェーダ
		DefaultGS m_gShader;
		// ピクセルシェーダ
		SpritePS m_pShader;
		// 頂点シェーダー
		DefaultVS m_vShader;
		// テクスチャ
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

};