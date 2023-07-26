#pragma once
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <SimpleMath.h>
#include <Game\DataStructure\Animation\AnimationDataLoder.h>
#include <Game\Shader\Geometry\Default\DefaultGS.h>
#include <Game\Shader\Pixel\Sprite\SpritePS.h>
#include <Game\Shader\Pixel\Default\DefaultPS.h>
#include <Game\Shader\Vertex\Default\DefaultVS.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class SwichableAnimation final : public EffectInterface
{
	public:
		// コンストラクタ
		SwichableAnimation(const Vector3& targetPos, const Vector3 scale, char* path);
		// 初期化処理
		void Initialize() override;
		// 更新処理
		void Update(const DX::StepTimer& timer) override;
		// 描画処理
		void Render(Matrix world, Matrix view, Matrix proj) override;
		// 消える条件定義
		bool Death() override;
		// 削除申請
		void Kill() { m_kill = true; };
		// アニメーションの切り替え
		void SwitchAnimetion(int oneCycle, int ID);
		enum Dir
		{
			RIGHT = 1, 
			LEFT = -1
		};
		// 向きの設定
		void Direction(Dir dir);
	private:
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
		const Vector3& m_position;
		// スケール
		Vector3 m_scale;
		// 残りフレーム
		int	m_life;
		// アニメーション１サイクル分のフレーム
		int m_cycle;
		// 現在再生中のアニメーション
		int m_currentAnimation;
		// アニメーションの集まり
		AnimationDataRepository m_animations;
		// 削除申請
		bool m_kill;
};