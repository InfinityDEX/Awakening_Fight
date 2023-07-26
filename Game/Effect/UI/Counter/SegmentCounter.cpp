#include "pch.h"
#include "SegmentCounter.h"
#include <Game\Locator\Locator.h>

// �R���X�g���N�^
SegmentCounter::SegmentCounter()
{
	// �J�E���^�[�̉摜���擾
	wchar_t * path = L"Resources\\Sprite\\UI\\SegmentTimer\\Segment.png";
	// �傫��
	float scale = 0.2f;
	// ���W
	Vector3 position = Vector3(0, 0.79f, 0);
	// �\�̈ʂ̃Z�O�����g���쐬
	m_10_Place = new Split(position + Vector3(-0.5f * scale, 0, 0), Vector3(0.5f, 1, 1) * scale, Vector2(4, 4), path);
	// ��̈ʂ̃Z�O�����g���쐬
	m_1_Place = new Split(position + Vector3(0.5f * scale, 0, 0), Vector3(0.5f, 1, 1) * scale, Vector2(4, 4), path);
	// ���v�ł��쐬
	m_clockBoard = new Split(position, Vector3::One * scale, Vector2(4, 4), path);
	// ���v�ł̃X�v���C�g���W�����߂�
	m_clockBoard->SplitPosition(Vector2(0, 3));
	// �}�l�[�W���[���Ăяo��
	auto manager = Locator::GetEffectManager();
	// �}�l�[�W���[�ɐ�ɕ`�悳�����̂��珇�ɓo�^
	manager->Register(DrawingOrder::Front, m_clockBoard);
	manager->Register(DrawingOrder::Front, m_10_Place);
	manager->Register(DrawingOrder::Front, m_1_Place);
}

// �^�C�}�[���Z�b�g
void SegmentCounter::SetTime(const int time)
{
	// 0�ȉ���0�A99�ȏ��99
	int num = std::min(std::max(time, 0), 99);
	// �\�̈�
	int tenth = num / 10;
	// ��̈�
	int first = num % 10;
	// �\�̈ʂ��Z�b�g
	m_10_Place->SplitPosition(Vector2(tenth % 4, tenth / 4));
	// ��̈ʂ��Z�b�g
	m_1_Place->SplitPosition(Vector2(first % 4, first / 4));
}
