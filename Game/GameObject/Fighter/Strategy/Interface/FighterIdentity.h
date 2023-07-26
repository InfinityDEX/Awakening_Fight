#pragma once
#ifndef FIGHTERIDENTITY
#define FIGHTERIDENTITY
#include <SpriteBatch.h>
#include <SimpleMath.h>

#include <Game\3rdParty\Timer\StepTimer.h>
#include <Game\DataStructure\Fighter\Loader\Skill\SkillDataLoader.h>
#include <Game\DataStructure\Fighter\Loader\Status\StatusDataLoader.h>
#include <Game\Controller\CommandData\CommandData.h>
#include <Model.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class FighterFrame;
struct FighterShareData;

// ファイターの個性
class FighterIdentity
{
	public:
		// コンストラクタ
		FighterIdentity() = default;
		// デストラクタ
		~FighterIdentity() = default;
		// 初期化
		virtual void Initialize(FighterShareData* plug);
		// 通常更新処理
		virtual void Update(const DX::StepTimer& timer) = 0;
		// 状態別更新処理
		void StateUpdata(const DX::StepTimer& timer);
		// 現在発動できる技データを算出する(存在しなければnullptrを返す)
		const SkillData* Find_Matched_Skill();
		////////////////////////////////////////////
		//　　　　　　　　　　　　　　　　　　　　　
		//  コマンド判定　　　　　　　　　　　 　　　　　　　　　　　　　　　  
		//　　　　　　　　　　　　　　　　　　　　　
		////////////////////////////////////////////
		// ボタン判定
		bool PushJudgment(int buttom);
		// 連打判定(ボタン単一)
		bool RepeatedlyJudgment(KeyData buttom, int num);
		// レバーコマンド判定
		bool LeverCommandJudgment(const SkillData& data);
		bool LeverCommandJudgment(const std::vector<int>& command, const int& limit);
		////////////////////////////////////////////
		//　　　　　　　　　　　　　　　　　　　　　
		//  Getter　　　　　　　　　　　 　　　　　　　　　　　　　　　  
		//　　　　　　　　　　　　　　　　　　　　　
		////////////////////////////////////////////
		// プラグの取得
		FighterShareData& Plug() { return *m_plug; };
		// 技データの取得
		virtual const SkillRepository& Skills() { return *m_skillDatas; };
		// ステータスデータの取得
		const StatusData& Status() { return *m_statusData; };
		////////////////////////////////////////////
		//　　　　　　　　　　　　　　　　　　　　　
		//  Setter　　　　　　　　　　　 　　　　　　　　　　　　　　　  
		//　　　　　　　　　　　　　　　　　　　　　
		////////////////////////////////////////////
		// 技データファイルの読み込み
		void LoadSkill(char* path) { m_skillDatas = LoadSkillDataFile(path); };
		// ステータスファイルの読み込み
		void LoadStatus(char* path) { m_statusData = LoadStatusDataFile(path); };
		////////////////////////////////////////////
		//　　　　　　　　　　　　　　　　　　　　　
		//  状態関数　　　　　　　　　　　 　　　　　　　　　　　　　　　  
		//　　　　　　　　　　　　　　　　　　　　　
		////////////////////////////////////////////
		// 立ち
		virtual void Stand(const DX::StepTimer& timer) = 0;
		// しゃがみ
		virtual void Squat(const DX::StepTimer& timer) = 0;
		// 空中
		virtual void Fly(const DX::StepTimer& timer) = 0;
	private:
		// コントローラーインターフェース
		FighterShareData* m_plug;
		// 技データ
		std::unique_ptr<SkillRepository> m_skillDatas;
		// ステータスデータ
		std::unique_ptr<StatusData> m_statusData;

		// 連打有効期間
		static const int Repeatedly_coefficient;
};

#endif // !FIGHTERIDENTITY
