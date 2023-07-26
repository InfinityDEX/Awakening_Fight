#pragma once
#include <Game\Controller\Interface\IController.h>
#include <Game\Controller\CommandData\CommandData.h>
#include <Game\Scene\Interface\IScene.h>
#include <GeometricPrimitive.h>
#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <memory>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class ResultScene :	public IScene
{
	public:
		enum Judge
		{
			P1,
			P2,
			Draw,
		};
	public:
		ResultScene(Judge judge);
		~ResultScene() override;

		// ����������
		void initialize() override;

		// ��������
		virtual void Create() override;

		// �X�V����
		void Update(DX::StepTimer const& timer) override;

		// �`�揈��
		void Render() override;

		// �I������
		void Finalize() override;

	private:
		std::unique_ptr<CommonStates> m_states;
		std::unique_ptr<SpriteBatch> m_spriteBatch;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgrandIcon;
};

