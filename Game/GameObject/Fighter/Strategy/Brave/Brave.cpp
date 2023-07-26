#include "pch.h"
#include "Brave.h"
#include <Game\GameObject\Fighter\FighterFrame\Fighter.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                        
// public                                                                                                                 
//                                                                                                                        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ������
void Brave::Initialize(FighterShareData * plug)
{
	FighterIdentity::Initialize(plug);

	// �A�j���[�V����
	char* animePath = "Resources\\Data\\SkillData\\Brave\\Animation.json";
	m_animations = std::make_unique<SwichableAnimation>(
		Plug().AnimationCenter,
		Vector3::One,
		animePath
	);
	Plug().Animation = m_animations.get();
	Locator::GetEffectManager()->Register(DrawingOrder::Front, m_animations.get());
	// �Z�f�[�^�̓ǂݍ���
	LoadSkill("Resources\\Data\\SkillData\\Brave\\Skill.json");
	// �X�e�[�^�X�̓ǂݍ���
	LoadStatus("Resources\\Data\\SkillData\\Brave\\BraveStatus.json");
	
}

void Brave::Update(const DX::StepTimer & timer)
{

}

void Brave::Stand(const DX::StepTimer & timer)
{
	// �v���O
	FighterShareData& plug = Plug();
	// �T�C�Y�ύX
	*plug.Size = Vector3(Status().StandBodySize);
}

void Brave::Squat(const DX::StepTimer & timer)
{
	// �v���O
	FighterShareData& plug = Plug();
	// �T�C�Y�ύX
	*plug.Size = Vector3(Status().SquatBodySize);
	plug.transform->position.y = plug.Size->y / 2;
}

void Brave::Fly(const DX::StepTimer & timer)
{
	// �v���O
	FighterShareData& plug = Plug();
	// �T�C�Y�ύX
	*plug.Size = Vector3(Status().AerialBodySize);
}
