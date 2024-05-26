#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    Obstacle(sf::Texture& t);
    void update(float time, float acceleration);
    sf::Sprite sprite;

    void setPosition(float x, float y); // Метод установки позиции
    sf::FloatRect getRect() const;     // Метод получения прямоугольника

private:
    sf::FloatRect rect;
    float dx;
    float dy;
    bool onRoad;
    float currentFrame;
};
