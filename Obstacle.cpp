#include "Obstacle.h"

Obstacle::Obstacle(sf::Texture& t) : dx(0), dy(0.5), onRoad(true), currentFrame(0) {
    sprite.setTexture(t);
    rect = sf::FloatRect(1000, 0, 109, 206);
}

void Obstacle::update(float time) {
    if ((rect.top - dy * time) < 1080 && onRoad) {
        onRoad = false;
    }
    else {
        onRoad = false;
    }
    if (onRoad) {
        rect.top -= dy * time;
    }
    sprite.setPosition(rect.left, rect.top);
}
