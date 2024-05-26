#include "Car.h"

extern int rightBoarder;
extern int leftBoarder;

Car::Car(sf::Texture& t) : dx(0), dy(0), onRoad(true), hasCollision(false), currentFrame(0), centerStayTime(0), inCenter(false) {
    sprite.setTexture(t);
    rect = sf::FloatRect(1000, 770, 109, 206); // ��������, ��� ��������� ������� ����������
}

void Car::update(float time, float acceleration) {
    float nextPositionX = rect.left + dx * time * acceleration;
    float nextPositionY = rect.top + dy * time * acceleration;

    // ��������, ��������� �� ������ � ������
    if (nextPositionX > 820 && nextPositionX < 980) {
        inCenter = true;
        centerStayTime += time;
        // ���� ������ ������� ����� ��������� � ������, ����������� �
        if (centerStayTime > 400) { // 2000 ����������� - ����� �������
            if (nextPositionX > rect.left) {
                rect.left += 10;
            }
            else {
                rect.left -= 10;
            }
            inCenter = false;
            centerStayTime -= 100;
        }
    }
    else {
        inCenter = false;
        centerStayTime = 0;
    }

    // ����������� ���������� ������ � �������� ������ ������
    if (nextPositionX < rightBoarder && nextPositionX > leftBoarder) {
        onRoad = true;
    }
    else {
        onRoad = false;
    }

    // ����������� ������, ���� ��� ��������� �� ������
    if (onRoad) {
        rect.left += dx * time * acceleration;
    }

    sprite.setPosition(rect.left, rect.top); // ��������, ��� ������� ������� ����������� ���������
    dx = 0;
    dy = 0;
}

sf::FloatRect Car::getRect() const {
    return sf::FloatRect(rect.left + rectOffset, rect.top + rectOffset, rect.width - 2 * rectOffset, rect.height - 2 * rectOffset);
}

void Car::moveX(float offset) {
    dx = offset;
}
