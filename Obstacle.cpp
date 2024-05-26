#include "Obstacle.h"

Obstacle::Obstacle(const sf::Texture& texture) : speedMultiplier(1.0f) { // Инициализация множителя скорости
    sprite.setTexture(texture);
}

void Obstacle::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::FloatRect Obstacle::getRect() const {
    return sprite.getGlobalBounds();
}

void Obstacle::update(float time, float acceleration, bool faster) {
    float finalSpeedMultiplier = speedMultiplier * (faster ? 1.5f : 1.0f);
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
