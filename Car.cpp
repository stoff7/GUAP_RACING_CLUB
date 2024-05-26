#include "Car.h"

extern int rightBoarder;
extern int leftBoarder;

Car::Car(sf::Texture& t) : dx(0), onRoad(true), hasCollision(false), currentFrame(0), centerStayTime(0), inCenter(false) {
    sprite.setTexture(t);
    rect = sf::FloatRect(1000, 0, 109, 206);
}

void Car::update(float time, float acceleration) {
    float nextPosition = rect.left + dx * time * acceleration;

    // ѕроверка, находитс€ ли машина в центре
    if (nextPosition > 820 && nextPosition < 980) {
        inCenter = true;
        centerStayTime += time;
        // ≈сли машина слишком долго находитс€ в центре, переместить еЄ
        if (centerStayTime > 400) { // 2000 микросекунд - лимит времени
            if (nextPosition > rect.left) {
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
    if (nextPosition < rightBoarder && nextPosition > leftBoarder) {
        onRoad = true;
    }
    else {
        onRoad = false;
    }

    // ѕеремещение машины, если она находитс€ на дороге
    if (onRoad) {
        rect.left += dx * time * acceleration;
    }

    sprite.setPosition(rect.left, 770);
    dx = 0;
}
