#include "Game.h"

Game::Game() : window(sf::VideoMode(1920, 1080), "GUAP RACING CLUB!"), car(tCar), acceleration(1), onMenu(true), inGame(false), onEndScreen(false), obstacleSpawnTimer(0) {
    tStartButton.loadFromFile("SB.png");
    tCar.loadFromFile("car.png");
    tRoad.loadFromFile("road2.jpg");
    tBackGround.loadFromFile("BG.jpg");
    tPit.loadFromFile("Pit.png");

    startButton.setPosition(680, 270);
    startButton.setTexture(tStartButton);
    startButton.setScale(0.8, 0.8);
    backGround.setPosition(0, 0);
    backGround.setTexture(tBackGround);

    road.setPosition(0, 0);
    road.setTexture(tRoad);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        if (onMenu) {
            window.clear();
            window.draw(backGround);
            window.draw(startButton);
            window.display();
            continue;
        }
        if (inGame) {
            float time = clock.restart().asMicroseconds() / 500.0f;
            update(time);
            generateObstacle();
        }
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (onMenu && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            onMenu = false;
            inGame = true;
        }
        if (inGame && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            inGame = false;
            onMenu = true;
        }
        if (inGame && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
            car.setDx(-0.7f);
        }
        if (inGame && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
            car.setDx(0.7f);
        }
    }
}

void Game::update(float time) {
    acceleration += 0.005f;
    car.update(time);

    for (auto it = obstacles.begin(); it != obstacles.end();) {
        it->update(time);
        if (it->isOffScreen()) {
            it = obstacles.erase(it);
        }
        else {
            ++it;
        }
    }

    float CurrentFrame = 0;
    CurrentFrame += 0.006f * time;
    if (CurrentFrame > 3) CurrentFrame -= 3;
    road.setTextureRect(sf::IntRect(0, 500 * int(CurrentFrame), 1920, 1080));

    window.clear();
    window.draw(road);
    window.draw(car.getSprite());
    for (auto& obstacle : obstacles) {
        window.draw(obstacle.getSprite());
    }
    window.display();
}

void Game::generateObstacle() {
    obstacleSpawnTimer += clock.getElapsedTime().asSeconds();
    if (obstacleSpawnTimer > 4.0f) {
        obstacleSpawnTimer = 0.0f;
        obstacles.emplace_back(tPit, 981, 330);
    }
}
