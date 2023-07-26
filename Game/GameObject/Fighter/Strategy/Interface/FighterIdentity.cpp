#include "pch.h"
#include "FighterIdentity.h"
#include <Game\GameObject\Fighter\FighterFrame\Fighter.h>

const int FighterIdentity::Repeatedly_coefficient = 10;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                        
// public                                                                                                                 
//                                                                                                                        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FighterIdentity::Initialize(FighterShareData* plug)
{
	m_plug = plug;
}

void FighterIdentity::StateUpdata(const DX::StepTimer & timer)
{
	switch (m_plug->fighterState)
	{
	case STAND:
	case FRONTWALK:
		Stand(timer);
		break;
	case SQUAT:
		Squat(timer);
		break;
	case FLY:
		Fly(timer);
		break;
	default:
		break;
	}
}

const SkillData* FighterIdentity::Find_Matched_Skill()
{
	// 返り値
	const SkillData* tmp = nullptr;
	// 技を調べる
	for (auto skill_Iter = Skills().begin(); skill_Iter != Skills().end() ; skill_Iter++)
	{
		// 現在の状態をとる
		auto nowState = m_plug->fighterState;
		//歩きは立ち状態とする
		if (nowState == FighterState::FRONTWALK || nowState == FighterState::BACKWALK)
			nowState = FighterState::STAND;
			 
		// この技が現在の状態で発動できなければ
		if(nowState != skill_Iter->Condition)
			continue; // この技の判定はしない

		// ボタンが正しく押されていなければ
		if (!PushJudgment(skill_Iter->LastPushButton))
			continue; // この技の判定はしない

		// コマンドの判定
		if (LeverCommandJudgment(*skill_Iter))
		{
			// 合致した
			tmp = &(*skill_Iter);
			break;
		}
	}
	return tmp;
}

// ボタン判定
bool FighterIdentity::PushJudgment(int buttom)
{
	// ボタンが一つも登録されていないデータは無効
	assert(buttom != 0 && "ボタンが一つも登録されていないデータが渡されました");

	// コントローラマネージャ
	auto manager = Locator::GetControllerManager();
	// コマンドデータの参照
	CommandData& command = manager->CommandHistory(m_plug->ControllerNum);

	for (int i = 0; i < CommandData::PushFream; i++)
	{
		if ((command[i] & buttom) != 0)
		{
			buttom -= command[i] & buttom;
			// ボタンがすべて押されていたら
			if (buttom == 0)
			{
				// ボタン入力成立
				return true;
			}
		}
		// 一番最初に必要なボタンの入力が一つもなければ
		else if(i == 0)
		{
			// ボタン入力失敗
			return false;
		}
	}

	// 押されていないボタンが存在している
	return false;
}

// 連打判定(ボタン単一)
bool FighterIdentity::RepeatedlyJudgment(KeyData buttom, int num)
{
	// ボタンが一つも登録されていないデータは無効
	assert(buttom != 0 && "ボタンが一つも登録されていないデータが渡されました");

	// コントローラマネージャ
	auto manager = Locator::GetControllerManager();
	// コマンドデータの参照
	CommandData& command = manager->CommandHistory(m_plug->ControllerNum);

	// このフレーム中に必要なキーが押されてるか確認する
	if ((command[0] & buttom) == 0)
	{
		// このフレーム中にボタンは押されなかった
		return false;
	}

	// 最後に入力が感知されてから遡ったフレーム数
	int frameCount = 0;
	// 連打が成立した回数
	int success = 1;

	// 一つ前のコマンドデータから確認する
	for (int i = 1; i < command.CommandSize; i++)
	{
		// もし必要なボタンの入力があったら
		if ((command[i] & buttom) != 0)
		{
			// フレームカウンタをリセット
			frameCount = 0;
			// 成立カウンタを進める
			success++;
			// もし連打され数が指定した値以上であったら
			if (success >= num)
				return true; // 連打コマンド成立 

			// このまま次のループへ進む
			continue;
		}
		// フレームカウンタを進める
		frameCount++;
		
		// もしフレームカウンタの値が連打の有効フレーム数を超えていたら
		if (frameCount > Repeatedly_coefficient)
		{
			// ループを終了する
			break;
		}
	}

	// 必要な連打数に達しなかった
	return false;
}

// レバーコマンド判定
bool FighterIdentity::LeverCommandJudgment(const SkillData& data)
{
	// コントローラマネージャ
	auto manager = Locator::GetControllerManager();
	// コマンドデータの参照
	CommandData& inputData = manager->CommandHistory(m_plug->ControllerNum);
	// 最初に判定する入力データを保持
	auto currentCommand = data.LeverCommand.rbegin();

	// コマンド履歴を見ていく
	for (int before_What_Frame = 0; before_What_Frame < CommandData::CommandSize; before_What_Frame++)
	{
		// 制限時間を超えていたら
		if (before_What_Frame > data.TimeLimit)
			// この技の判断を終える
			false;

		// 向きを見て反転する
		int input = *currentCommand;
		if (m_plug->Dir == -1)
		{
			input = Flip_horizontal(input);
		}

		// 保持している入力データをチェックする
		if (
			input & inputData[before_What_Frame] &&
			(
				before_What_Frame == 0 ||
				inputData[before_What_Frame] != inputData[before_What_Frame - 1]
				)
			)
		{
			// イテレータを進める
			currentCommand++;
			//	もし今調べたデータが最後のコマンドだったら
			if (currentCommand == data.LeverCommand.rend())
			{
				// 合致した
				return true;
			}
		}
	}

	// コマンド不成立
	return false;
}
bool FighterIdentity::LeverCommandJudgment(const std::vector<int>& command, const int & limit)
{
	// コントローラマネージャ
	auto manager = Locator::GetControllerManager();
	// コマンドデータの参照
	CommandData& inputData = manager->CommandHistory(m_plug->ControllerNum);
	// 最初に判定する入力データを保持
	auto currentCommand = command.rbegin();

	// 入力履歴を見ていく
	for (int before_What_Frame = 0; before_What_Frame < CommandData::CommandSize; before_What_Frame++)
	{
		// 制限時間を超えていたら
		if (before_What_Frame > limit)
			// この技の判断を終える
			false;

		// 向きを見て反転する
		int input = *currentCommand;
		if (m_plug->Dir == -1)
		{
			input = Flip_horizontal(input);
		}

		// 保持している入力データをチェックする
		if (
			input & inputData[before_What_Frame] &&
			(
				before_What_Frame == 0 ||
				inputData[before_What_Frame] != inputData[before_What_Frame - 1]
				)
			)
		{
			// イテレータを進める
			currentCommand++;
			//	もし今調べたデータが最後のコマンドだったら
			if (currentCommand == command.rend())
			{
				// 合致した
				return true;
			}
		}
	}

	// コマンド不成立
	return false;
}
