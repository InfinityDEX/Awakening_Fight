#include "pch.h"
#include "SkillDataLoader.h"
#include <Game\3rdParty\Json\picojson.h>
#include <Game\Controller\CommandData\CommandData.h>
#include <fstream>


std::unique_ptr<SkillRepository> LoadSkillDataFile(char* SkillDataPath)
{
	// 返り値用のデータ
	std::unique_ptr<SkillRepository> skillDatas = std::make_unique<SkillRepository>();

	// ファイル入力ストリーム
	std::ifstream fs;

	// ファイルを開く
	fs.open(SkillDataPath);
	assert(fs);

	// value型で根データを取得
	picojson::value root;
	fs >> root;

	// データ配列に変換
	picojson::array datas = root.get<picojson::array>();

	// データの読み込み
	for (picojson::array::iterator iter = datas.begin(); iter != datas.end(); iter++)
	{
		// スキルデータ
		SkillData skillData;
		/// スキル識別データ
		// 技名
		skillData.SkillNama = iter->get<picojson::object>()
			["SkillName"].get<std::string>();
		// ID
		skillData.AnimationID = iter->get<picojson::object>()
			["AnimationID"].get<double>();
		skillData.Condition = iter->get<picojson::object>()
			["Condition"].get<double>();
		// レバーコマンド
		picojson::array command =
			iter->get<picojson::object>()
			["LeverCommand"].get<picojson::array>();
		for (picojson::array::iterator iter2 = command.begin(); iter2 != command.end(); iter2++)
		{
			int keyData = TenKeytoKeyData(static_cast<int>(iter2->get<double>()));
			skillData.LeverCommand.push_back(keyData);
		}
		// ボタンデータ
		skillData.LastPushButton = iter->get<picojson::object>()
			["LastPushButton"].get<double>();
		// 制限時間
		skillData.TimeLimit = iter->get<picojson::object>()
			["TimeLimit"].get<double>();

		/// 移動データ
		picojson::object movement =
			iter->get<picojson::object>()
			["Movement"].get<picojson::object>();
		// 移動演算タイプ
		skillData.Movement.MoveType = static_cast<MoveMentData::Type>(
			static_cast<int>(
				movement["MoveType"].get<double>()
				)
			);
		// 定数
		skillData.Movement.constant = Vector2(
			movement["Constant"].get<picojson::object>()
			["x"].get<double>(),
			movement["Constant"].get<picojson::object>()
			["y"].get<double>()
		);

		/// 衝撃波データ
		picojson::object impact =
			iter->get<picojson::object>()
			["Impact"].get<picojson::object>();
		// 座標
		skillData.Impact.Position = Vector2(
			impact["Position"].get<picojson::object>()
			["x"].get<double>(),
			impact["Position"].get<picojson::object>()
			["y"].get<double>()
		);
		// 横幅
		skillData.Impact.Width =
			impact["Width"].get<double>();
		// 縦幅
		skillData.Impact.Height =
			impact["Height"].get<double>();
		// 発生(F)
		skillData.Impact.StartUpTime =
			impact["StartUpTime"].get<double>();
		// 持続(F)
		skillData.Impact.ActiveTime =
			impact["ActiveTime"].get<double>();
		// 硬直(F)
		skillData.Impact.RigidityTime =
			impact["RigidityTime"].get<double>();

		/// ダメージデータ
		picojson::object damage =
			impact["Damage"].get<picojson::object>();
		// ダメージタイプ
		skillData.Impact.Damage.DamageType = 
			static_cast<DamageData::Type>(static_cast<int>(damage["DamageType"].get<double>()));
		// ダメージ量
		skillData.Impact.Damage.Damage =
			damage["Damage"].get<double>();
		// ブロック時ダメージ量
		skillData.Impact.Damage.BlockDamage =
			damage["BlockDamage"].get<double>();
		// スタン値
		skillData.Impact.Damage.StunDamage =
			damage["StunDamage"].get<double>();
		// ヒット硬直(F)
		skillData.Impact.Damage.HitStunTime_Difference =
			damage["HitStunTime_Difference"].get<double>();
		// ガード硬直(F)
		skillData.Impact.Damage.GuardStunTime_Difference =
			damage["GuardStunTime_Difference"].get<double>();
		// ヒット衝撃
		skillData.Impact.Damage.Hit_Impact = Vector2(
			damage["Hit_Impact"].get<picojson::object>()
			["x"].get<double>(),
			damage["Hit_Impact"].get<picojson::object>()
			["y"].get<double>()
		);
		// ガード衝撃
		skillData.Impact.Damage.Guard_Impact = Vector2(
			damage["Guard_Impact"].get<picojson::object>()
			["x"].get<double>(),
			damage["Guard_Impact"].get<picojson::object>()
			["y"].get<double>()
		);

		// データを追加する
		skillDatas->push_back(skillData);
	}

	// 作成したデータを送る
	return move(skillDatas);
}
