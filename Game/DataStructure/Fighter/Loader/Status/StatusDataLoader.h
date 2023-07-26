#pragma once

#include <Game\DataStructure\Fighter\Struct\Status\FighterStatus.h>

std::unique_ptr<StatusData> LoadStatusDataFile(char* StatusDataPath);
