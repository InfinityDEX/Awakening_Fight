#pragma once
#include <vector>

// �����f�[�^���X�g 
struct SoundData
{
	unsigned int ID;
	wchar_t* path;
};

// ���X�g�쐬�֐�
std::vector<SoundData> CreatSoundDataList(char * SoundDataPath);