#pragma once
#include <Game\Controller\CommandData\CommandData.h>
#include <Game\Controller\Interface\IController.h>
#include <deque>
#include <map>

// コントローラマネージャ
class ControllerManager final
{
	public:
		// コントローラーの追加
		void Register(int num, std::unique_ptr<IController> controller);
		// 入力履歴の取得
		CommandData CommandHistory(int num);
		// コントローラのアップデート
		void Update();
		// 入力データの取得
		int GetInputData(int num);
		int GetInputData();
		// 瞬間入力の取得
		int GetMomentInputData(int num);
		int GetMomentInputData();
	private:
		// 入力データの履歴
		std::map<int, CommandData> m_commands;
		// コントローラーのコンテナ
		std::map<int,std::unique_ptr<IController>> m_controllers;
};