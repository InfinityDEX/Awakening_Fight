#include "pch.h"
#include "AnimationDataLoder.h"
#include <fstream>
#include <Game\3rdParty\Json\picojson.h>
#include <WICTextureLoader.h>
#include <Game\Locator\Locator.h>

// �A�j���[�V�����f�[�^�̓ǂݍ���
AnimationDataRepository LoadAnimationData(char * AnimationDataPath)
{
	// �f�o�C�X���擾
	auto device = Locator::GetDeviceResources()->GetD3DDevice();

	// �Ԃ�l
	AnimationDataRepository animedata;
	
	// �t�@�C�����̓X�g���[��
	std::ifstream fs;

	// �t�@�C�����J��
	fs.open(AnimationDataPath);
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
		AnimationData anime;

		// �A�j���[�V�����t�@�C���p�X
		std::string path = iter->get<picojson::object>()
			["Path"].get<std::string>();
		const size_t newsizew = strlen(path.c_str()) + 1;
		wchar_t *wcstring = new wchar_t[newsizew];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsizew, path.c_str(), _TRUNCATE);
		// �e�N�X�`�����쐬
		CreateWICTextureFromFile(device, wcstring, nullptr, anime.texture.GetAddressOf());

		// ������
		anime.split = Vector2(
			iter->get<picojson::object>()["Split"].get<picojson::object>()
			["x"].get<double>(),
			iter->get<picojson::object>()["Split"].get<picojson::object>()
			["y"].get<double>()
		);

		int count = iter->get<picojson::object>()["ID"].get<double>();
		animedata[count] = anime;
	}
	return animedata;
}
