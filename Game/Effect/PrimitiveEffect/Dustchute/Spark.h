#pragma once
#include <Game\Effect\DefaultEffect.h>
#include <Game\Effect\AnimationEffect.h>
#include <Game\3rdParty\DeviceResources.h>

class Spark : public AnimationEffect
{
public:
	Spark(Vector3 position,Vector3 direction, float range, int sparkNum, float speed = 1.0f, float length = 1.0f);
	~Spark() = default;
	void Create(DX::DeviceResources* deviceResources) override;
	void Update(DX::StepTimer timer) override;
	void Setting(DX::DeviceResources* deviceResourcese, Matrix view, Matrix proj);
private:
	// �p�[�e�B�N���������
	void MakeSpark();
	//���ꂼ��̃N���X���ŗL�̒萔�o�b�t�@������
	struct ConstBuffer
	{
		Matrix		matWorld;
		Matrix		matView;
		Matrix		matProj;
		Vector4		Time;
		Vector4		Center;
		Vector4		Length;
	};

	// �Œ�o�b�t�@
	ConstBuffer m_cbuff;
	// ���q�̐��̃J�E���^�[
	int m_sparkNum;
	// ���q���g�U����p�x
	const float m_range;
	// ���˂̎��x�N�g��
	const Vector3 m_direction;
	// �p�[�e�B�N���̈ړ��X�s�[�h
	const float m_speed;
	// ������钷��
	const float m_length;
	// �������Ԃ̃C���^�[�o��
	const int m_interval;
	// �������Ԃ̃C���^�[�o���J�E���^�[
	int m_counter;
	// �ړ��x�N�g��
	std::vector<Vector3> m_velocities;
};