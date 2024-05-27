#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include <fstream>
#include "Car.h"
#include "Obstacle.h"
#include "Resources.h"

using namespace sf;

const float initialSpawnInterval = 10000.0f;
const float minSpawnInterval = 3500.0f;
int rightBoarder = 1419, leftBoarder = 390;
float acceleration = 0.000005;
const float maxAcceleration = 0.5;

bool onMenu = true, inGame = false, onEndScreen = false, onSettings = false;
bool isDraggingMenu = false, isDraggingGame = false;
float menuVolume = 50.0f, gameVolume = 50.0f;

void saveSettings(float menuVolume, float gameVolume) {
    std::ofstream configFile("settings.cfg");
    if (configFile.is_open()) {
        configFile << "menuVolume " << menuVolume << std::endl;
        configFile << "gameVolume " << gameVolume << std::endl;
        configFile.close();
    }
}

void loadSettings(float& menuVolume, float& gameVolume) {
    std::ifstream configFile("settings.cfg");
    if (configFile.is_open()) {
        std::string settingName;
        while (configFile >> settingName) {
            if (settingName == "menuVolume") {
                configFile >> menuVolume;
            }
            else if (settingName == "gameVolume") {
                configFile >> gameVolume;
            }
        }
        configFile.close();
    }
}

void hoverEffect(Sprite& button, Vector2i mousePosition, float scaleFactor = 1.1f) {
    if (button.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
        button.setScale(scaleFactor, scaleFactor);
    }
    else {
        button.setScale(1.0f, 1.0f);
    }
}

int main() {
    RenderWindow window(VideoMode(1920, 1080), "GUAP RACING CLUB!");

    loadSettings(menuVolume, gameVolume);

    Clock clock;
    Resources resources;

    Sprite road, startButton, backGround, endScreen, backGroundSettings;
    Sprite settingsButton, exitButton, volumeScalerMenu, volumeScalerGame, backButton;

    startButton.setTexture(resources.tStartButton);
    startButton.setPosition(0, 290);

    settingsButton.setTexture(resources.tSettingsButton);
    settingsButton.setPosition(0, 490);

    exitButton.setTexture(resources.tExitButton);
    exitButton.setPosition(0, 680);

    volumeScalerMenu.setTexture(resources.tVolumeScaler);
    volumeScalerMenu.setPosition(750 + (menuVolume / 100.0f) * 450 - volumeScalerMenu.getGlobalBounds().width / 2, 367);

    volumeScalerGame.setTexture(resources.tVolumeScaler);
    volumeScalerGame.setPosition(750 + (gameVolume / 100.0f) * 450 - volumeScalerGame.getGlobalBounds().width / 2, 525);

    backButton.setTexture(resources.tBackButton);
    FloatRect bounds = backButton.getGlobalBounds();
    backButton.setOrigin(bounds.width / 2, bounds.height / 2);
    backButton.setScale(0.8f, 0.8f);
    backButton.setPosition(1200 - backButton.getGlobalBounds().width / 2, 780);

    backGround.setPosition(0, 0);
    backGround.setTexture(resources.tBackGround);

    backGroundSettings.setPosition(0, 0);
    backGroundSettings.setTexture(resources.tBackGroundSettings);

    endScreen.setPosition(0, 0);
    endScreen.setTexture(resources.tEndScreen);

    std::vector<Obstacle> obstacles;

    Car car(resources.tcar);
    car.sprite.setPosition(560, 770);

    float CurrentFrame = 0;
    road.setPosition(0, 0);
    road.setTexture(resources.tRoad);

    srand(static_cast<unsigned int>(time(0)));

    // Set initial volume levels
    resources.menuMusic.setVolume(menuVolume);
    resources.gameMusic.setVolume(gameVolume);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            Vector2i mousePosition = Mouse::getPosition(window);
            if (onMenu) {
                hoverEffect(startButton, mousePosition);
                hoverEffect(settingsButton, mousePosition);
                hoverEffect(exitButton, mousePosition);

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (startButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        onMenu = false;
                        inGame = true;
                        resources.menuMusic.stop();
                        resources.gameMusic.play();
                    }
                    else if (settingsButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        onMenu = false;
                        onSettings = true;
                    }
                    else if (exitButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        window.close();
                    }
                }

                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                    onMenu = false;
                    inGame = true;
                    resources.menuMusic.stop();
                    resources.gameMusic.play();
                }
            }
            else if (onSettings) {
                hoverEffect(backButton, mousePosition);

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (volumeScalerMenu.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        isDraggingMenu = true;
                    }
                    else if (volumeScalerGame.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        isDraggingGame = true;
                    }
                    else if (backButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                        onSettings = false;
                        onMenu = true;
                        saveSettings(menuVolume, gameVolume); // Save settings when returning to menu
                    }
                }

                if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                    isDraggingMenu = false;
                    isDraggingGame = false;
                }

                if (isDraggingMenu && event.type == Event::MouseMoved) {
                    float newVolume = std::max(0.f, std::min(100.f, ((mousePosition.x - 750) / 400.0f) * 100.0f));
                    menuVolume = newVolume;
                    volumeScalerMenu.setPosition(750 + (menuVolume / 100.0f) * 450 - volumeScalerMenu.getGlobalBounds().width / 2, 367);
                    resources.menuMusic.setVolume(menuVolume);
                }

                if (isDraggingGame && event.type == Event::MouseMoved) {
                    float newVolume = std::max(0.f, std::min(100.f, ((mousePosition.x - 750) / 400.0f) * 100.0f));
                    gameVolume = newVolume;
                    volumeScalerGame.setPosition(750 + (gameVolume / 100.0f) * 450 - volumeScalerGame.getGlobalBounds().width / 2, 525);
                    resources.gameMusic.setVolume(gameVolume);
                }
            }
            else if (inGame) {
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                    inGame = false;
                    onMenu = true;
                    resources.menuMusic.stop();
                    resources.gameMusic.play();
                }
            }
            else if (onEndScreen && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                onEndScreen = false;
                inGame = true;
                obstacles.clear();
                car.sprite.setPosition(560, 770);
                acceleration = 0.00001;
            }
        }

        if (onMenu) {
            if (resources.menuMusic.getStatus() != sf::Music::Playing) {
                resources.menuMusic.play();
            }
            window.clear();
            window.draw(backGround);
            window.draw(startButton);
            window.draw(settingsButton);
            window.draw(exitButton);
            window.display();
        }
        else if (onSettings) {
            window.clear();
            window.draw(backGroundSettings);
            window.draw(volumeScalerMenu);
            window.draw(volumeScalerGame);
            window.draw(backButton);
            window.display();
        }
        else if (inGame) {
            if (acceleration < maxAcceleration) {
                acceleration += 0.000010;
            }

            float time = clock.getElapsedTime().asMicroseconds();
            time /= 400;
            clock.restart();

            static float spawnTimer = 0;
            spawnTimer += time;
            float spawnInterval = initialSpawnInterval - ((initialSpawnInterval - minSpawnInterval) * (acceleration / maxAcceleration));

            if (spawnTimer > spawnInterval) {
                Obstacle::createObstacles(obstacles, resources.obstacleTextures, acceleration);
                spawnTimer = 0;
            }

            for (auto& obstacle : obstacles) {
                float speedMultiplier;
                float x = obstacle.getSprite().getPosition().x;
                if ((x >= 500 && x <= 665)) {
                    speedMultiplier = 1;
                }
                else if ((x >= 665 && x <= 980)) {
                    speedMultiplier = 1;
                }
                else if ((x >= 1250 && x <= 1390)) {
                    speedMultiplier = 1;
                }
                else {
                    speedMultiplier = 1;
                }
                obstacle.update(time, acceleration, speedMultiplier);
            }

            Obstacle::removeOffscreenObstacles(obstacles);

            CurrentFrame += 0.006 * time;
            if (CurrentFrame > 3) {
                CurrentFrame -= 3;
            }
            road.setTextureRect(IntRect(0, 500 * int(CurrentFrame), 1920, 1080));

            if (Keyboard::isKeyPressed(Keyboard::A)) {
                car.moveX(-2);
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                car.moveX(2);
            }

            car.update(time, acceleration);

            if (Obstacle::checkCollision(car, obstacles)) {
                inGame = false;
                onEndScreen = true;
                resources.gameMusic.stop();
            }

            window.clear();
            window.draw(road);
            window.draw(car.sprite);
            for (const auto& obstacle : obstacles) {
                window.draw(obstacle.getSprite());
            }
            window.display();
        }
        else if (onEndScreen) {
            window.clear();
            window.draw(endScreen);
            window.display();
        }
    }

    saveSettings(menuVolume, gameVolume);

    return 0;
}
