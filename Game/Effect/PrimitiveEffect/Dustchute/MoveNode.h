#pragma once
#include <Game\Game_Environment\IObject.h>
#include <Effects.h>
#include <SpriteBatch.h>
#include <PrimitiveBatch.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include <Model.h>
#include <SimpleMath.h>
#include <list>
#include <VertexTypes.h>

#include <Game\Interface\IObjectManager.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class MoveIcon : public IObject
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
			Fort =  1,
			Back = -1,
		};
	public:
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
		MoveIcon(Vector2 start, Vector2 end, wchar_t* path, Vector3 scale = Vector3::One, float moveTime = 1);
		~MoveIcon();
		virtual void Create();
		virtual void Initialize();
		virtual void Update(DX::StepTimer const& timer) override;
		virtual void Render(const Matrix view, const Matrix proj) override;
		virtual void Reset();
		void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
		void Route(GOING route) { m_route = route; };
		bool MoveEnd();
		
		// 現在のノードの場所(出発地から目的地までの距離に対する進行度の割合)
		float CurrentLocation();
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
		const Vector3 StartPos;
		const Vector3 EndPos;
		Vector3 m_scale;
		const float MoveTime;
		float m_nowTime;
		float m_ratio;
		GOING m_beforeRoute;
		GOING m_route;
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