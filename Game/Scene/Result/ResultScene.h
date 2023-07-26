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

		// ‰Šú‰»ˆ—
		void initialize() override;

		// ¶¬ˆ—
		virtual void Create() override;

		// XVˆ—
		void Update(DX::StepTimer const& timer) override;

		// •`‰æˆ—
		void Render() override;

		// I—¹ˆ—
		void Finalize() override;

	private:
		std::unique_ptr<CommonStates> m_states;
		std::unique_ptr<SpriteBatch> m_spriteBatch;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgrandIcon;
};

