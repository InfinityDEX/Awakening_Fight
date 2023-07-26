#include "pch.h"
#include "StatusDataLoader.h"
#include <fstream>
#include <Game\3rdParty\Json\picojson.h>

std::unique_ptr<StatusData> LoadStatusDataFile(char * StatusDataPath)
{
	// ファイル入力ストリーム
	std::ifstream fs;

	// ファイルを開く
	fs.open(StatusDataPath);
	assert(fs);

	// value型で根データを取得
	picojson::value root;
	fs >> root;

	// object型に変換
	picojson::object rootObject = root.get<picojson::object>();
	
	/// 立ち状態ステータス
	picojson::object stand = rootObject["Stand"].get<picojson::object>();
	// 立ち状態の体のサイズ
	Vector2 stand_Hit_Box = Vector2(
		stand["Hit_box"].get<picojson::object>()
		["x"].get<double>(),
		stand["Hit_box"].get<picojson::object>()
		["y"].get<double>()
	);

	/// しゃがみ状態ステータス
	picojson::object squat = rootObject["Squat"].get<picojson::object>();
	// しゃがみ状態の体のサイズ
	Vector2 squat_Hit_Box = Vector2(
		squat["Hit_box"].get<picojson::object>()
		["x"].get<double>(),
		squat["Hit_box"].get<picojson::object>()
		["y"].get<double>()
	);

	/// 空中状態ステータス
	picojson::object aerial = rootObject["Aerial"].get<picojson::object>();
	// 空中状態の体のサイズ
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
