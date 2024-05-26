#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "Car.h"
#include "Obstacle.h"
#include "Resources.h"

using namespace sf;

const float initialSpawnInterval = 8000.0f; // Начальное время между спавнами (в микросекундах)
const float minSpawnInterval = 3500.0f;     // Минимальное время между спавнами (в микросекундах)

int rightBoarder = 1419, leftBoarder = 390;
float acceleration = 0.00001;
const float maxAcceleration = 1.0;
bool onMenu = true, inGame = false, onEndScreen = false;

const int range1_min = 520, range1_max = 890;
const int range2_min = 990, range2_max = 1390;

void createObstacles(std::vector<Obstacle>& obstacles, std::vector<Texture>& textures) {
    int numObstacles = rand() % 3 + 1; // Случайное количество препятствий от 1 до 3

    for (int i = 0; i < numObstacles; ++i) {
        bool validPositionFound = false;
        for (int attempts = 0; attempts < 10; ++attempts) { // Даем до 10 попыток для поиска валидной позиции
            int randomPoint;
            if (rand() % 2 == 0) {
                randomPoint = rand() % (range1_max - range1_min + 1) + range1_min;
            }
            else {
                randomPoint = rand() % (range2_max - range2_min + 1) + range2_min;
            }

            int randomIndex = rand() % textures.size();
            Obstacle newObstacle(textures[randomIndex]);
            if (randomPoint < 1050) {
                newObstacle.setRotation(180); // Используем метод setRotation
            }
            newObstacle.setPosition(static_cast<float>(randomPoint), -200);

            // Устанавливаем множитель скорости в зависимости от позиции
            float speedMultiplier;
            if ((randomPoint >= 500 && randomPoint <= 665) || (randomPoint >= 1250 && randomPoint <= 1390)) {
                speedMultiplier = 1.2f;
            }
            else {
                speedMultiplier = 1.5f;
            }
            newObstacle.setSpeedMultiplier(speedMultiplier);

            bool intersects = false;
            for (const auto& obstacle : obstacles) {
                if (newObstacle.getRect().intersects(obstacle.getRect())) {
                    intersects = true;
                    break;
                }
            }

            if (!intersects) {
                obstacles.push_back(newObstacle);
                validPositionFound = true;
                break;
            }
        }

        if (!validPositionFound) {
            break;
        }
    }
}


void removeOffscreenObstacles(std::vector<Obstacle>& obstacles) {
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle& obstacle) {
        return obstacle.getRect().top > 1080;
        }), obstacles.end());
}

bool checkCollision(const Car& car, const std::vector<Obstacle>& obstacles) {
    sf::FloatRect carRect = car.getRect();
    for (const auto& obstacle : obstacles) {
        if (carRect.intersects(obstacle.getRect())) {
            return true;
        }
    }
    return false;
}

int main() {
    RenderWindow window(VideoMode(1920, 1080), "GUAP RACING CLUB!");

    Clock clock;
    Resources resources;

    Sprite road, startButton, backGround, endScreen;
    startButton.setPosition(680, 270);
    startButton.setTexture(resources.tStartButton);
    startButton.setScale(0.8, 0.8);
    backGround.setPosition(0, 0);
    backGround.setTexture(resources.tBackGround);
    endScreen.setPosition(0, 0);
    endScreen.setTexture(resources.tEndScreen); // Установка текстуры для энд скрина

    std::vector<Obstacle> obstacles;

    Car car(resources.tcar);
    car.sprite.setPosition(560, 770);

    float CurrentFrame = 0;
    road.setPosition(0, 0);
    road.setTexture(resources.tRoad);

    srand(static_cast<unsigned int>(time(0)));

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (onMenu && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePosition = Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    onMenu = false;
                    inGame = true;
                }
            }
            if (inGame && event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                inGame = false;
                onMenu = true;
            }
            if (onEndScreen && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                onEndScreen = false;
                inGame = true;
                obstacles.clear();
                car.sprite.setPosition(560, 770);
                acceleration = 0.00001;
            }
        }

        if (onMenu) {
            window.clear();
            window.draw(backGround);
            window.draw(startButton);
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

            // Линейная интерполяция времени между спавнами в зависимости от ускорения
            float spawnInterval = initialSpawnInterval - ((initialSpawnInterval - minSpawnInterval) * (acceleration / maxAcceleration));

            if (spawnTimer > spawnInterval) {
                createObstacles(obstacles, resources.obstacleTextures);
                spawnTimer = 0;
            }

            for (auto& obstacle : obstacles) {
                float x = obstacle.getSprite().getPosition().x;
                float speedMultiplier = (x >= 500 && x <= 750) || (x >= 1200 && x <= 1400) ? 1.2f : 1.5f;
                obstacle.update(time, acceleration, speedMultiplier);
            }

            removeOffscreenObstacles(obstacles);

            CurrentFrame += 0.006 * time;
            if (CurrentFrame > 3) {
                CurrentFrame -= 3;
            }
            road.setTextureRect(IntRect(0, 500 * int(CurrentFrame), 1920, 1080));

            if (Keyboard::isKeyPressed(Keyboard::A)) {
                car.moveX(-0.9);
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                car.moveX(0.9);
            }

            car.update(time, acceleration);

            if (checkCollision(car, obstacles)) {
                inGame = false;
                onEndScreen = true;
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
            window.draw(endScreen); // Отображение энд скрина
            window.display();
        }
    }

    return 0;
}
