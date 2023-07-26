#include "pch.h"
#include "ObjectManager.h"
#include <Game\GameObject\Interface\IObject.h>

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::Initialize()
{
}

void ObjectManager::Update(DX::StepTimer const& timer)
{
	// �I�u�W�F�N�g�폜
	std::vector<IObject*> tmp;
	for (auto object : m_objects)
	{
		if (!object->CheckDead())
		{
			tmp.push_back(object);
		}
	}
	auto itrNewEnd = std::remove_if(m_objects.begin(), m_objects.end(),
		[](IObject* i)->bool
	{ return !i->CheckDead(); });
	m_objects.erase(itrNewEnd, m_objects.end());
	for (auto object : tmp)
	{
		delete object;
	}
	tmp.clear();
	// �A�b�v�f�[�g

	for (IObject* &object : m_objects)
	{
		object->Update(timer);	
	}
	AddObject();
}

void ObjectManager::Render(const Matrix view, const Matrix proj)
{
	for (IObject* &object : m_objects)
	{
		object->Render(view,proj);
	}
}

void ObjectManager::Finalize()
{
	// �ǉ��\�肾�����I�u�W�F�N�g�폜
	std::for_each(m_addObjects.begin(), m_addObjects.end(), [](IObject* p) { delete p; });
	m_addObjects.clear();

	// �Ǘ����Ă����I�u�W�F�N�g�폜
	std::for_each(m_objects.begin(), m_objects.end(), [](IObject* p) { delete p; });
	m_objects.clear();
}

void ObjectManager::AddObject()
{
	for (IObject* &object : m_addObjects)
	{
		object->Initialize();
		object->Create();
		m_objects.push_back(object);
	}
	m_addObjects.clear();
}
