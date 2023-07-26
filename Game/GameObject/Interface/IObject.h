#ifndef IOBJECT
#define IOBJECT

#include <Game\3rdParty\Timer\StepTimer.h>
#include <Game\Locator\Locator.h>
#include <SimpleMath.h>

class ObjectManager;
using namespace DirectX;
using namespace DirectX::SimpleMath;
// ���W�E��]�s��E�ړ��ʂ̃Z�b�g
struct Transform
{
	Vector3 position = Vector3::Zero;
	Vector3 velocity = Vector3::Zero;
	Matrix  rotation = Matrix::Identity;
};

class IObject
{
public:
	IObject() 
	{
		m_alive = true; 
		//Locator::GetCollisionManager();
	};
	virtual ~IObject() {};
	virtual void Create() = 0;
	virtual void Initialize() = 0;
	virtual void Update(DX::StepTimer const& timer) = 0;
	virtual void Render(const Matrix view, const Matrix proj) = 0;
	virtual void Reset() = 0;
	bool CheckDead() { return m_alive; };
	void KillSwitch() { m_alive = false; };
	const Transform* GetTransform() const { return &m_transform; };
	Transform* GetTransform() { return &m_transform; };
	bool m_update = true;

private:
	// ���W�E��]�s��E�ړ��ʂ̃Z�b�g
	Transform m_transform;
	// false�ɂȂ�����I�u�W�F�N�g�͏�����
	bool m_alive = true;
};
#endif