#include "pch.h"
#include "SoundDataLoader.h"
#include <Game\3rdParty\Json\picojson.h>
#include <fstream>

// リスト作成関数
std::vector<SoundData> CreatSoundDataList(char * SoundDataPath)
{
	// 返り値用のデータ
	std::vector<SoundData> list;

	// ファイル入力ストリーム
	std::ifstream fs;

	// ファイルを開く
	fs.open(SoundDataPath);
	assert(fs);

	// value型で根データを取得
	picojson::value root;
	fs >> root;

	//ファイルを閉じる
	fs.close();

	// データ配列に変換
	picojson::array datas = root.get<picojson::array>();

	for (picojson::array::iterator iter = datas.begin(); iter != datas.end(); iter++)
	{
		// IDの項目が存在しなければ、それはコメントなのでスキップ
		if (iter->get<picojson::object>()["ID"].is<double>() == true)
		{
			// サウンドデータ
			SoundData data;
			// IDの登録
			data.ID = static_cast<unsigned int>(iter->get<picojson::object>()["ID"].get<double>());
			// パスの登録
			std::string path = iter->get<picojson::object>()["Path"].get<std::string>();
			// string型をwchar_t*型に変換
			const size_t newsizew = strlen(path.c_str()) + 1;
			data.path = new wchar_t[newsizew];
			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, data.path, newsizew, path.c_str(), _TRUNCATE);
			//データを追加する
			list.push_back(data);
		}
	}

	return list;
}
