#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Car.h"

class Obstacle {
private:
    sf::Sprite sprite;
    float speedMultiplier; // ����� ������� ��� ���������� ��������� ��������

public:
    Obstacle(const sf::Texture& texture);

    void setPosition(float x, float y);
    sf::FloatRect getRect() const;
    void update(float time, float acceleration, float speedMultiplier);

    sf::Sprite getSprite() const;
    void setRotation(float angle);

    void setSpeedMultiplier(float multiplier); // ����� ����� ��� ��������� ��������� ��������
    float getSpeedMultiplier() const;          // ����� ����� ��� ��������� ��������� ��������

    static void createObstacles(std::vector<Obstacle>& obstacles, std::vector<sf::Texture>& textures, float acceleration);
    static void removeOffscreenObstacles(std::vector<Obstacle>& obstacles);
    static bool checkCollision(const Car& car, const std::vector<Obstacle>& obstacles);
};
