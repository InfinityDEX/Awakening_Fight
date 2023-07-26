#pragma once


#include <Game\3rdParty\StepTimer.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <Game\Effect\Interface\EffectInterface.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class FlashEffect : public EffectInterface
{
	public:
		FlashEffect(int life, Vector3 pos, wchar_t* path);
		virtual void Update(DX::StepTimer timer);
		virtual void Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj);
		virtual void Create(DX::DeviceResources* deviceResources);

		std::vector<VertexPositionColorTexture>* GetVertexs() { return &m_vertex; };
		virtual void* GetBuffer() { return &m_cbuff; };

	private:
		struct ConstBuffer
		{
			Matrix		matWorld;
			Matrix		matView;
			Matrix		matProj;
			Vector4		Time;
			Vector4		Colors;
		};
		ConstBuffer m_cbuff;

		int	m_life;
		wchar_t* m_path;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
};