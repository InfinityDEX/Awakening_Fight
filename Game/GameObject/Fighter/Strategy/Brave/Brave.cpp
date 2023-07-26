#include "pch.h"
#include "Brave.h"
#include <Game\GameObject\Fighter\FighterFrame\Fighter.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                        
// public                                                                                                                 
//                                                                                                                        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 初期化
void Brave::Initialize(FighterShareData * plug)
{
	FighterIdentity::Initialize(plug);

	// アニメーション
	char* animePath = "Resources\\Data\\SkillData\\Brave\\Animation.json";
	m_animations = std::make_unique<SwichableAnimation>(
		Plug().AnimationCenter,
		Vector3::One,
		animePath
	);
	Plug().Animation = m_animations.get();
	Locator::GetEffectManager()->Register(DrawingOrder::Front, m_animations.get());
	// 技データの読み込み
	LoadSkill("Resources\\Data\\SkillData\\Brave\\Skill.json");
	// ステータスの読み込み
	LoadStatus("Resources\\Data\\SkillData\\Brave\\BraveStatus.json");
	
}

void Brave::Update(const DX::StepTimer & timer)
{

}

void Brave::Stand(const DX::StepTimer & timer)
{
	// プラグ
	FighterShareData& plug = Plug();
	// サイズ変更
	*plug.Size = Vector3(Status().StandBodySize);
}

void Brave::Squat(const DX::StepTimer & timer)
{
	// プラグ
	FighterShareData& plug = Plug();
	// サイズ変更
	*plug.Size = Vector3(Status().SquatBodySize);
	plug.transform->position.y = plug.Size->y / 2;
}

void Brave::Fly(const DX::StepTimer & timer)
{
	// プラグ
	FighterShareData& plug = Plug();
	// サイズ変更
	*plug.Size = Vector3(Status().AerialBodySize);
}
