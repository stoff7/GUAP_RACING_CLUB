#include "Resources.h"

Resources::Resources() {
    // Загрузка текстур для игры
    tStartButton.loadFromFile("SB.png");
    tcar.loadFromFile("car.png");
    tRoad.loadFromFile("road2.jpg");
    tBackGround.loadFromFile("BG.jpg");
    tEndScreen.loadFromFile("ES.png"); // Загрузка текстуры для энд скрина
    tCar1.loadFromFile("acetone.png");
    tCar2.loadFromFile("Ferrari.png");
    tCar3.loadFromFile("citroen.png");

    // Инициализация вектора текстур для препятствий
    obstacleTextures = { tCar1, tCar2, tCar3 };
}
