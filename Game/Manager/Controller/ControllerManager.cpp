#include "pch.h"
#include "ControllerManager.h"
#include <numeric>

// コントローラーの追加
void ControllerManager::Register(int num, std::unique_ptr<IController> controller)
{
	// コントローラーを登録する
	m_controllers[num] = std::move(controller);
}

// 入力履歴の取得
CommandData ControllerManager::CommandHistory(int num)
{
	return m_commands[num];
}

// コントローラの更新
void ControllerManager::Update()
{
	for (int i = 0; i < m_controllers.size(); i++)
	{
		int command = m_controllers[i]->GetInput();
		m_commands[i].Input(command);
	}
}

// 入力データの取得
int ControllerManager::GetInputData(int num)
{
	// 指定したコントローラーから入力データを受け取る
	return m_commands[num][0];
}
int ControllerManager::GetInputData()
{
	// 全てのコントローラの入力値を合算して返す
	int totalInput = 0;
	for (int i = 0; i < m_controllers.size(); i++)
	{
		totalInput += GetInputData(i);
	}
	return totalInput;
}

// 瞬間入力の取得
int ControllerManager::GetMomentInputData(int num)
{
	// 指定したコントローラーから押したばかりの入力データを受け取る
	return m_commands[num][0] - (m_commands[num][0] &m_commands[num][1]);
}
int ControllerManager::GetMomentInputData()
{
	// 全てのコントローラの瞬間入力値を合算して返す
	int totalInput = 0;
	for (int i = 0; i < m_controllers.size(); i++)
	{
		totalInput += GetMomentInputData(i);
	}
	return totalInput;
}
