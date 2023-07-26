#include "pch.h"
#include "SpritePS.h"
#include <Game\3rdParty\BinaryFile\BinaryFile.h>
#include <Game\3rdParty\DeviceResources\DeviceResources.h>
#include <Game\Locator\Locator.h>

// �V�F�[�_
Microsoft::WRL::ComPtr<ID3D11PixelShader> SpritePS::m_shader;

// �R���X�g���N�^
SpritePS::SpritePS()
{
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

	/// �s�N�Z���V�F�[�_���쐬����
	// �s�N�Z���V�F�[�_�̃t�@�C���ǂݍ���
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Sprite/SpritePS.cso");
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_shader.ReleaseAndGetAddressOf())))
	{
		// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
}

// �萔�o�b�t�@�̍X�V
void SpritePS::UpdateCBuffer()
{
	// �f�o�C�X�R���e�L�X�g�擾
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();
	// �萔�o�b�t�@�̔z��擾
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_cbuff, 0, 0);
	//�萔�o�b�t�@���V�F�[�_�ɓn��
	context->PSSetConstantBuffers(0, 1, cb);
	// �s�N�Z���V�F�[�_�̃Z�b�g
	context->PSSetShader(m_shader.Get(), nullptr, 0);
}

// �V�F�[�_�̎擾
ID3D11PixelShader * SpritePS::GetShader()
{
	return m_shader.Get();
}

// �萔�o�b�t�@�̎擾
SpritePS::ConstBuffer & SpritePS::GetCBuffer()
{
	return m_cbuff;
}
