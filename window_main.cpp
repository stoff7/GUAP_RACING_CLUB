#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Car.h"
#include "Obstacle.h"

using namespace sf;

int rightBoarder = 1419, leftBoarder = 390;
float acceleration = 0.00001;
const float maxAcceleration = 1.0;
bool onMenu = true, inGame = false, onEndScreen = false;

// Массив точек появления
const std::vector<int> spawnPoints = { 500, 650, 800, 1100, 1250, 1400 };

// Функция для создания новых препятствий
void createObstacles(std::vector<Obstacle>& obstacles, std::vector<Texture>& textures) {
    int numObstacles = rand() % 3 + 1; // Случайное количество препятствий от 1 до 3
    std::vector<int> availablePoints = spawnPoints;

    for (int i = 0; i < numObstacles; ++i) {
        if (availablePoints.empty()) {
            break;
        }
        int pointIndex = rand() % availablePoints.size();
        int randomPoint = availablePoints[pointIndex];
        availablePoints.erase(availablePoints.begin() + pointIndex);

        int randomIndex = rand() % textures.size();
        Obstacle newObstacle(textures[randomIndex]);
        if (randomPoint < 1100){
            newObstacle.sprite.setRotation(180);
        }
        newObstacle.setPosition(static_cast<float>(randomPoint), -100);  // Используем метод setPosition
        obstacles.push_back(newObstacle);
    }
}

// Функция для удаления препятствий, которые вышли за границу экрана
void removeOffscreenObstacles(std::vector<Obstacle>& obstacles) {
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle& obstacle) {
        return obstacle.getRect().top > 1080;  // Используем метод getRect
        }), obstacles.end());
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
            if (spawnTimer > 5000) { // Каждые 5 секунд
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
                car.dx = -0.7;
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                car.dx = 0.7;
            }

            car.update(time, acceleration);

            window.clear();
            window.draw(road);
            window.draw(car.sprite);
            for (const auto& obstacle : obstacles) {
                window.draw(obstacle.sprite);
            }
            window.display();
        }
    }

    return 0;
}
