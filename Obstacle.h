#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
private:
    sf::Sprite sprite;
    float speedMultiplier; // Ќовый атрибут дл€ случайного множител€ скорости

public:
    Obstacle(const sf::Texture& texture);

    void setPosition(float x, float y);
    sf::FloatRect getRect() const;
    void update(float time, float acceleration, bool faster);

    sf::Sprite getSprite() const;
    void setRotation(float angle);

    void setSpeedMultiplier(float multiplier); // Ќовый метод дл€ установки множител€ скорости
    float getSpeedMultiplier() const;          // Ќовый метод дл€ получени€ множител€ скорости
};
