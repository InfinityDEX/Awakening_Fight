#pragma once
#include <Game\Effect\EffectObject\Stage\Interface\IStage.h>
#include <Game\Effect\PrimitiveEffect\Unlimited\UnlimitedEffect.h>

class Subway final : public IStage
{
	public:
		// コンストラクタ
		Subway();
		// デストラクタ
		~Subway();
		// 初期化処理
		void Initialize() override {};
		// 更新処理
		void Update(const DX::StepTimer& timer) override;
	private:
		// プラットフォーム(戦場)
		UnlimitedEffect* m_platform;
		// 線路の仕切り
		UnlimitedEffect* m_partition;
		// 向こう側のプラットフォーム
		UnlimitedEffect* m_oppositePlatform;
		// 手前の電車
		UnlimitedEffect* m_backTrain;
		// 手前の電車のスタート座標
		Vector3 m_bTrainStartPos;
		// 手前の電車のスピード
		float m_bTrainSpeed;
		// 向こうの電車
		UnlimitedEffect* m_frontTrain;
		// 向こうの電車のスタート座標
		Vector3 m_fTrainStartPos;
		// 手前の電車のスピード
		float m_fTrainSpeed;
};