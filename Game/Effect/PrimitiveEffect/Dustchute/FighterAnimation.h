#pragma once
#include <Game\Effect\Animation\AnimationEffect.h>
#include <Game\DataStructure\Fighter\Loader\Animation\AnimationDataLoder.h>

class FighterAnimation : public AnimationEffect
{
	public:
		FighterAnimation(const Vector3* const pos, const int* const dir, const Vector3 scale);
		~FighterAnimation();
		void NowAnimation(int motionID, int cycle = 60, bool nonStop = false);
		void Register(AnimationDataRepository animationRepository);
		virtual void Update(DX::StepTimer timer);

		bool AnimationEnd() { return m_endOfAnimation; };
	private:
		//それぞれのクラスが固有の定数バッファを持つ
		struct ConstBuffer
		{
			Matrix		matWorld;
			Matrix		matView;
			Matrix		matProj;
			Vector4		Time;
			Vector4		Split;
			Vector4		Pos;
		};

		ConstBuffer m_cbuff;
		const Vector3* const m_position;
		const Vector3 m_scale;
		const int* const m_dir;
		struct AnimationData
		{
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
			Vector2 split;
		};
		// アニメーションが終わるまで変更できないモード
		bool m_nonStop;
		bool m_endOfAnimation;

		std::map<int, AnimationData> m_textures;
		// 現在描画してるアニメーションのデータ
		AnimationData* m_currentTextureData;
};