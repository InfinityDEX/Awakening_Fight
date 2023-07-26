#pragma once

#include <Game\Game_Environment\IObject.h>
#include <Game\Interface\IObjectManager.h>

#include <Effects.h>
#include <SpriteBatch.h>
#include <PrimitiveBatch.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include <Model.h>
#include <SimpleMath.h>
#include <list>
#include <VertexTypes.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class GearCursor : public IObject
{
	public:
		struct ConstBuffer
		{
			Matrix		matWorld;
			Matrix		matView;
			Matrix		matProj;
		};
		enum GOING
		{
			Forth = 1,
			Back = -1,
		};
	public:
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
		GearCursor(Vector3* selected);
		~GearCursor();
		virtual void Create();
		virtual void Initialize();
		virtual void Update(DX::StepTimer const& timer) override;
		virtual void Render(const Matrix view, const Matrix proj) override;
		virtual void Reset();
		void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
		void SetSelectedPostion(const Vector3 * const);
	protected:
		std::vector<VertexPositionColorTexture> m_vertexPoints;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		std::unique_ptr<PrimitiveBatch<VertexPositionColorTexture>> m_pBatch;
		std::unique_ptr<SpriteBatch> m_sBatch;
		std::unique_ptr<BasicEffect> m_effect;
		std::unique_ptr<CommonStates> m_state;
	protected:
		float m_time;
		const Vector3* m_selectPositiom;
	private:
		DX::DeviceResources*			m_deviceResources;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
		std::unique_ptr<DirectX::CommonStates> m_states;
		// 頂点シェーダ
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		// ピクセルシェーダ
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		// ジオメトリシェーダ
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;
		std::vector<DirectX::VertexPositionColorTexture>  m_vertex;
		DirectX::SimpleMath::Vector3 m_centerPosition;
		std::unique_ptr<DirectX::EffectFactory> m_fxFactory;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_capture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_rtv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
};