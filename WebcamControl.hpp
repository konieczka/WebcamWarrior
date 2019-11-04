#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <opencv2/opencv.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cstdlib>
#include <cstdio>
#include <ctime>


using namespace cv;
using namespace std;
class WebcamControl
{
public:
    int x;
    int y;
    void run();
    int getX();
    int getY();
};