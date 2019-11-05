#pragma once
#include "SpawnSocket.h"

class Entity
{
private:
	int centerX;
	int centerY;
	int hitboxRadius;
	int cooldown;

	bool isMounted;
	SpawnSocket socket;

	sf::Sprite sprite;
	sf::RectangleShape border;
	sf::CircleShape hitbox;
public:
	Entity();
	Entity(int, sf::Sprite&);
	void updatePosition(int, int);
	void drawHitbox(sf::RenderWindow&);
	void draw(sf::RenderWindow&);
	bool checkCollision(int, int, int);
};

