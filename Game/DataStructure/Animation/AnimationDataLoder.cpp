#include "pch.h"
#include "AnimationDataLoder.h"
#include <fstream>
#include <Game\3rdParty\Json\picojson.h>
#include <WICTextureLoader.h>
#include <Game\Locator\Locator.h>

// アニメーションデータの読み込み
AnimationDataRepository LoadAnimationData(char * AnimationDataPath)
{
	// デバイスを取得
	auto device = Locator::GetDeviceResources()->GetD3DDevice();

	// 返り値
	AnimationDataRepository animedata;
	
	// ファイル入力ストリーム
	std::ifstream fs;

	// ファイルを開く
	fs.open(AnimationDataPath);
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
		AnimationData anime;

		// アニメーションファイルパス
		std::string path = iter->get<picojson::object>()
			["Path"].get<std::string>();
		const size_t newsizew = strlen(path.c_str()) + 1;
		wchar_t *wcstring = new wchar_t[newsizew];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsizew, path.c_str(), _TRUNCATE);
		// テクスチャを作成
		CreateWICTextureFromFile(device, wcstring, nullptr, anime.texture.GetAddressOf());

		// 分割数
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
