#pragma once
#include <Game\Effect\Interface\EffectInterface.h>


using namespace DirectX;
using namespace DirectX::SimpleMath;

class ChaseEffect : public EffectInterface
{
public:
	ChaseEffect(Vector3 pos, wchar_t * path);
	virtual void Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj);
	virtual void Create(DX::DeviceResources* deviceResources);
	void SetPosition(Vector3 pos);
	virtual void* GetBuffer() { return &m_cbuff; };

protected:

	//それぞれのクラスが固有の定数バッファを持つ
	struct ConstBuffer
	{
		Matrix		matWorld;
		Matrix		matView;
		Matrix		matProj;
		Vector4		Time;
	};

	ConstBuffer m_cbuff;

	wchar_t* m_path;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
};

