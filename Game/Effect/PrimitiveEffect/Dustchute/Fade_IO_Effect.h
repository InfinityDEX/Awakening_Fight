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
class FadeEffect : public EffectInterface
{
public:
	enum GOING
	{
		Fort = 1,
		Back = -1,
	};
public:
	FadeEffect(Vector3 start, Vector3 end, Vector3 vel, float moveTime, Vector3 scale, wchar_t* path);
	virtual void Update(DX::StepTimer timer);
	virtual void Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj);
	virtual void Create(DX::DeviceResources* deviceResources);

	//Vector3 GetPosition() { return m_position; };
	std::vector<VertexPositionColorTexture>* GetVertexs() { return &m_vertex; };
	virtual void* GetBuffer() { return &m_cbuff; };
	void Route(GOING route) { m_route = route; };
	bool MoveEnd();

	bool Dead() { return m_killSwitch; };

protected:
	const float	MoveTime;
	const Vector3 StartPos;
	const Vector3 EndPos;
	Vector3 m_scale;
	float m_nowTime;
	float m_ratio;
	GOING m_beforeRoute;
	GOING m_route;
	std::vector<DirectX::VertexPositionColorTexture>  m_vertex;
	std::unique_ptr<DirectX::CommonStates> m_states;

	// 頂点シェーダだけは共通

	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;

	// キルスイッチ
	bool m_killSwitch = false;
private:

	//それぞれのクラスが固有の定数バッファを持つ
	struct ConstBuffer
	{
		Matrix		matWorld;
		Matrix		matView;
		Matrix		matProj;
		Vector4		thickness;
	};
	ConstBuffer m_cbuff;
	Vector4		m_thickness;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
};