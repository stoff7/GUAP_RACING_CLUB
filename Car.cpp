#include "Car.h"

extern int rightBoarder;
extern int leftBoarder;

Car::Car(sf::Texture& t) : dx(0), dy(0), onRoad(true), hasCollision(false), currentFrame(0), centerStayTime(0), inCenter(false) {
    sprite.setTexture(t);
    rect = sf::FloatRect(1000, 770, 109, 206); // ”бедимс€, что начальна€ позици€ правильна€
}

void Car::update(float time, float acceleration) {
    float nextPositionX = rect.left + dx * time * acceleration;
    float nextPositionY = rect.top + dy * time * acceleration;

    // ѕроверка, находитс€ ли машина в центре
    if (nextPositionX > 820 && nextPositionX < 980) {
        inCenter = true;
        centerStayTime += time;
        // ≈сли машина слишком долго находитс€ в центре, переместить еЄ
        if (centerStayTime > 400) { // 2000 микросекунд - лимит времени
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

    // ќбеспечение нахождени€ машины в пределах границ дороги
    if (nextPositionX < rightBoarder && nextPositionX > leftBoarder) {
        onRoad = true;
    }
    else {
        onRoad = false;
    }

    // ѕеремещение машины, если она находитс€ на дороге
    if (onRoad) {
        rect.left += dx * time * acceleration;
    }

    sprite.setPosition(rect.left, rect.top); // ”бедимс€, что позици€ спрайта обновл€етс€ правильно
    dx = 0;
    dy = 0;
}

sf::FloatRect Car::getRect() const {
    return sf::FloatRect(rect.left + rectOffset, rect.top + rectOffset, rect.width - 2 * rectOffset, rect.height - 2 * rectOffset);
}

void Car::moveX(float offset) {
    dx = offset;
}
