#pragma once
#ifndef OBJECTMANAGER
#define OBJECTMANAGER
#include <Game\3rdParty\Timer\StepTimer.h>
#include <SimpleMath.h>
#include <vector>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class IObject;
class ObjectManager final
{
	public:
		ObjectManager();
		~ObjectManager();
		void Initialize();
		void Update(DX::StepTimer const& timer);
		void Render(const Matrix view, const Matrix proj);
		void Finalize();
		void ObjectManager::Register(IObject * object){m_addObjects.push_back(object);}
	private:
		void AddObject();
		std::vector<IObject*>	m_objects;
		std::vector<IObject*>	m_addObjects;
};

#endif
