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
	// エフェクトの消去
	DeleteEffect();

	// エフェクトの個別更新
	EffectUpdate(timer);

	// エフェクトの追加
	AddEffect();
}

void EffectManager::Render(DrawingOrder side, Matrix world, Matrix view, Matrix proj)
{
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	for (auto effect : m_effectList[side])
	{
		// 描画
		effect->Render(world, view, proj);

		//他のエフェクトに影響が出る可能性があるので使い終わったらシェーダを外す
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
	// すべてのエフェクトを削除する
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
	// エフェクト削除
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
	// アップデート
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
