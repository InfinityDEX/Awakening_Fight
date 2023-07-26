#pragma once
#include <vector>

// 音声データリスト 
struct SoundData
{
	unsigned int ID;
	wchar_t* path;
};

// リスト作成関数
std::vector<SoundData> CreatSoundDataList(char * SoundDataPath);