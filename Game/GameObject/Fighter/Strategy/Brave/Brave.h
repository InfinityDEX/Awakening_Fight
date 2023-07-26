#pragma once
#ifndef BRAVE
#define BRAVE

#include <Game\GameObject\Fighter\Strategy\Interface\FighterIdentity.h>
#include <Game\GameObject\Fighter\FighterFrame\Fighter.h>
#include <Game\GameObject\Physics\Collider\AttackedCollider\AttackedCollider.h>
#include <Game\Effect\PrimitiveEffect\SwichableAnimation\SwichableAnimation.h>

// 勇者（仮）
class Brave : public FighterIdentity
{
	public:
		// コンストラクタ
		Brave() = default;
		// デストラクタ
		~Brave() = default;
	private:
		// 初期化関数　　　　　　　　　　　 　　　　　
		void Initialize(FighterShareData* plug) override;
		// 通常更新処理
		void Update(const DX::StepTimer& timer)override;
		// 立ち
		void Stand(const DX::StepTimer& timer) override;
		// しゃがみ
		void Squat(const DX::StepTimer& timer) override;
		// 空中
		void Fly(const DX::StepTimer& timer) override;

		std::unique_ptr<SwichableAnimation> m_animations;
};

#endif // !BRAVE
