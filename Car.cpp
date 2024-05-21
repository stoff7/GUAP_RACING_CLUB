#include "Car.h"

extern int rightBoarder;
extern int leftBoarder;

Car::Car(sf::Texture& t) : dx(0), onRoad(true), hasCollision(false), currentFrame(0) {
    sprite.setTexture(t);
    rect = sf::FloatRect(1000, 0, 109, 206);
}

void Car::update(float time) {
    if ((rect.left + dx * time) < rightBoarder && (rect.left + dx * time) > leftBoarder) {
        onRoad = true;
    }
    else {
        onRoad = false;
    }
    if (onRoad) {
        rect.left += dx * time;
    }

    if (hasCollision) {
        hasCollision = true;
    }

    sprite.setPosition(rect.left, 770);
    dx = 0;
}
