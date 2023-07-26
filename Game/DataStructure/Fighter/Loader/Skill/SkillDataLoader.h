#pragma once
#include <Game\DataStructure\Fighter\Struct\Skill\FighterSkillData.h>

// jsonファイルからスキルデータを取得
std::unique_ptr<SkillRepository> LoadSkillDataFile(char* SkillDataPath);