#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include <fstream>
#include "Car.h"
#include "Obstacle.h"
#include "Resources.h"

using namespace sf;

const float initialSpawnInterval = 10000.0f;
const float minSpawnInterval = 3500.0f;
int rightBoarder = 1419, leftBoarder = 390;
float acceleration = 0.000005;
const float maxAcceleration = 0.5;

float roadY = 0;
float roadSpeed = 3.0f;

bool onMenu = true, inGame = false, onEndScreen = false, onSettings = false, inPause = false;
bool isDraggingMenu = false, isDraggingGame = false;
float menuVolume = 50.0f, gameVolume = 50.0f;

void saveSettings(float menuVolume, float gameVolume) {
	std::ofstream configFile("settings.cfg");
	if (configFile.is_open()) {
		configFile << "menuVolume " << menuVolume << std::endl;
		configFile << "gameVolume " << gameVolume << std::endl;
		configFile.close();
	}
}

void loadSettings(float& menuVolume, float& gameVolume) {
	std::ifstream configFile("settings.cfg");
	if (configFile.is_open()) {
		std::string settingName;
		while (configFile >> settingName) {
			if (settingName == "menuVolume") {
				configFile >> menuVolume;
			}
			else if (settingName == "gameVolume") {
				configFile >> gameVolume;
			}
		}
		configFile.close();
	}
}

void hoverEffect(Sprite& button, Vector2i mousePosition, float scaleFactor = 1.1f) {
	if (button.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
		button.setScale(scaleFactor, scaleFactor);
	}
	else {
		button.setScale(1.0f, 1.0f);
	}
}

void handleMouseButtonPressed(Event event, Vector2i mousePosition, Sprite& volumeScalerMenu, Sprite& volumeScalerGame, Sprite& Button, bool& isDraggingMenu, bool& isDraggingGame, bool& onSettings, bool& onMenu,float menuVolume, float gameVolume, Resources& resources,bool& inPause) {
	if (event.mouseButton.button == Mouse::Left) {
		if (volumeScalerMenu.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
			isDraggingMenu = true;
		}
		else if (volumeScalerGame.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
			isDraggingGame = true;
		}
		else if (Button.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)) && onSettings) {
			onSettings = false;
			onMenu = true;
			saveSettings(menuVolume, gameVolume);
		}
		else if (Button.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)) && !onSettings) {
			inPause = false;
			inGame = true;
			resources.gameMusic.play();
			saveSettings(menuVolume, gameVolume);
		}
	}
}

void handleMouseButtonReleased(Event event, bool& isDraggingMenu, bool& isDraggingGame) {
	if (event.mouseButton.button == Mouse::Left) {
		isDraggingMenu = false;
		isDraggingGame = false;
	}
}

void handleMouseMoved(Event event, Vector2i mousePosition, bool isDraggingMenu, bool isDraggingGame, float& menuVolume, float& gameVolume, Sprite& volumeScalerMenu, Sprite& volumeScalerGame, Resources& resources, bool Settings) {
	if (isDraggingMenu) {
		if (Settings) {
			float newVolume = std::max(0.f, std::min(100.f, ((mousePosition.x - 750) / 400.0f) * 100.0f));
			menuVolume = newVolume;
			volumeScalerMenu.setPosition(750 + (menuVolume / 100.0f) * 450 - volumeScalerMenu.getGlobalBounds().width / 2, 367);
			resources.menuMusic.setVolume(menuVolume);
			// volumeScalerMenu.setPosition(1295 + (menuVolume / 100.0f) * 450 - volumeScalerMenu.getGlobalBounds().width / 2, 353);
			// volumeScalerGame.setPosition(130 + (gameVolume / 100.0f) * 450 - volumeScalerGame.getGlobalBounds().width / 2, 353)
		}
		else {
			float newVolume = std::max(0.f, std::min(100.f, ((mousePosition.x - 1295) / 400.0f) * 100.0f));
			menuVolume = newVolume;
			volumeScalerMenu.setPosition(1295 + (menuVolume / 100.0f) * 450 - volumeScalerMenu.getGlobalBounds().width / 2, 353);
			resources.menuMusic.setVolume(menuVolume);
		}
	}

	if (isDraggingGame) {
		if (Settings) {
			float newVolume = std::max(0.f, std::min(100.f, ((mousePosition.x - 750) / 400.0f) * 100.0f));
			gameVolume = newVolume;
			volumeScalerGame.setPosition(750 + (gameVolume / 100.0f) * 450 - volumeScalerGame.getGlobalBounds().width / 2, 525);
			resources.gameMusic.setVolume(gameVolume);
		}
		else {
			float newVolume = std::max(0.f, std::min(100.f, ((mousePosition.x - 130) / 400.0f) * 100.0f));
			gameVolume = newVolume;
			volumeScalerGame.setPosition(130 + (gameVolume / 100.0f) * 450 - volumeScalerGame.getGlobalBounds().width / 2, 353);
			resources.gameMusic.setVolume(gameVolume);
		}
	}
}

void handleEscapeKey(Event event, bool& inGame, bool& inPause, Resources& resources) {
	if (event.key.code == Keyboard::Escape) {
		inGame = false;
		inPause = true;
		resources.gameMusic.pause();
	}
}


int main() {
	RenderWindow window(VideoMode(1920, 1080), "GUAP RACING CLUB!");

	loadSettings(menuVolume, gameVolume);

	Clock clock;
	Resources resources;

	Sprite road, startButton, backGround, endScreen, backGroundSettings, pauseMenu, pauseButton,menuButton,restartButton;
	Sprite settingsButton, exitButton, volumeScalerMenu, volumeScalerGame, backButton;

	startButton.setTexture(resources.tStartButton);
	startButton.setPosition(0, 290);

	settingsButton.setTexture(resources.tSettingsButton);
	settingsButton.setPosition(0, 490);

	pauseButton.setTexture(resources.tPauseButton);
	pauseButton.setOrigin(pauseButton.getGlobalBounds().width / 2, pauseButton.getGlobalBounds().height / 2);
	pauseButton.setPosition(960, 390);

	pauseMenu.setTexture(resources.tPauseMenu);
	pauseMenu.setPosition(0, -30);

	exitButton.setTexture(resources.tExitButton);
	exitButton.setPosition(0, 680);

	volumeScalerMenu.setTexture(resources.tVolumeScaler);

	volumeScalerGame.setTexture(resources.tVolumeScaler);

	backButton.setTexture(resources.tBackButton);
	backButton.setOrigin(backButton.getGlobalBounds().width / 2, backButton.getGlobalBounds().height / 2);
	backButton.setScale(0.8f, 0.8f);
	backButton.setPosition(1200 - backButton.getGlobalBounds().width / 2, 780);

	backGround.setPosition(0, 0);
	backGround.setTexture(resources.tBackGround);

	backGroundSettings.setPosition(0, 0);
	backGroundSettings.setTexture(resources.tBackGroundSettings);

	restartButton.setTexture(resources.tStartButton);
	restartButton.setPosition(0, 290);

	menuButton.setTexture(resources.tStartButton);
	menuButton.setPosition(0, 290);

	endScreen.setPosition(0, 0);
	endScreen.setTexture(resources.tEndScreen);

	std::vector<Obstacle> obstacles;

	Car car(resources.tcar);
	car.sprite.setPosition(560, 770);

	float CurrentFrame = 0;
	road.setOrigin(0, 1080);
	road.setPosition(0, 0);
	road.setTexture(resources.tRoad);

	srand(static_cast<unsigned int>(time(0)));

	// Set initial volume levels
	resources.menuMusic.setVolume(menuVolume);
	resources.gameMusic.setVolume(gameVolume);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}

			Vector2i mousePosition = Mouse::getPosition(window);
			if (onMenu) {
				hoverEffect(startButton, mousePosition);
				hoverEffect(settingsButton, mousePosition);
				hoverEffect(exitButton, mousePosition);

				if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
					if (startButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
						onMenu = false;
						inGame = true;
						resources.menuMusic.stop();
						resources.gameMusic.play();
					}
					else if (settingsButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
						onMenu = false;
						onSettings = true;
					}
					else if (exitButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
						window.close();
					}
				}

				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
					onMenu = false;
					inGame = true;
					resources.menuMusic.stop();
					resources.gameMusic.play();
				}
			}
			else if (onSettings) {
				hoverEffect(backButton, mousePosition);
				volumeScalerMenu.setPosition(750 + (menuVolume / 100.0f) * 450 - volumeScalerMenu.getGlobalBounds().width / 2, 367);
				volumeScalerGame.setPosition(750 + (gameVolume / 100.0f) * 450 - volumeScalerGame.getGlobalBounds().width / 2, 525);

				if (event.type == Event::MouseButtonPressed) {
					handleMouseButtonPressed(event, mousePosition, volumeScalerMenu, volumeScalerGame, backButton, isDraggingMenu, isDraggingGame, onSettings, onMenu, menuVolume, gameVolume, resources,inPause);
				}
				else if (event.type == Event::MouseButtonReleased) {
					handleMouseButtonReleased(event, isDraggingMenu, isDraggingGame);
				}
				else if (event.type == Event::MouseMoved) {
					handleMouseMoved(event, mousePosition, isDraggingMenu, isDraggingGame, menuVolume, gameVolume, volumeScalerMenu, volumeScalerGame, resources,onSettings);
				}
			}
			else if (inGame) {
				if (event.type == Event::KeyPressed) {
					handleEscapeKey(event, inGame, inPause, resources);
				}
			}
			else if (inPause) {

				clock.restart();

				hoverEffect(pauseButton, mousePosition);
				volumeScalerMenu.setPosition(1295 + (menuVolume / 100.0f) * 450 - volumeScalerMenu.getGlobalBounds().width / 2, 353);
				volumeScalerGame.setPosition(130 + (gameVolume / 100.0f) * 450 - volumeScalerGame.getGlobalBounds().width / 2, 353);

				if (event.type == Event::MouseButtonPressed) {
					handleMouseButtonPressed(event, mousePosition, volumeScalerMenu, volumeScalerGame, pauseButton, isDraggingMenu, isDraggingGame, onSettings, onMenu, menuVolume, gameVolume, resources,inPause);
				}
				else if (event.type == Event::MouseButtonReleased) {
					handleMouseButtonReleased(event, isDraggingMenu, isDraggingGame);
				}
				else if (event.type == Event::MouseMoved) {
					handleMouseMoved(event, mousePosition, isDraggingMenu, isDraggingGame, menuVolume, gameVolume, volumeScalerMenu, volumeScalerGame, resources, onSettings);
				}
			}
			else if (onEndScreen && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
				onEndScreen = false;
				inGame = true;
				obstacles.clear();
				car.sprite.setPosition(560, 770);
				acceleration = 0.00001;
			}
			else if (onEndScreen && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
				onEndScreen = false;
				resources.gameMusic.play();
				inGame = true;
				obstacles.clear();
				car.sprite.setPosition(560, 770);
				acceleration = 0.00001;
			}

		}

		if (onMenu) {
			if (resources.menuMusic.getStatus() != sf::Music::Playing) {
				resources.menuMusic.play();
			}
			window.clear();
			window.draw(backGround);
			window.draw(startButton);
			window.draw(settingsButton);
			window.draw(exitButton);
			window.display();
		}
		else if (onSettings) {
			window.clear();
			window.draw(backGroundSettings);
			window.draw(volumeScalerMenu);
			window.draw(volumeScalerGame);
			window.draw(backButton);
			window.display();
		}

		else if (inPause)
		{
			window.clear();
			window.draw(road);
			window.draw(car.sprite);
			for (const auto& obstacle : obstacles) {
				window.draw(obstacle.getSprite());
			}
			window.draw(pauseMenu);
			window.draw(volumeScalerMenu);
			window.draw(volumeScalerGame);
			window.draw(pauseButton);
			window.display();
		}

		else if (inGame) {
			if (acceleration < maxAcceleration) {
				acceleration += 0.000010;
			}

			float time = clock.getElapsedTime().asMicroseconds();
			time /= 400;
			clock.restart();

			roadY += roadSpeed * time * acceleration;
			if (roadY >= 1080) {
				roadY -= 1080;
			}
			road.setPosition(0, roadY);

			static float spawnTimer = 0;
			spawnTimer += time;
			float spawnInterval = initialSpawnInterval - ((initialSpawnInterval - minSpawnInterval) * (acceleration / maxAcceleration));

			if (spawnTimer > spawnInterval) {
				Obstacle::createObstacles(obstacles, resources.obstacleTextures, acceleration);
				spawnTimer = 0;
			}

			for (auto& obstacle : obstacles) {
				float speedMultiplier;
				float x = obstacle.getSprite().getPosition().x;
				if ((x >= 500 && x <= 665)) {
					speedMultiplier = 1.5f;
				}
				else if ((x >= 665 && x <= 980)) {
					speedMultiplier = 1.7f;
				}
				else if ((x >= 1250 && x <= 1390)) {
					speedMultiplier = 1.2f;
				}
				else {
					speedMultiplier = 1.2f;
				}
				obstacle.update(time, acceleration, speedMultiplier);
			}

			Obstacle::removeOffscreenObstacles(obstacles);

			CurrentFrame += 0.006 * time;
			if (CurrentFrame > 3) {
				CurrentFrame -= 3;
			}
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				car.moveX(-0.7);
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				car.moveX(0.7);
			}

			car.update(time, acceleration);

			if (Obstacle::checkCollision(car, obstacles)) {
				inGame = false;
				onEndScreen = true;
				resources.gameMusic.stop();
			}

			window.clear();
			window.draw(road);
			window.draw(car.sprite);
			for (const auto& obstacle : obstacles) {
				window.draw(obstacle.getSprite());
			}
			window.display();
		}
		else if (onEndScreen) {
			window.clear();
			window.draw(menuButton);
			window.draw(restartButton);
			window.draw(endScreen);
			window.display();
		}
	}

	saveSettings(menuVolume, gameVolume);

	return 0;
}
