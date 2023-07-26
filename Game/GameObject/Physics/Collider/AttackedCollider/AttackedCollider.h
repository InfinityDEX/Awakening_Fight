#pragma once
#include <Game\DataStructure\Fighter\Loader\Skill\SkillDataLoader.h>
#include <Game\3rdParty\Timer\StepTimer.h>
#include <Game\GameObject\Interface\IObject.h>
#include <Game\GameObject\Physics\PrimitivePhysics\Box\Box.h>

class FighterFrame;
class AttackedCollider final : public Box
{
	public:
		enum Type
		{
			High,
			Low,
			OverHead,
			Throw,
		};
	public:
		AttackedCollider(FighterFrame* target, Vector3* chase, ImpactData impactData);
		int Attack() { return m_damage; };
		~AttackedCollider();
		void Create() override;
		void Initialize() override;
		void Update(DX::StepTimer const& timer) override;
		void Render(const Matrix view, const Matrix proj) override;
		void Reset() override;
		AttackedCollider::Type GetAttackType() { return m_attackType; };
	private:
		Type m_attackType;
		FighterFrame* m_target;
		int m_life;
		DamageData m_damageData;
		int m_damage;
		const Vector3 m_farAway;
		const Vector3* m_chaseTarget;
		Vector3  m_interval;
};

