#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
private:
    sf::Sprite sprite;
    float speedMultiplier; // ����� ������� ��� ���������� ��������� ��������

public:
    Obstacle(const sf::Texture& texture);

    void setPosition(float x, float y);
    sf::FloatRect getRect() const;
    void update(float time, float acceleration, bool faster);

    sf::Sprite getSprite() const;
    void setRotation(float angle);

    void setSpeedMultiplier(float multiplier); // ����� ����� ��� ��������� ��������� ��������
    float getSpeedMultiplier() const;          // ����� ����� ��� ��������� ��������� ��������
};
