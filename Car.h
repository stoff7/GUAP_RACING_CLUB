#pragma once
#include <SFML/Graphics.hpp>

class Car {
public:
    Car(sf::Texture& t);
    void update(float time, float acceleration);
    sf::Sprite sprite;
    sf::FloatRect getRect() const;  // Метод для получения прямоугольника машины
    void moveX(float offset);       // Метод для перемещения по координате x

private:
    sf::FloatRect rect;
    bool onRoad;
    bool hasCollision;
    float currentFrame;
    float centerStayTime; // Таймер нахождения в центре
    bool inCenter;        // Флаг, указывающий, что машина в центре
    float dx;             // Смещение по оси x
    float dy;             // Смещение по оси y
    const float rectOffset = 15.0f; // Смещение для уменьшения прямоугольника
};
