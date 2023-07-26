#pragma once
#include <Game\Scene\Interface\IScene.h>
#include <Game\Locator\Locator.h>
//#include <Game\Effect\MoveNode.h>
//#include <Game\Effect\GearCursor.h>
#include <GeometricPrimitive.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <memory>
#include <Audio.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class MenuScene :	public IScene
{
public:
	MenuScene();
	~MenuScene() override;

	// ����������
	void initialize() override;

	// ��������
	virtual void Create() override;

	// �X�V����
	void Update(DX::StepTimer const& timer) override;

	// ���X�V����
	void BackUpdate(DX::StepTimer const& timer) override;

	// �`�揈��
	void Render() override;

	// �I������
	void Finalize() override;

private:
	enum Mode
	{
		Training,
		Battle,
		Option
	};

	const float NODE_MOVE_INTERVAL = 0.1f;

	float m_nowTime;

	int m_count;

	bool m_selectStart = false;

	bool m_sceneMove = false;

	bool m_returnTitle = false;

	Mode m_selectMode;
};

