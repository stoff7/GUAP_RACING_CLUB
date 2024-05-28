#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <vector>

using namespace sf;

class Resources {
public:
    Text gameTimeText, maxTime;
    Texture tRoad, tBackGround, tStartButton, tEndScreen, tSettingsButton, tExitButton, tMenuButton,tRestartButton,tScoreTable;
    Texture tcar, tCar1, tCar2, tCar3, tCar4, tCar5, tCar6, tCar7, tCar8, tVolumeScaler, tBackButton, tBackGroundSettings, tPauseButton, tPauseMenu ;
    std::vector<Texture> obstacleTextures;
    Music menuMusic, gameMusic;
    Font font;
    Resources();
};
