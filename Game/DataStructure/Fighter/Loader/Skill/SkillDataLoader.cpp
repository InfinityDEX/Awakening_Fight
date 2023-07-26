#include "pch.h"
#include "SkillDataLoader.h"
#include <Game\3rdParty\Json\picojson.h>
#include <Game\Controller\CommandData\CommandData.h>
#include <fstream>


std::unique_ptr<SkillRepository> LoadSkillDataFile(char* SkillDataPath)
{
	// �Ԃ�l�p�̃f�[�^
	std::unique_ptr<SkillRepository> skillDatas = std::make_unique<SkillRepository>();

	// �t�@�C�����̓X�g���[��
	std::ifstream fs;

	// �t�@�C�����J��
	fs.open(SkillDataPath);
	assert(fs);

	// value�^�ō��f�[�^���擾
	picojson::value root;
	fs >> root;

	// �f�[�^�z��ɕϊ�
	picojson::array datas = root.get<picojson::array>();

	// �f�[�^�̓ǂݍ���
	for (picojson::array::iterator iter = datas.begin(); iter != datas.end(); iter++)
	{
		// �X�L���f�[�^
		SkillData skillData;
		/// �X�L�����ʃf�[�^
		// �Z��
		skillData.SkillNama = iter->get<picojson::object>()
			["SkillName"].get<std::string>();
		// ID
		skillData.AnimationID = iter->get<picojson::object>()
			["AnimationID"].get<double>();
		skillData.Condition = iter->get<picojson::object>()
			["Condition"].get<double>();
		// ���o�[�R�}���h
		picojson::array command =
			iter->get<picojson::object>()
			["LeverCommand"].get<picojson::array>();
		for (picojson::array::iterator iter2 = command.begin(); iter2 != command.end(); iter2++)
		{
			int keyData = TenKeytoKeyData(static_cast<int>(iter2->get<double>()));
			skillData.LeverCommand.push_back(keyData);
		}
		// �{�^���f�[�^
		skillData.LastPushButton = iter->get<picojson::object>()
			["LastPushButton"].get<double>();
		// ��������
		skillData.TimeLimit = iter->get<picojson::object>()
			["TimeLimit"].get<double>();

		/// �ړ��f�[�^
		picojson::object movement =
			iter->get<picojson::object>()
			["Movement"].get<picojson::object>();
		// �ړ����Z�^�C�v
		skillData.Movement.MoveType = static_cast<MoveMentData::Type>(
			static_cast<int>(
				movement["MoveType"].get<double>()
				)
			);
		// �萔
		skillData.Movement.constant = Vector2(
			movement["Constant"].get<picojson::object>()
			["x"].get<double>(),
			movement["Constant"].get<picojson::object>()
			["y"].get<double>()
		);

		/// �Ռ��g�f�[�^
		picojson::object impact =
			iter->get<picojson::object>()
			["Impact"].get<picojson::object>();
		// ���W
		skillData.Impact.Position = Vector2(
			impact["Position"].get<picojson::object>()
			["x"].get<double>(),
			impact["Position"].get<picojson::object>()
			["y"].get<double>()
		);
		// ����
		skillData.Impact.Width =
			impact["Width"].get<double>();
		// �c��
		skillData.Impact.Height =
			impact["Height"].get<double>();
		// ����(F)
		skillData.Impact.StartUpTime =
			impact["StartUpTime"].get<double>();
		// ����(F)
		skillData.Impact.ActiveTime =
			impact["ActiveTime"].get<double>();
		// �d��(F)
		skillData.Impact.RigidityTime =
			impact["RigidityTime"].get<double>();

		/// �_���[�W�f�[�^
		picojson::object damage =
			impact["Damage"].get<picojson::object>();
		// �_���[�W�^�C�v
		skillData.Impact.Damage.DamageType = 
			static_cast<DamageData::Type>(static_cast<int>(damage["DamageType"].get<double>()));
		// �_���[�W��
		skillData.Impact.Damage.Damage =
			damage["Damage"].get<double>();
		// �u���b�N���_���[�W��
		skillData.Impact.Damage.BlockDamage =
			damage["BlockDamage"].get<double>();
		// �X�^���l
		skillData.Impact.Damage.StunDamage =
			damage["StunDamage"].get<double>();
		// �q�b�g�d��(F)
		skillData.Impact.Damage.HitStunTime_Difference =
			damage["HitStunTime_Difference"].get<double>();
		// �K�[�h�d��(F)
		skillData.Impact.Damage.GuardStunTime_Difference =
			damage["GuardStunTime_Difference"].get<double>();
		// �q�b�g�Ռ�
		skillData.Impact.Damage.Hit_Impact = Vector2(
			damage["Hit_Impact"].get<picojson::object>()
			["x"].get<double>(),
			damage["Hit_Impact"].get<picojson::object>()
			["y"].get<double>()
		);
		// �K�[�h�Ռ�
		skillData.Impact.Damage.Guard_Impact = Vector2(
			damage["Guard_Impact"].get<picojson::object>()
			["x"].get<double>(),
			damage["Guard_Impact"].get<picojson::object>()
			["y"].get<double>()
		);

		// �f�[�^��ǉ�����
		skillDatas->push_back(skillData);
	}

	// �쐬�����f�[�^�𑗂�
	return move(skillDatas);
}
