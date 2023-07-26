#pragma once
#include <list>
#include <Game\Controller\CommandData\CommandData.h>

// コントローラーのインターフェース
class IController
{
	public:
		// コンストラクタ
		IController() = default;
		// デストラクタ
		virtual ~IController() {};
		// 入力データの取得
		virtual int GetInput() = 0;
};