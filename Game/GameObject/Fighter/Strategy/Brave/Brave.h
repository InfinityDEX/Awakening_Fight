#pragma once
#ifndef BRAVE
#define BRAVE

#include <Game\GameObject\Fighter\Strategy\Interface\FighterIdentity.h>
#include <Game\GameObject\Fighter\FighterFrame\Fighter.h>
#include <Game\GameObject\Physics\Collider\AttackedCollider\AttackedCollider.h>
#include <Game\Effect\PrimitiveEffect\SwichableAnimation\SwichableAnimation.h>

// �E�ҁi���j
class Brave : public FighterIdentity
{
	public:
		// �R���X�g���N�^
		Brave() = default;
		// �f�X�g���N�^
		~Brave() = default;
	private:
		// �������֐��@�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@
		void Initialize(FighterShareData* plug) override;
		// �ʏ�X�V����
		void Update(const DX::StepTimer& timer)override;
		// ����
		void Stand(const DX::StepTimer& timer) override;
		// ���Ⴊ��
		void Squat(const DX::StepTimer& timer) override;
		// ��
		void Fly(const DX::StepTimer& timer) override;

		std::unique_ptr<SwichableAnimation> m_animations;
};

#endif // !BRAVE
