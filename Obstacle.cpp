#include "Obstacle.h"

Obstacle::Obstacle(sf::Texture& t) : dx(0), dy(1), onRoad(true), currentFrame(0) {
    sprite.setTexture(t);
    rect = sf::FloatRect(0, 0, 109, 206);
    sprite.setOrigin(rect.width / 2, rect.height / 2);
}

void Obstacle::update(float time, float acceleration) {
    rect.top += dy * time * acceleration;
    sprite.setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2); // Центрирование позиции
}

void Obstacle::setPosition(float x, float y) {
    rect.left = x;
    rect.top = y;
    sprite.setPosition(x, y);
}

sf::FloatRect Obstacle::getRect() const {
    return sf::FloatRect(rect.left, rect.top, rect.width, rect.height);
}
