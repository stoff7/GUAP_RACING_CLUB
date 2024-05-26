#include <SFML/Graphics.hpp>
#include "Car.h"
#include "Obstacle.h"

using namespace sf;

int rightBoarder = 1419, leftBoarder = 390;
float acceleration = 1;
bool onMenu = true, inGame = false, onEndScreen = false;

int main() {
    RenderWindow window(VideoMode(1920, 1080), "GUAP RACING CLUB!");

    Clock clock;
    Texture tcar, tRoad, tBackGround, tStartButton, tCar1;
    tStartButton.loadFromFile("SB.png");
    tcar.loadFromFile("car.png");
    tRoad.loadFromFile("road2.jpg");
    tBackGround.loadFromFile("BG.jpg");
    tCar1.loadFromFile("acetone.png");

    Sprite road, startButton, backGround;
    startButton.setPosition(680, 270);
    startButton.setTexture(tStartButton);
    startButton.setScale(0.8, 0.8);
    backGround.setPosition(0, 0);
    backGround.setTexture(tBackGround);

    Obstacle pit(tCar1);
    pit.sprite.setPosition(981, 330);
    pit.sprite.setScale(0.4, 0.5);

    Car car(tcar);
    car.sprite.setPosition(560, 770);

    float CurrentFrame = 0;
    road.setPosition(0, 0);
    road.setTexture(tRoad);

    while (window.isOpen()) {
        Event event;
        while (onMenu) {
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                // Проверка нажатия мыши
                if (event.type == Event::MouseButtonPressed) {
                    if (event.mouseButton.button == Mouse::Left) {
                        Vector2i mousePosition = Mouse::getPosition(window);
                        // Проверка, попадает ли курсор в границы кнопки
                        if (startButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                            onMenu = false;
                            inGame = true;
                        }
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                    onMenu = false;
                    inGame = true;
                }
            }

            window.clear();
            window.draw(backGround);
            window.draw(startButton);
            window.display();
        }

        while (inGame) {
            acceleration += 0.005;

            float time = clock.getElapsedTime().asMicroseconds();
            time /= 500;
            clock.restart();

            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            CurrentFrame += 0.006 * time;
            if (CurrentFrame > 3) {
                CurrentFrame -= 3;
            }
            road.setTextureRect(IntRect(0, 500 * int(CurrentFrame), 1920, 1080));

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                inGame = false;
                onMenu = true;
            }

            if (Keyboard::isKeyPressed(Keyboard::A)) {
                car.dx = -0.7;
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                car.dx = 0.7;
            }

            car.update(time);
            pit.update(time);

            window.clear();
            window.draw(road);
            window.draw(car.sprite);
            window.draw(pit.sprite);
            window.display();
        }
    }

    return 0;
}
