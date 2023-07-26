#pragma once
#include <SimpleMath.h>
#include <Game\Effect\PrimitiveEffect\Split\Split.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class SegmentCounter
{
	public:
		// �R���X�g���N�^
		SegmentCounter();
		// �J�E���^�[�̒l���Z�b�g
		void SetTime(const int time);
	private:
		// �\�̈�
		Split* m_10_Place;
		// ��̈�
		Split* m_1_Place;
		// ���v��
		Split* m_clockBoard;
};