#pragma once
#include <Game\DataStructure\Fighter\Struct\Skill\FighterSkillData.h>

// json�t�@�C������X�L���f�[�^���擾
std::unique_ptr<SkillRepository> LoadSkillDataFile(char* SkillDataPath);