#pragma once
#include <Game\3rdParty\Timer\StepTimer.h>
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <SimpleMath.h>
#include <Model.h>
#include <list>
#include <map>

// �`�揇
enum DrawingOrder
{
	Front,
	Back
};

class EffectManager final
{
	public:
		// �}�l�[�W���[�̍X�V����
		void Update(const  DX::StepTimer& timer) ;
		// �`�揈��
		void Render(DrawingOrder side, Matrix world, Matrix view, Matrix proj) ;
		// Effect�̒ǉ�
		void Register(DrawingOrder side, EffectInterface* effect) ;
		// ���ׂẴI�u�W�F�N�g�̍폜
		void Reset();
	private:
		// �G�t�F�N�g�̏���
		void DeleteEffect();
		// �G�t�F�N�g�̍X�V
		void EffectUpdate(const DX::StepTimer& timer);
		// �ۗ���Ԃ̃G�t�F�N�g�̒ǉ�
		void AddEffect();
		// �Ǘ����Ă���G�t�F�N�g
		std::map<DrawingOrder, std::list<EffectInterface*>>	m_effectList;
		// �܂��ǉ����Ă��Ȃ��ۗ����̃G�t�F�N�g
		std::list<std::pair<DrawingOrder, EffectInterface*>> m_addEffectList;
};