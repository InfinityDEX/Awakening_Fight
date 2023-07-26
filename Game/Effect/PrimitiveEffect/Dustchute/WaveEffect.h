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
class WaveEffect : public EffectInterface
{
public:
	WaveEffect(int life, Vector3 pos, Vector3 vel, Vector4 color = Vector4(1, 1, 1, 1));
	virtual void Update(DX::StepTimer timer);
	virtual void Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj);
	virtual void Create(DX::DeviceResources* deviceResources);

	//Vector3 GetPosition() { return m_position; };
	std::vector<VertexPositionColorTexture>* GetVertexs() { return &m_vertex; };
	virtual void* GetBuffer() { return &m_cbuff; };

	// �F�����̕ύX
	void ChangeColor(Vector4 color);
	bool Dead() { return m_killSwitch; };

protected:
	int	m_life;
	Vector3		m_position;
	Vector3		m_velocity;
	std::vector<DirectX::VertexPositionColorTexture>  m_vertex;
	std::unique_ptr<DirectX::CommonStates> m_states;

	// ���_�V�F�[�_�����͋���

	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;

	// �L���X�C�b�`
	bool m_killSwitch = false;
private:

	//���ꂼ��̃N���X���ŗL�̒萔�o�b�t�@������
	struct ConstBuffer
	{
		Matrix		matWorld;
		Matrix		matView;
		Matrix		matProj;
		Vector4		Time;
		Vector4		Color;
	};
	ConstBuffer m_cbuff;
	Vector4		m_color;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
};