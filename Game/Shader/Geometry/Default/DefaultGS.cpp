#include "pch.h"
#include "DefaultGS.h"
#include <Game\3rdParty\BinaryFile\BinaryFile.h>
#include <Game\3rdParty\DeviceResources\DeviceResources.h>
#include <Game\Locator\Locator.h>

// �V�F�[�_
Microsoft::WRL::ComPtr<ID3D11GeometryShader> DefaultGS::m_shader;

// �R���X�g���N�^
DefaultGS::DefaultGS()
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
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/DefaultGS.cso");
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_shader.ReleaseAndGetAddressOf())))
	{
		// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
}

void DefaultGS::UpdateCBuffer()
{
	// �f�o�C�X�R���e�L�X�g�擾
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();
	// �萔�o�b�t�@�̔z��擾
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_cbuff, 0, 0);
	//�萔�o�b�t�@���V�F�[�_�ɓn��
	context->GSSetConstantBuffers(0, 1, cb);
	// �W�I���g���V�F�[�_�̃Z�b�g
	context->GSSetShader(m_shader.Get(), nullptr, 0);
}

// �V�F�[�_�̌Ăяo��
ID3D11GeometryShader * DefaultGS::GetShader()
{
	return m_shader.Get();
}

// �萔�o�b�t�@�p�f�[�^�̌Ăяo��
DefaultGS::ConstBuffer& DefaultGS::GetCBuffer()
{
	return m_cbuff;
}
