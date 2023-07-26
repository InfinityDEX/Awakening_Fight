#include "pch.h"
#include "Spark.h"
#include <Game\3rdParty\BinaryFile.h>

Spark::Spark(Vector3 position, Vector3 direction, float range, int sparkNum, float speed, float length)
	: AnimationEffect(999, Vector2(1, 1), position, Matrix::Identity, L"Resources\\Sprite\\Particle.png")
	, m_sparkNum(sparkNum)
	, m_range(range)
	, m_direction(direction)
	, m_speed(speed)
	, m_length(length * 10)
	, m_interval(2)
{
	// ���_�C���f�b�N�X��������
	m_vertex.clear();
	// �p�[�e�B�N���������
	MakeSpark();
	// �J�E���^�[�̒l���Z�b�g
	m_counter = m_interval;
	// �G�t�F�N�g�̑傫��������������
	m_scale = Vector3(0.1f);
}

void Spark::Create(DX::DeviceResources * deviceResources)
{
	AnimationEffect::Create(deviceResources);

	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();
	
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/Spark/SparkGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Spark/SparkPS.cso");

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
}

void Spark::Update(DX::StepTimer timer)
{
	// �����J�E���^�[��0�ł��p�[�e�B�N���̎c�ʂ��܂��c���Ă�����
	if (m_counter == 0 && m_sparkNum > 0)
	{
		// �p�[�e�B�N���������
		MakeSpark();
		// �J�E���^�[�̒l���Z�b�g
		m_counter = m_interval;
		// ���q�̃J�E���^�[������炷
		m_sparkNum--;
	}

	// �����z����L��
	std::vector<int> m_deleteMemory;
	// �S�Ẵo�[�e�b�N�X�����
	for (int i = 0; i < m_vertex.size(); i++)
	{
		m_vertex[i].position = m_vertex[i].position + m_velocities[i];

		if (
			Vector3(m_vertex[i].position).Length() > m_length && // ���������ꏊ����w�肵��������藣��Ă��āA
			!(m_sparkNum > 0 && m_vertex.size() == 1)) // �������J�E���^��0�Œ��_�C���f�b�N�X�������Ȃ肻���łȂ����
		{
			// �폜����z��Ƃ��ċL��
			m_deleteMemory.push_back(i);
		}
	}

	// �폜����l�������
	if (m_deleteMemory.size() > 0)
	{
		// �폜�z����\�[�g(����)
		std::sort(m_deleteMemory.begin(), m_deleteMemory.end());
		// ���Ԃɍ폜����(���o�[�X�Ȃ͉̂��̂ق���������Ȃ���΂����Ȃ���)
		for (auto iter = m_deleteMemory.rbegin(); iter < m_deleteMemory.rend(); iter++)
		{
			// ��������p�[�e�B�N���������A�`�悵�Ă������_���Ȃ��Ȃ�����
			if (m_sparkNum == 0 && m_vertex.size() <= m_deleteMemory.size())
			{
				// ���_�C���f�b�N�X�͍폜�����G�t�F�N�g�̃L���X�C�b�`��true�ɂ��ď������I���
				m_killSwitch = true;
				return;
			}
			// ���_�Ƃ���ɑΉ������ړ��x�N�g�����폜
			m_vertex.erase(m_vertex.begin() + (*iter));
			m_velocities.erase(m_velocities.begin() + (*iter));
		}
	}

	// �R���X�g�o�b�t�@�̃^�C�}�[�̒l��ݒ肷��
	m_cbuff.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);

	// �ړ����E������ݒ肷��
	m_cbuff.Length.x = m_length;

	// �J�E���^�[������炷
	m_counter--;
}

void Spark::Setting(DX::DeviceResources * deviceResourcese, Matrix view, Matrix proj)
{
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	ID3D11Buffer* cb[1] = { GetD3DBuffer() };
	auto device = deviceResourcese->GetD3DDevice();
	auto context = deviceResourcese->GetD3DDeviceContext();

	m_cbuff.matWorld = (Matrix::CreateScale(m_scale) * m_rotation * Matrix::CreateTranslation(m_position)).Transpose();
	m_cbuff.matView = view.Transpose();
	m_cbuff.matProj = proj.Transpose();

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(GetD3DBuffer(), 0, NULL, &m_cbuff, 0, 0);
	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�͎Q�Ƃ̂�
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	//// �J�����O�͔����v���
	//context->RSSetState(m_states->CullCounterClockwise());
	// ���ʃJ�����O
	context->RSSetState(m_states->CullNone());
	// �e�N�X�`�����Z�b�g
	context->PSSetShaderResources(0, 1, GetPPTexture());
	//�萔�o�b�t�@���V�F�[�_�ɓn���i�Ƃ肠����PS�͗v��Ȃ��̂ŃR�����g�A�E�g�j
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// ���ʃJ�����O
	context->PSSetSamplers(0, 1, sampler);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}

void Spark::MakeSpark()
{
	m_vertex.push_back(VertexPositionColorTexture(Vector3::Zero, Vector4::Zero, Vector2::Zero));

	float radius = static_cast<float>(rand() % static_cast<int>(m_range * 100)) / 100;

	radius -= m_range / 2;
	Vector3 vec;

	vec.x = cos(radius) * m_direction.x - sin(radius) * m_direction.y;
	vec.y = sin(radius) * m_direction.x + cos(radius) * m_direction.y;
	m_velocities.push_back(vec * m_speed);
}
