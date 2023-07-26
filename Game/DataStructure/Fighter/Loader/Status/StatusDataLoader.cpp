#include "pch.h"
#include "StatusDataLoader.h"
#include <fstream>
#include <Game\3rdParty\Json\picojson.h>

std::unique_ptr<StatusData> LoadStatusDataFile(char * StatusDataPath)
{
	// �t�@�C�����̓X�g���[��
	std::ifstream fs;

	// �t�@�C�����J��
	fs.open(StatusDataPath);
	assert(fs);

	// value�^�ō��f�[�^���擾
	picojson::value root;
	fs >> root;

	// object�^�ɕϊ�
	picojson::object rootObject = root.get<picojson::object>();
	
	/// ������ԃX�e�[�^�X
	picojson::object stand = rootObject["Stand"].get<picojson::object>();
	// ������Ԃ̑̂̃T�C�Y
	Vector2 stand_Hit_Box = Vector2(
		stand["Hit_box"].get<picojson::object>()
		["x"].get<double>(),
		stand["Hit_box"].get<picojson::object>()
		["y"].get<double>()
	);

	/// ���Ⴊ�ݏ�ԃX�e�[�^�X
	picojson::object squat = rootObject["Squat"].get<picojson::object>();
	// ���Ⴊ�ݏ�Ԃ̑̂̃T�C�Y
	Vector2 squat_Hit_Box = Vector2(
		squat["Hit_box"].get<picojson::object>()
		["x"].get<double>(),
		squat["Hit_box"].get<picojson::object>()
		["y"].get<double>()
	);

	/// �󒆏�ԃX�e�[�^�X
	picojson::object aerial = rootObject["Aerial"].get<picojson::object>();
	// �󒆏�Ԃ̑̂̃T�C�Y
	Vector2 aerial_Hit_Box = Vector2(
		aerial["Hit_box"].get<picojson::object>()
		["x"].get<double>(),
		aerial["Hit_box"].get<picojson::object>()
		["y"].get<double>()
	);
	return std::make_unique<StatusData>(
		stand_Hit_Box,
		squat_Hit_Box,
		aerial_Hit_Box
	);
}
