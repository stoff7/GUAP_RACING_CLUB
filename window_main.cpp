#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "Car.h"
#include "Obstacle.h"

using namespace sf;

int rightBoarder = 1419, leftBoarder = 390;
float acceleration = 0.00001;
const float maxAcceleration = 1.0;
bool onMenu = true, inGame = false, onEndScreen = false;

// Диапазоны точек появления
const int range1_min = 500, range1_max = 800;
const int range2_min = 1100, range2_max = 1400;

// Функция для создания новых препятствий
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
            if (randomPoint < 1100) {
                newObstacle.sprite.setRotation(180);
            }
            newObstacle.setPosition(static_cast<float>(randomPoint), -200);

            // Проверка пересечения с существующими препятствиями
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

        // Если не нашли валидную позицию, продолжаем к следующему препятствию
        if (!validPositionFound) {
            break;
        }
    }
}

// Функция для удаления препятствий, которые вышли за границу экрана
void removeOffscreenObstacles(std::vector<Obstacle>& obstacles) {
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle& obstacle) {
        return obstacle.getRect().top > 1080;  // Используем метод getRect
        }), obstacles.end());
}

// Функция для проверки столкновения
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
    Texture tcar, tRoad, tBackGround, tStartButton, tCar1, tCar2, tCar3;
    tStartButton.loadFromFile("SB.png");
    tcar.loadFromFile("car.png");
    tRoad.loadFromFile("road2.jpg");
    tBackGround.loadFromFile("BG.jpg");
    tCar1.loadFromFile("acetone.png");
    tCar2.loadFromFile("Ferrari.png"); // Загрузка дополнительных текстур
    tCar3.loadFromFile("citroen.png");

    std::vector<Texture> obstacleTextures = { tCar1, tCar2, tCar3 }; // Вектор текстур для препятствий

    Sprite road, startButton, backGround;
    startButton.setPosition(680, 270);
    startButton.setTexture(tStartButton);
    startButton.setScale(0.8, 0.8);
    backGround.setPosition(0, 0);
    backGround.setTexture(tBackGround);

    std::vector<Obstacle> obstacles; // Вектор для хранения объектов препятствий

    Car car(tcar);
    car.sprite.setPosition(560, 770);

    float CurrentFrame = 0;
    road.setPosition(0, 0);
    road.setTexture(tRoad);

    srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел

    while (window.isOpen()) {
        Event event;
        while (onMenu) {
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                // Проверка нажатия мыши
                if (event.type == Event::MouseButtonPressed) {
                    if (event.mouseButton.button == Mouse::Left) {
                        Vector2i mousePosition = Mouse::getPosition(window);
                        // Проверка, попадает ли курсор в границы кнопки
                        if (startButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                            onMenu = false;
                            inGame = true;
                        }
                    }
                }
            }

            window.clear();
            window.draw(backGround);
            window.draw(startButton);
            window.display();
        }

        while (inGame) {
            if (acceleration < maxAcceleration) {
                acceleration += 0.000010;
            }

            float time = clock.getElapsedTime().asMicroseconds();
            time /= 400;
            clock.restart();

            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            // Появление новых препятствий с интервалом времени
            static float spawnTimer = 0;
            spawnTimer += time;
            if (spawnTimer > 3333) { // Каждые 3.33 секунды 
                createObstacles(obstacles, obstacleTextures);
                spawnTimer = 0;
            }

            // Обновление и отрисовка препятствий
            for (auto& obstacle : obstacles) {
                obstacle.update(time, acceleration); // Обновляем с учетом ускорения
            }

            // Удаление препятствий, которые ушли за границу экрана
            removeOffscreenObstacles(obstacles);

            CurrentFrame += 0.006 * time;
            if (CurrentFrame > 3) {
                CurrentFrame -= 3;
            }
            road.setTextureRect(IntRect(0, 500 * int(CurrentFrame), 1920, 1080));

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                inGame = false;
                onMenu = true;
            }

            if (Keyboard::isKeyPressed(Keyboard::A)) {
                car.moveX(-0.7);
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                car.moveX(0.7);
            }

            car.update(time, acceleration);

            // Проверка столкновений
            if (checkCollision(car, obstacles)) {
                inGame = false;
                onEndScreen = true;
            }

            window.clear();
            window.draw(road);
            window.draw(car.sprite);
            for (const auto& obstacle : obstacles) {
                window.draw(obstacle.sprite);
            }
            window.display();
        }

        while (onEndScreen) {
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                // Проверка нажатия клавиши для перезапуска игры
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                    onEndScreen = false;
                    onMenu = true;
                    obstacles.clear();
                    car.sprite.setPosition(560, 770);
                    acceleration = 0.00001;
                }
            }

            window.clear();
            window.draw(backGround);
            window.draw(startButton);
            window.display();
        }
    }

    return 0;
}
