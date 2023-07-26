#include "pch.h"
#include "EffectManager.h"
#include <WICTextureLoader.h>
#include <Game\3rdParty\BinaryFile\BinaryFile.h>
#include "d3d11.h"
#include <Effects.h>
#include <Model.h>
#include <Game\Locator\Locator.h>

void EffectManager::Update(const DX::StepTimer& timer)
{
	// �G�t�F�N�g�̏���
	DeleteEffect();

	// �G�t�F�N�g�̌ʍX�V
	EffectUpdate(timer);

	// �G�t�F�N�g�̒ǉ�
	AddEffect();
}

void EffectManager::Render(DrawingOrder side, Matrix world, Matrix view, Matrix proj)
{
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	for (auto effect : m_effectList[side])
	{
		// �`��
		effect->Render(world, view, proj);

		//���̃G�t�F�N�g�ɉe�����o��\��������̂Ŏg���I�������V�F�[�_���O��
		context->VSSetShader(nullptr, nullptr, 0);
		context->GSSetShader(nullptr, nullptr, 0);
		context->PSSetShader(nullptr, nullptr, 0);
	}
}

void EffectManager::Register(DrawingOrder side, EffectInterface * effect)
{
	m_addEffectList.push_back(std::make_pair(side, effect));
}

void EffectManager::Reset()
{
	// ���ׂẴG�t�F�N�g���폜����
	std::vector<EffectInterface*> tmp;
	for (auto effect : m_effectList[DrawingOrder::Back])
	{
		tmp.push_back(effect);
	}
	auto itrNewEnd = std::remove_if(m_effectList[DrawingOrder::Back].begin(), m_effectList[DrawingOrder::Back].end(),
		[](EffectInterface* i)->bool
	{ return i->Death(); });
	m_effectList[DrawingOrder::Back].erase(itrNewEnd, m_effectList[DrawingOrder::Back].end());

	for (auto effect : m_effectList[DrawingOrder::Front])
	{
		tmp.push_back(effect);
	}
	itrNewEnd = std::remove_if(m_effectList[DrawingOrder::Front].begin(), m_effectList[DrawingOrder::Front].end(),
		[](EffectInterface* i)->bool
	{ return i->Death(); });
	m_effectList[DrawingOrder::Front].erase(itrNewEnd, m_effectList[DrawingOrder::Front].end());

	for (auto effect : tmp)
	{
		delete effect;
	}
	m_effectList[DrawingOrder::Back].resize(0);
	m_effectList[DrawingOrder::Front].resize(0);
	tmp.clear();
}

void EffectManager::DeleteEffect()
{
	// �G�t�F�N�g�폜
	std::vector<EffectInterface*> tmp;
	for (auto effect : m_effectList[DrawingOrder::Back])
	{
		if (effect->Death())
		{
			tmp.push_back(effect);
		}
	}
	auto itrNewEnd = std::remove_if(m_effectList[DrawingOrder::Back].begin(), m_effectList[DrawingOrder::Back].end(),
		[](EffectInterface* i)->bool
	{ return i->Death(); });
	m_effectList[DrawingOrder::Back].erase(itrNewEnd, m_effectList[DrawingOrder::Back].end());

	for (auto effect : m_effectList[DrawingOrder::Front])
	{
		if (effect->Death())
		{
			tmp.push_back(effect);
		}
	}
	itrNewEnd = std::remove_if(m_effectList[DrawingOrder::Front].begin(), m_effectList[DrawingOrder::Front].end(),
		[](EffectInterface* i)->bool
	{ return i->Death(); });
	m_effectList[DrawingOrder::Front].erase(itrNewEnd, m_effectList[DrawingOrder::Front].end());

	for (auto effect : tmp)
	{
		delete effect;
	}
	tmp.clear();
}

void EffectManager::EffectUpdate(const DX::StepTimer& timer)
{
	// �A�b�v�f�[�g
	for (std::list<EffectInterface*>::iterator itr = m_effectList[DrawingOrder::Back].begin(); itr != m_effectList[DrawingOrder::Back].end(); itr++)
	{
		(*itr)->Update(timer);
	}
	for (std::list<EffectInterface*>::iterator itr = m_effectList[DrawingOrder::Front].begin(); itr != m_effectList[DrawingOrder::Front].end(); itr++)
	{
		(*itr)->Update(timer);
	}
}

void EffectManager::AddEffect()
{
	for (auto effectDeta : m_addEffectList)
	{
		effectDeta.second->Initialize();
		m_effectList[effectDeta.first].push_back(effectDeta.second);
	}
	m_addEffectList.clear();
}
