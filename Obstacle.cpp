#include "Obstacle.h"
#include <cstdlib>
#include <algorithm>

const int range1_min = 480, range1_max = 900;
const int range2_min = 1000, range2_max = 1420;

Obstacle::Obstacle(const sf::Texture& texture) : speedMultiplier(1.0f) { // »нициализаци€ множител€ скорости
    sprite.setTexture(texture);
}

void Obstacle::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::FloatRect Obstacle::getRect() const {
    return sprite.getGlobalBounds();
}

void Obstacle::update(float time, float acceleration, float speedMultiplier) {
    float finalSpeedMultiplier = this->speedMultiplier * speedMultiplier;
    sprite.move(0, time * acceleration * finalSpeedMultiplier);
}

sf::Sprite Obstacle::getSprite() const {
    return sprite;
}

void Obstacle::setRotation(float angle) {
    sprite.setRotation(angle);
}

void Obstacle::setSpeedMultiplier(float multiplier) {
    speedMultiplier = multiplier;
}

float Obstacle::getSpeedMultiplier() const {
    return speedMultiplier;
}

void Obstacle::createObstacles(std::vector<Obstacle>& obstacles, std::vector<sf::Texture>& textures, float acceleration) {
    int numObstacles = rand() % 3 + 1; // —лучайное количество преп€тствий от 1 до 3

    for (int i = 0; i < numObstacles; ++i) {
        bool validPositionFound = false;
        for (int attempts = 0; attempts < 10; ++attempts) { // ƒаем до 10 попыток дл€ поиска валидной позиции
            int randomPoint;
            if (rand() % 2 == 0) {
                randomPoint = rand() % (range1_max - range1_min + 1) + range1_min;
            }
            else {
                randomPoint = rand() % (range2_max - range2_min + 1) + range2_min;
            }

            int randomIndex = rand() % textures.size();
            Obstacle newObstacle(textures[randomIndex]);
            if (randomPoint < 1000) {
                newObstacle.setRotation(180); // »спользуем метод setRotation
            }
            newObstacle.setPosition(static_cast<float>(randomPoint), -200);

            // ”станавливаем множитель скорости в зависимости от позиции
            float speedMultiplier;
            if ((randomPoint >= 500 && randomPoint <= 665)) {
                speedMultiplier = 2.2f;
            }
            else if ((randomPoint >= 665 && randomPoint <= 980)) {
                speedMultiplier = 2.5f;
            }
            else if ((randomPoint >= 1250 && randomPoint <= 1390))
            {
                speedMultiplier = 1.5f;
            }
            else
            {
                speedMultiplier = 1.2f;
            };
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

void Obstacle::removeOffscreenObstacles(std::vector<Obstacle>& obstacles) {
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle& obstacle) {
        return obstacle.getRect().top > 1080;
        }), obstacles.end());
}

bool Obstacle::checkCollision(const Car& car, const std::vector<Obstacle>& obstacles) {
    sf::FloatRect carRect = car.getRect();
    for (const auto& obstacle : obstacles) {
        if (carRect.intersects(obstacle.getRect())) {
            return true;
        }
    }
    return false;
}
