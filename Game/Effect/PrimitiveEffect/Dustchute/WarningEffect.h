#pragma once
#include <Game\Effect\DefaultEffect.h>
#include <Game\3rdParty\DeviceResources.h>

class WarningEffect : public EffectInterface
{
public:
	WarningEffect();
	~WarningEffect() = default;
	void Create(DX::DeviceResources* deviceResources) override;
	void Update(DX::StepTimer timer) override;
	void Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj);
	void* GetBuffer() override { return &m_cbuff; };
	void Kill() { m_killSwitch = true; };
private:
	//それぞれのクラスが固有の定数バッファを持つ
	struct ConstBuffer
	{
		Matrix		matWorld;
		Matrix		matView;
		Matrix		matProj;
		Vector4		Time;
	};

	ConstBuffer m_cbuff;
	Vector4 m_subtractColor;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
};