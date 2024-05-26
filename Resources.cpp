#include "Resources.h"

Resources::Resources() {
    // �������� ������� ��� ����
    tStartButton.loadFromFile("SB.png");
    tcar.loadFromFile("car.png");
    tRoad.loadFromFile("road2.jpg");
    tBackGround.loadFromFile("BG.jpg");
    tEndScreen.loadFromFile("ES.png"); // �������� �������� ��� ��� ������
    tCar1.loadFromFile("acetone.png");
    tCar2.loadFromFile("Ferrari.png");
    tCar3.loadFromFile("citroen.png");

    // ������������� ������� ������� ��� �����������
    obstacleTextures = { tCar1, tCar2, tCar3 };
}
