#include "pch.h"
#include "GearCursor.h"
#include <WICTextureLoader.h>

#include <Game\3rdParty\BinaryFile.h>
#include "d3d11.h"
#include <Effects.h>

const std::vector<D3D11_INPUT_ELEMENT_DESC> GearCursor::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

GearCursor::GearCursor(Vector3* selected)
{
	m_deviceResources = GameContext<DX::DeviceResources>::Get();
	auto device = m_deviceResources->GetD3DDevice();
	DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Resources\\Sprite\\gear.png", nullptr, m_texture.GetAddressOf());
	DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Resources\\Sprite\\gear.png", nullptr, m_texture2.GetAddressOf());
	m_states = std::make_unique<CommonStates>(device);

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/ParticlePS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());
	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// �v���~�e�B�u�o�b�`�̍쐬
	m_pBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_deviceResources->GetD3DDeviceContext());

	m_state = std::make_unique<CommonStates>(device);

	m_sBatch = std::make_unique<SpriteBatch>(m_deviceResources->GetD3DDeviceContext());

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"Resources");

	m_selectPositiom = selected;
}


GearCursor::~GearCursor()
{
}

void GearCursor::Create()
{
}

void GearCursor::Initialize()
{
	m_time = 0;
	m_transform.position = *m_selectPositiom;
}

void GearCursor::Reset()
{
}

void GearCursor::Update(DX::StepTimer const & timer)
{
	m_time += timer.GetElapsedSeconds();
	m_transform.position = Vector3::Lerp(m_transform.position, *m_selectPositiom, 0.5f);
}

void GearCursor::Render(const Matrix view, const Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_vertex.clear();
	//�G�t�F�N�g�̒��_�̍��W���擾����
	Vector3 pos =  m_transform.position;
	//�擾�������W��o�^����
	VertexPositionColorTexture vertex;
	//vertex = VertexPositionColorTexture(pos, Vector4(vel.x,vel.y,vel.z,1), Vector2(0.0f,3.0f));
	//vertex = VertexPositionColorTexture(pos, Vector4::Zero, Vector2::Zero);
	vertex = VertexPositionColorTexture(Vector3::Zero, Vector4::Zero, Vector2::Zero);
	m_vertex.push_back(vertex);

	float rotationVolume= m_time * 5;

	Matrix  mat = Matrix::Identity;
	Matrix  world = Matrix::Identity;
	world *= Matrix::CreateScale(0.2f);
	world *= Matrix::CreateRotationZ(rotationVolume);
	world *= Matrix::CreateTranslation(pos - Vector3(0.6f, 0, 0));
	Draw(world, mat, mat);

	mat = Matrix::Identity;
	world = Matrix::Identity;
	world *= Matrix::CreateScale(0.2f);
	world *= Matrix::CreateRotationZ(-rotationVolume);
	world *= Matrix::CreateTranslation(pos + Vector3(0.6f, 0, 0));
	Draw(world, mat, mat);

	mat = Matrix::Identity;
	world = Matrix::Identity;
	world *= Matrix::CreateScale(0.1f);
	world *= Matrix::CreateRotationZ(rotationVolume + XMConvertToRadians(15));
	world *= Matrix::CreateTranslation(pos + Vector3(0.4f, 0.2f, 0));
	Draw(world, mat, mat);

	mat = Matrix::Identity;
	world = Matrix::Identity;
	world *= Matrix::CreateScale(0.1f);
	world *= Matrix::CreateRotationZ(-rotationVolume + XMConvertToRadians(15));
	world *= Matrix::CreateTranslation(pos - Vector3(0.4f, 0.2f, 0));
	Draw(world, mat, mat);
}

void GearCursor::Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	//�萔�o�b�t�@�œn���l�̐ݒ�
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();

	Vector2 aspect = Vector2(m_deviceResources->GetOutputSize().right, m_deviceResources->GetOutputSize().bottom);

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�͎Q�Ƃ̂�
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// �J�����O�͔����v���
	context->RSSetState(m_states->CullCounterClockwise());

	//�o�b�`�ɒ��_����n��
	//�萔�o�b�t�@���V�F�[�_�ɓn���i�Ƃ肠����PS�͗v��Ȃ��̂ŃR�����g�A�E�g�j
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//�T���v���[�A�V�F�[�_�A�摜�����ꂼ��o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture2.GetAddressOf());

	//���̓��C�A�E�g�𔽉f
	context->IASetInputLayout(m_inputLayout.Get());

	//m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[2], m_vertex[3]);
	
	m_pBatch->Begin();
	m_pBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex[0], m_vertex.size());
	m_pBatch->End();

	//���̃��f���ɉe�����o��\��������̂Ŏg���I�������V�F�[�_���O��
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void GearCursor::SetSelectedPostion(const Vector3 * const pos)
{
	m_selectPositiom = pos;
}

