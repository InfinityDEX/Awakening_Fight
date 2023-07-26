#pragma once
#include <Audio.h>
#include <map>

using namespace DirectX;
class SoundManager final
{
	public:
		// �f�X�g���N�^
		~SoundManager() = default;
		// �T�E���h�f�[�^�̒ǉ�
		void Register(int ID, wchar_t* path);
		// ��񂾂��̌��ʉ�
		void OneShot(int ID);
		// �������[�v
		void Loop(int ID);
		// ���[�v�̒�~
		void LoopStop(int ID);
		// �S�Ẳ����̒�~
		void AllStop(int ID);
		// �{�����[���̒���
		void Volume(int ID, float volume);
		// �I�[�f�B�I�G���W���̎擾
		static const AudioEngine* Engine() { return s_AudioEngine.get(); };
	private:
		// �I�[�f�B�I�G���W��
		static std::unique_ptr<AudioEngine> s_AudioEngine;
		// �����f�[�^�̃��\�[�X
		std::map<int,std::unique_ptr<SoundEffect>> m_soundResources;
		// �����C���X�^���X
		std::map<int, std::unique_ptr<SoundEffectInstance>> m_soundInstances;
};