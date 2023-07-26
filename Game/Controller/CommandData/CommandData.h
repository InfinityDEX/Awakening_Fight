#pragma once
#include <assert.h>
// �R���g���[������̓��͂𓝈ꂷ��ׂ̗񋓑�
enum KeyData
{
	// ��������
	NEUTRAL		= 0b000000000001, // ����
	UP			= 0b000000000010, // ��
	DOWN		= 0b000000000100, // ��
	LEFT		= 0b000000001000, // ��
	RIGHT		= 0b000000010000, // �E
	// �U���{�^��
	WEAK		= 0b000000100000, // ��U��
	MODERATO	= 0b000001000000, // ���U��
	HARD		= 0b000010000000, // ���U��
	// �V�X�e���R�}���h
	DICISION	= 0b000100000000, // ����
	CANCEL		= 0b001000000000, // �L�����Z��
	ESCAPE		= 0b010000000000, // �Q�[���I��
	OPTION		= 0b100000000000, // �I�v�V����
	// �{�^�����ׂ�
	ANY			= 0b111111100000,
};

// -1���P����͂����LEFT��RIGHT�ŕԂ�
inline KeyData DirectionToKeyData(int dir)
{
	assert(dir == -1 || dir == 1 && "�v�����ꂽ�l�ȊO�̐��l���n����܂���(�v������l��-1��1�ł�)");
	return static_cast<KeyData>(KeyData::LEFT * (dir == -1) + KeyData::RIGHT * (dir == 1));
}

// LEFT��RIGHT����͂����-1���P�ŕԂ�
inline int KeyDataToDirection(KeyData dir)
{
	assert(dir & KeyData::LEFT || dir & KeyData::RIGHT && "�v�����ꂽ�l�ȊO�̐��l���n����܂���(�v������l��KeyData::LEFT��KeyData::RIGHT�ł�)");
	return  -1 * (dir == KeyData::LEFT) + 1 * (dir == KeyData::RIGHT);
}

// �e���L�[�����o�[�ԍ����L�[�f�[�^�ɕϊ�
inline int TenKeytoKeyData(int tenKeyNumber)
{
	switch (tenKeyNumber)
	{
	case 1:
		return KeyData::DOWN | KeyData::LEFT;
	case 2:
		return KeyData::DOWN;
	case 3:
		return KeyData::DOWN | KeyData::RIGHT;
	case 4:
		return KeyData::LEFT;
	case 5:
		return KeyData::NEUTRAL;
	case 6:
		return KeyData::RIGHT;
	case 7:
		return KeyData::UP | KeyData::LEFT;
	case 8:
		return KeyData::UP;
	case 9:
		return KeyData::UP | KeyData::RIGHT;
	default:
		assert(!"�P�`�X�ȊO�̒l�����͂���܂���");
		break;
	}
}

// ���E�̓��͂𔽓]
inline int Flip_horizontal(int input)
{
	int flip = input;

	// ���̃f�[�^����������
	if (flip & KeyData::LEFT)
	{
		// left������
		flip ^= KeyData::LEFT;
		// right�Ƃ̘_���a���o��
		flip |= KeyData::RIGHT;
		// �l��Ԃ�
		return flip;
	}
	
	// �E�̃f�[�^����������
	if(flip & KeyData::RIGHT)
	{
		// right������
		flip ^= KeyData::RIGHT;
		// left�Ƃ̘_���a���o��
		flip |= KeyData::LEFT;
		// �l��Ԃ�
		return flip;
	}

	return flip;
}

class CommandData
{
	public:
		CommandData()
		{
			for (int i = 0; i < CommandSize; i++)
			{
				m_commands[i] = 0;
			}
		}
		~CommandData() = default;
		void Input(int command);
		// �R�}���h�̍ő�ۑ���
		static const int CommandSize;
		// ���������̗L���t���[��
		static const int PushFream;
		const int operator[](int i)
		{
			assert(i < CommandSize && 0 <= i && "�z��̃T�C�Y��60(0�`59)�ł�");
			return m_commands[i];
		}
	private:
		int m_commands[60];
};