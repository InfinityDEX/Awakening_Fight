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
		//���ꂼ��̃N���X���ŗL�̒萔�o�b�t�@������
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
		// �A�j���[�V�������I���܂ŕύX�ł��Ȃ����[�h
		bool m_nonStop;
		bool m_endOfAnimation;

		std::map<int, AnimationData> m_textures;
		// ���ݕ`�悵�Ă�A�j���[�V�����̃f�[�^
		AnimationData* m_currentTextureData;
};