#include "pch.h"
#include "SoundDataLoader.h"
#include <Game\3rdParty\Json\picojson.h>
#include <fstream>

// ���X�g�쐬�֐�
std::vector<SoundData> CreatSoundDataList(char * SoundDataPath)
{
	// �Ԃ�l�p�̃f�[�^
	std::vector<SoundData> list;

	// �t�@�C�����̓X�g���[��
	std::ifstream fs;

	// �t�@�C�����J��
	fs.open(SoundDataPath);
	assert(fs);

	// value�^�ō��f�[�^���擾
	picojson::value root;
	fs >> root;

	//�t�@�C�������
	fs.close();

	// �f�[�^�z��ɕϊ�
	picojson::array datas = root.get<picojson::array>();

	for (picojson::array::iterator iter = datas.begin(); iter != datas.end(); iter++)
	{
		// ID�̍��ڂ����݂��Ȃ���΁A����̓R�����g�Ȃ̂ŃX�L�b�v
		if (iter->get<picojson::object>()["ID"].is<double>() == true)
		{
			// �T�E���h�f�[�^
			SoundData data;
			// ID�̓o�^
			data.ID = static_cast<unsigned int>(iter->get<picojson::object>()["ID"].get<double>());
			// �p�X�̓o�^
			std::string path = iter->get<picojson::object>()["Path"].get<std::string>();
			// string�^��wchar_t*�^�ɕϊ�
			const size_t newsizew = strlen(path.c_str()) + 1;
			data.path = new wchar_t[newsizew];
			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, data.path, newsizew, path.c_str(), _TRUNCATE);
			//�f�[�^��ǉ�����
			list.push_back(data);
		}
	}

	return list;
}
