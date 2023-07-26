#pragma once
#include <assert.h>
// コントローラからの入力を統一する為の列挙体
enum KeyData
{
	// 方向入力
	NEUTRAL		= 0b000000000001, // 中央
	UP			= 0b000000000010, // 上
	DOWN		= 0b000000000100, // 下
	LEFT		= 0b000000001000, // 左
	RIGHT		= 0b000000010000, // 右
	// 攻撃ボタン
	WEAK		= 0b000000100000, // 弱攻撃
	MODERATO	= 0b000001000000, // 中攻撃
	HARD		= 0b000010000000, // 強攻撃
	// システムコマンド
	DICISION	= 0b000100000000, // 決定
	CANCEL		= 0b001000000000, // キャンセル
	ESCAPE		= 0b010000000000, // ゲーム終了
	OPTION		= 0b100000000000, // オプション
	// ボタンすべて
	ANY			= 0b111111100000,
};

// -1か１を入力するとLEFTかRIGHTで返す
inline KeyData DirectionToKeyData(int dir)
{
	assert(dir == -1 || dir == 1 && "要求された値以外の数値が渡されました(要求する値は-1か1です)");
	return static_cast<KeyData>(KeyData::LEFT * (dir == -1) + KeyData::RIGHT * (dir == 1));
}

// LEFTかRIGHTを入力すると-1か１で返す
inline int KeyDataToDirection(KeyData dir)
{
	assert(dir & KeyData::LEFT || dir & KeyData::RIGHT && "要求された値以外の数値が渡されました(要求する値はKeyData::LEFTかKeyData::RIGHTです)");
	return  -1 * (dir == KeyData::LEFT) + 1 * (dir == KeyData::RIGHT);
}

// テンキー式レバー番号をキーデータに変換
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
		assert(!"１～９以外の値が入力されました");
		break;
	}
}

// 左右の入力を反転
inline int Flip_horizontal(int input)
{
	int flip = input;

	// 左のデータがあったら
	if (flip & KeyData::LEFT)
	{
		// leftを消す
		flip ^= KeyData::LEFT;
		// rightとの論理和を出す
		flip |= KeyData::RIGHT;
		// 値を返す
		return flip;
	}
	
	// 右のデータがあったら
	if(flip & KeyData::RIGHT)
	{
		// rightを消す
		flip ^= KeyData::RIGHT;
		// leftとの論理和を出す
		flip |= KeyData::LEFT;
		// 値を返す
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
		// コマンドの最大保存量
		static const int CommandSize;
		// 同時押しの有効フレーム
		static const int PushFream;
		const int operator[](int i)
		{
			assert(i < CommandSize && 0 <= i && "配列のサイズは60(0～59)です");
			return m_commands[i];
		}
	private:
		int m_commands[60];
};