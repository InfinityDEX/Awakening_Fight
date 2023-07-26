#include "pch.h"
#include "DefaultVS.h"
#include <Game\3rdParty\BinaryFile\BinaryFile.h>
#include <Game\3rdParty\DeviceResources\DeviceResources.h>
#include <Game\Locator\Locator.h>

// ���̓��C�A�E�g
Microsoft::WRL::ComPtr<ID3D11InputLayout> DefaultVS::m_inputLayout;
// �V�F�[�_
Microsoft::WRL::ComPtr<ID3D11VertexShader> DefaultVS::m_shader;
// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> DefaultVS::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// �R���X�g���N�^
DefaultVS::DefaultVS()
{
	// �f�o�C�X���擾����
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	///�@�萔�o�b�t�@���쐬����
	// �o�b�t�@�f�B�X�N
	D3D11_BUFFER_DESC bd;
	// �o�b�t�@�f�B�X�N���N���A
	ZeroMemory(&bd, sizeof(bd));
	// GPU��p���ēǂݏ�������ݒ�
	bd.Usage = D3D11_USAGE_DEFAULT;
	// ConstBuffer�\���̂̃������T�C�Y(Byte�P��)�Ńo�b�t�@�T�C�Y���w�肷��ݒ�
	bd.ByteWidth = sizeof(ConstBuffer);
	// �萔�o�b�t�@�Ƃ��ăp�C�v���C���ɑ���ݒ�
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// CPU�͎g�p���Ȃ��ݒ�
	bd.CPUAccessFlags = 0;
	// �f�B�X�N�����ƂɃo�b�t�@���쐬
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	// �������łɃV�F�[�_���쐬����Ă����炱�̐�͔�΂�
	if (m_shader.Get()) return;

	/// �W�I���g���V�F�[�_���쐬����
	// �W�I���g���V�F�[�_�̃t�@�C���ǂݍ���	
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/DefaultVS.cso");
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_shader.ReleaseAndGetAddressOf())))
	{
		// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �C���v�b�g���C�A�E�g�쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());
}

// �萔�o�b�t�@�̍X�V
void DefaultVS::UpdateCBuffer()
{
	// �f�o�C�X�R���e�L�X�g�擾
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();
	// �萔�o�b�t�@�̔z��擾
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_cbuff, 0, 0);
	//�萔�o�b�t�@���V�F�[�_�ɓn��
	context->VSSetConstantBuffers(0, 1, cb);
	// ���_�V�F�[�_�̃Z�b�g
	context->VSSetShader(m_shader.Get(), nullptr, 0);
}

// �V�F�[�_�[�̎擾
ID3D11VertexShader * DefaultVS::GetShader()
{
	return m_shader.Get();
}

// �C���v�b�g���C�A�E�g����̓A�Z���u���[�X�e�[�W�ɃZ�b�g
void DefaultVS::IASetInputLayout()
{
	// �f�o�C�X�R���e�L�X�g�擾
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	// ���̓��C�A�E�g�𔽉f
	context->IASetInputLayout(m_inputLayout.Get());
}

DefaultVS::ConstBuffer& DefaultVS::GetCBuffer()
{
	return m_cbuff;
}
