#include "pch.h"
#include "CommandData.h"

const int CommandData::CommandSize = 60;
const int CommandData::PushFream = 30;

void CommandData::Input(int command)
{
	// ˆê‚Â‚¸‚Â‚¸‚ç‚·
	for (int i = CommandSize - 1; i > 0; i--)
	{
		m_commands[i] = m_commands[i - 1];
	}
	// æ“ª‚É‘ã“ü
	m_commands[0] = command;
}
