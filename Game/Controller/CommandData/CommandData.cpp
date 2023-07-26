#include "pch.h"
#include "CommandData.h"

const int CommandData::CommandSize = 60;
const int CommandData::PushFream = 30;

void CommandData::Input(int command)
{
	// 一つずつずらす
	for (int i = CommandSize - 1; i > 0; i--)
	{
		m_commands[i] = m_commands[i - 1];
	}
	// 先頭に代入
	m_commands[0] = command;
}
