#include "pch.h"
#include "SoundManager.h"

// �I�[�f�B�I�G���W��
std::unique_ptr<AudioEngine> SoundManager::s_AudioEngine;

// �T�E���h�f�[�^�̒ǉ�
void SoundManager::Register(int ID, wchar_t* path)
{
	// �I�[�f�B�I�G���W���̐���
	if(!s_AudioEngine.get())
		s_AudioEngine = std::make_unique<AudioEngine>(AudioEngine_Default);
	// ���\�[�X�̃R�s�[
	m_soundResources[ID] = std::make_unique<SoundEffect>(s_AudioEngine.get(), path);
	// �C���X�^���X�̃R�s�[
	m_soundInstances[ID] = m_soundResources[ID]->CreateInstance();
}

// ��񂾂��̌��ʉ�
void SoundManager::OneShot(int ID)
{
	// ��������x�~�߂�
	m_soundInstances[ID]->Stop();
	// �������Đ�����
	m_soundInstances[ID]->Play();
}

// �������[�v
void SoundManager::Loop(int ID)
{
	// �C���X�^���X�����[�v�Đ�����
	m_soundInstances[ID]->Play(true);
}

// ���[�v�̒�~
void SoundManager::LoopStop(int ID)
{
	// �C���X�^���X���Đ�����
	m_soundInstances[ID]->Stop();
}

// �S�Ẳ����̒�~
void SoundManager::AllStop(int ID)
{
	// �S�Ẳ������~����
	for (int i = 0; i < m_soundInstances.size(); i++)
	{
		m_soundInstances[i]->Stop();
	}
}

// �{�����[���̒���
void SoundManager::Volume(int ID, float volume)
{
	m_soundInstances[ID]->SetVolume(volume);
}
