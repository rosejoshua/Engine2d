#include "LevelManager.hpp"

LevelManager::LevelManager()
{
    currentLevel = 0;
    currentLevelMap = nullptr;
    backgroundColor = white;
}

LevelManager::~LevelManager()
{
}

bool LevelManager::loadLevel(int levelNum)
{
    bool inError = false;
    currentLevel = levelNum;
    Bitmap bitmap(levelDir + std::to_string(currentLevel) + levelDirEnd);

    return inError;
}

void LevelManager::restartCurrentLevel()
{

}

void LevelManager::unloadLevel()
{
    delete currentLevelMap;
    currentLevelMap = nullptr;
}
