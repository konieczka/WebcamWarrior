#pragma once
#include "Menu.hpp"
#include "SliderSFML.hpp"
#include "GenericCHeader.hpp"
#include "Menu.hpp"

class OptionsWindow {
private:
	float currSliderValue;
	float oldSliderValue;

	sf::Text sliderText;
	SliderSFML slider;
public:
	OptionsWindow(int, int, sf::Font&, WebcamControl&);
	void handleEvent(sf::Event, bool&);
	void drawWindow(sf::RenderWindow&, WebcamControl&, Menu&, sf::Sprite&, sf::Text notFoundMessage);
	void initializeText(sf::Text&, sf::Font&, int, int, int, const String&, const sf::Color&);
};