#pragma once
#include <Game\Scene\Interface\IScene.h>
#include <Game\Effect\PrimitiveEffect\Wave\TitleBackGrandEffect.h>
#include <GeometricPrimitive.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <memory>
#include <Audio.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class TitleScene :	public IScene
{
	public:
		TitleScene();
		~TitleScene() override;

		// ����������
		void initialize() override;

		// ��������
		virtual void Create() override;

		// �X�V����
		void Update(DX::StepTimer const& timer) override;

		// ���X�V
		void BackUpdate(DX::StepTimer const& timer) override;

		// �`�揈��
		void Render() override;

		// �I������
		void Finalize() override;
	private:
		TitleBackGrandEffect* m_backGrand;
		Vector4 m_backColor;
};

