#pragma once
#include <SFML/Graphics.hpp>

class Car {
public:
    Car(sf::Texture& t);
    void update(float time, float acceleration);
    sf::Sprite sprite;
    sf::FloatRect getRect() const;  // ����� ��� ��������� �������������� ������
    void moveX(float offset);       // ����� ��� ����������� �� ���������� x

private:
    sf::FloatRect rect;
    bool onRoad;
    bool hasCollision;
    float currentFrame;
    float centerStayTime; // ������ ���������� � ������
    bool inCenter;        // ����, �����������, ��� ������ � ������
    float dx;             // �������� �� ��� x
    float dy;             // �������� �� ��� y
    const float rectOffset = 15.0f; // �������� ��� ���������� ��������������
};
