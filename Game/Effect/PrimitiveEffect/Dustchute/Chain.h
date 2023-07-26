#pragma once
#include <Game\Effect\DefaultEffect.h>
#include <Game\3rdParty\DeviceResources.h>

class Chain : public EffectInterface
{
public:
	Chain(Vector3 start, Vector3 end, int num);
	~Chain() = default;
	void Create(DX::DeviceResources* deviceResources) override;
	void Update(DX::StepTimer timer) override;
	void Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj);
	void* GetBuffer() override { return &m_cbuff; };
	void Lapse();
private:
	//それぞれのクラスが固有の定数バッファを持つ
	struct ConstBuffer
	{
		Matrix		matWorld;
		Matrix		matView;
		Matrix		matProj;
		Vector4		Time;
		Vector4		Width;
		Vector4		ChainNum;
		Vector4		StartPos;
		Vector4		EndPos;
		Vector4		Color;
	};

	ConstBuffer m_cbuff;
	Vector4 m_subtractColor;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tex;

};