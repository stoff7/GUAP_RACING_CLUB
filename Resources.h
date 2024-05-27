#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <vector>

class Resources {
public:
    sf::Texture tcar, tRoad, tBackGround, tStartButton, tEndScreen, tSettingsButton, tExitButton, tVolumeScaler, tBackButton, tBackGroundSettings;
    sf::Texture tCar1, tCar2, tCar3, tCar4, tCar5, tCar6, tCar7, tCar8; 
    std::vector<sf::Texture> obstacleTextures;
    sf::Music menuMusic, gameMusic;
    Resources();
};
