#include "Resources.h"

Resources::Resources() {
    // Загрузка текстур для игры
    tStartButton.loadFromFile("startButton.png");
    tPauseButton.loadFromFile("pauseButton.png");
    tPauseMenu.loadFromFile("pauseMenu.png");
    tMenuButton.loadFromFile("menuButton.png");
    tRestartButton.loadFromFile("restartButton.png");
    tcar.loadFromFile("car.png");
    tRoad.loadFromFile("road2.jpg");
    tBackGround.loadFromFile("BG.png");
    tEndScreen.loadFromFile("ES.png"); // Загрузка текстуры для энд скрина
    tCar1.loadFromFile("acetone.png");
    tCar2.loadFromFile("Ferrari.png");
    tCar3.loadFromFile("citroen.png");
    tCar4.loadFromFile("audi.png");
    tCar5.loadFromFile("koenig.png");
    tCar6.loadFromFile("cabri1.png");
    tCar7.loadFromFile("white.png");
    tCar8.loadFromFile("bugga.png");
    tSettingsButton.loadFromFile("settings.png"); // Загрузка текстуры для кнопки настроек
    tExitButton.loadFromFile("exit.png"); // Загрузка текстуры для кнопки выхода
    tVolumeScaler.loadFromFile("volumeScaler.png"); // Загрузка текстуры для ползунка громкости
    tBackButton.loadFromFile("back.png"); // Загрузка текстуры для кнопки назад
    tBackGroundSettings.loadFromFile("BackGroundSettings.png");
    menuMusic.openFromFile("menu.ogg");
    gameMusic.openFromFile("game.ogg");
    // Инициализация вектора текстур для препятствий
    obstacleTextures = { tCar1, tCar2, tCar3, tCar4, tCar5, tCar6, tCar7, tCar8 };
}
