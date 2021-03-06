#include "GameWindow.hpp"

GameWindow::GameWindow(int width, int height, sf::Font appFont) {
	srand(time(NULL));

	windowWidth = width;
	windowHeight = height;
	font = appFont;
	
	setup();
}

void GameWindow::setup() {
	setupGameover();
	setupSound();
	setupLogic();
	setupExplosions();
	setupUI();
	setupBackground();
	setupEntities();
}

void GameWindow::initializeText(sf::Text& text, sf::Font& font, int textSize, int xPosition, int yPosition, const String& label, const sf::Color& color)
{
	text.setFont(font);
	text.setString(label);
	text.setCharacterSize(textSize);
	text.setFillColor(color);
	text.setPosition(yPosition, xPosition);
}


void GameWindow::setupGameover() {
	gameOverRect = sf::IntRect(0, 0, 1280, 720);

	if (!gameOverTexture.loadFromFile("gameoverspritesheet2.png")) {
		cout << "Couldn't load the background image" << endl;
		exit(1);
	}
	gameOverSprite.setTexture(gameOverTexture);
	gameOverSprite.setPosition(0, 0);
}

void GameWindow::setupSound() {
	if (!shootSoundBuffer1.loadFromFile("flaunch.wav")) {
		cout << "Couldn't load the sound file" << endl;
	}
	shootSound1.setBuffer(shootSoundBuffer1);

	if (!shootSoundBuffer2.loadFromFile("rlaunch.wav")) {
		cout << "Couldn't load the sound file" << endl;
	}
	shootSound2.setBuffer(shootSoundBuffer2);

	shootSound1.setVolume(80);
	shootSound2.setVolume(80);
}

void GameWindow::setupLogic() {
	counter = 0;
	positioner = 592;
	for (int i = 0; i < SPAWN_TRACK_COUNT; i++) {
		spawnTracks[i] = SpawnTrack(positioner, (rand() % 2 + 1), (rand() % 250));
		positioner -= 128;
	}
}

void GameWindow::setupExplosions() {
	explosions = list<Explosion>();

	if (!explosionTexture.loadFromFile("explosionSpritesheet.png")) {
		cout << "Couldn't load the moneybag texture" << endl;
		exit(1);
	}
}

void GameWindow::setupUI() {
	playerHealth = 100;

	if (!lifeBarTexture.loadFromFile("bar_frame100.png")) {
		cout << "Couldn't load the lifebar image" << endl;
	}
	lifeBarSprite.setTexture(lifeBarTexture);
	lifeBarSprite.setPosition(windowWidth - 155, 6);

	points = 0;
	initializeText(pointTotal, font, 25, 10, 15, "SCORE: " + to_string(points), sf::Color::White);
	initializeText(gunpointNotFound, font, 40, (windowHeight / 2) - 50, (windowWidth / 3), "CANNOT LOCATE CONTROLLER", sf::Color::Yellow);
}

void GameWindow::setupBackground() {
	backgroundRect = sf::IntRect(0, 0, 1280, 720);
	backgroundSprite.setTextureRect(backgroundRect);

	if (!backgroundTexture.loadFromFile("bgspritesheet.png")) {
		cout << "Couldn't load the background image" << endl;
		exit(1);
	}

	backgroundSprite.setTexture(backgroundTexture);
}

void GameWindow::setupEntities() {
	setupMonsters();
	setupSupermonsters();
	setupMedpacks();
	setupMoneybags();
}

void GameWindow::setupMonsters() {
	if (!monsterTexture.loadFromFile("monstersSpritesheet.png")) {
		cout << "Couldn't load the monster texture" << endl;
		exit(1);
	}

	sf::Sprite monsterSprite;
	monsterSprite.setTexture(monsterTexture);

	for (int i = 0; i < MONSTER_COUNT; i++) {
		monsters[i] = Monster(1, 10, 10, 60, monsterSprite, "monster_death.wav");
		monsters[i].setCooldown(0 + rand() % 20);
	}
}

void GameWindow::setupSupermonsters() {
	if (!supermonsterTexture.loadFromFile("monstersSpritesheet2.png")) {
		cout << "Couldn't load the super monster texture" << endl;
		exit(1);
	}

	sf::Sprite supermonsterSprite;
	supermonsterSprite.setTexture(supermonsterTexture);
	
	for (int i = 0; i < SUPERMONSTER_COUNT; i++) {
		supermonsters[i] = Monster(2, 20, 20, 60, supermonsterSprite, "supermonster_death.wav");
		supermonsters[i].setCooldown(300 + rand() % 700);
	}
}

void GameWindow::setupMedpacks() {
	if (!medpackTexture.loadFromFile("healthpackSpritesheet.png")) {
		cout << "Couldn't load the medpack texture" << endl;
		exit(1);
	}

	sf::Sprite medpackSprite;
	medpackSprite.setTexture(medpackTexture);

	for (int i = 0; i < MEDPACK_COUNT; i++) {
		medpacks[i] = Monster(1, 0, -20, 60, medpackSprite, "Pop.wav");
		medpacks[i].setCooldown(1500 + rand() % 3500);
	}
}

void GameWindow::setupMoneybags() {
	if (!moneybagTexture.loadFromFile("coinSpritesheet.png")) {
		cout << "Couldn't load the moneybag texture" << endl;
		exit(1);
	}

	sf::Sprite moneybagSprite;
	moneybagSprite.setTexture(moneybagTexture);

	for (int i = 0; i < MONEYBAG_COUNT; i++) {
		moneybags[i] = Monster(1, 100, 0, 60, moneybagSprite, "Pop.wav");
		moneybags[i].setCooldown(2500 + rand() % 4500);
	}
}

void GameWindow::displayBackgroundAndUI(sf::RenderWindow& window) {
	window.clear();
	pointTotal.setString("POINTS: " + to_string(points));
	if(counter++ == 25) {
		if (backgroundRect.left == 1280) {
			backgroundRect.left = 0;
			backgroundRect.top += 720; 
			if (backgroundRect.top == (720 * 2))
				backgroundRect.top += 720;
			if (backgroundRect.top == (720 * 4)) {
				backgroundRect.top = 0;
			}
		}
		else backgroundRect.left += 1280;
		counter = 0;
	}

	backgroundSprite.setTextureRect(this->backgroundRect);
	updateLifeBar();

	window.draw(backgroundSprite);
	window.draw(pointTotal);
	window.draw(lifeBarSprite);
}

void GameWindow::displayGameObjects(sf::RenderWindow& window) {
	for (int i = 0; i < SPAWN_TRACK_COUNT; i++) {
		spawnTracks[i].update(playerHealth);
		spawnTracks[i].draw(window);
	}

	if (explosions.size() > 0) {
		for (std::list<Explosion>::iterator i = explosions.begin(); i != explosions.end(); ++i) {
			if (i->updateFrame()) {
				sf::Sprite temp(explosionTexture);
				temp.setPosition(i->getPositionX(), i->getPositionY());
				temp.setTextureRect(i->getRect());
				window.draw(temp);
			}
			else explosions.erase(i);
		}
	}
}

bool GameWindow::checkForCollisions(WebcamControl& webcamThread) {
	for (int i = 0; i < SPAWN_TRACK_COUNT; i++) {
		for (int j = 0; j < SPAWN_SOCKETS_PER_TRACK; j++) {
			if (spawnTracks[i].sockets[j].checkCollision(webcamThread.getX(), webcamThread.getY(), 30)) {
				int shotEffect1, shotEffect2;
				int posX, posY;

				string name = spawnTracks[i].sockets[j].registerShot(shotEffect1, shotEffect2, posX, posY);
				
				if (posX != -1000 && posY != -1000) {
					Explosion temp(posX, posY);
					explosions.push_back(temp);
				}

				if (shotEffect1 <= 0) playerHealth -= shotEffect1;
				points += shotEffect2;

				if (!deathSoundBuffer.loadFromFile(name)) {
					cout << "Couldn't load the sound file" << endl;
				}
				deathSound.setBuffer(deathSoundBuffer);
				deathSound.play();

				return true;
			}
		}
	}
	return false;
}

void GameWindow::updateEntities() {
	updateMonsters();
	updateSupermonsters();
	updateMoneybags();
	updateMedpacks();
}

void GameWindow::updateMonsters() {
	for (int i = 0; i < MONSTER_COUNT; i++) {
		if (!monsters[i].checkMount() && !monsters[i].hasCooldown()) {
			int memoryLimiter = 0;
			while (true && memoryLimiter < 4) {
				memoryLimiter++;
				SpawnSocket& helper = spawnTracks[rand() % SPAWN_TRACK_COUNT].sockets[rand() % SPAWN_SOCKETS_PER_TRACK];
				if (!helper.checkMount() && helper.isOutOfWindow()) {
					helper.mount(monsters[i]);

					break;
				}
			}
		}
	}
}

void GameWindow::updateSupermonsters() {
	for (int i = 0; i < SUPERMONSTER_COUNT; i++) {
		if (!supermonsters[i].checkMount() && !supermonsters[i].hasCooldown()) {
			int memoryLimiter = 0;
			while (true && memoryLimiter < 4) {
				memoryLimiter++;
				SpawnSocket& helper = spawnTracks[rand() % SPAWN_TRACK_COUNT].sockets[rand() % SPAWN_SOCKETS_PER_TRACK];
				if (!helper.checkMount() && helper.isOutOfWindow()) {
					helper.mount(supermonsters[i]);

					break;
				}
			}
		}
	}
}

void GameWindow::updateMoneybags() {
	for (int i = 0; i < MEDPACK_COUNT; i++) {
		if (!medpacks[i].checkMount() && !medpacks[i].hasCooldown()) {
			int memoryLimiter = 0;
			while (true && memoryLimiter < 4) {
				memoryLimiter++;
				SpawnSocket& helper = spawnTracks[rand() % SPAWN_TRACK_COUNT].sockets[rand() % SPAWN_SOCKETS_PER_TRACK];
				if (!helper.checkMount() && helper.isOutOfWindow()) {
					helper.mount(medpacks[i]);

					break;
				}
			}
		}
	}
}

void GameWindow::updateMedpacks() {
	for (int i = 0; i < MONEYBAG_COUNT; i++) {
		if (!moneybags[i].checkMount() && !moneybags[i].hasCooldown()) {
			int memoryLimiter = 0;
			while (true && memoryLimiter < 4) {
				memoryLimiter++;
				SpawnSocket& helper = spawnTracks[rand() % SPAWN_TRACK_COUNT].sockets[rand() % SPAWN_SOCKETS_PER_TRACK];
				if (!helper.checkMount() && helper.isOutOfWindow()) {
					helper.mount(moneybags[i]);

					break;
				}
			}
		}
	}
}

void GameWindow::handleEvent(sf::Event event, bool& spacePressed, bool& playPressed, WebcamControl& webcamThread) {
	if (event.type == sf::Event::KeyReleased || event.type == sf::Event::MouseButtonPressed) {
		if (event.key.code == sf::Keyboard::Space) {
			playGunshotSound();

			if (checkForCollisions(webcamThread) && !spacePressed) {
				spacePressed = true;
			}
			else if (!spacePressed && playPressed)
			{
				spacePressed = true;
				points -= 1;
			}
		}
		else if (event.key.code == sf::Keyboard::Escape)
		{
			playPressed = false;
		}
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Space) {
			spacePressed = false;
		}
	}
}

bool GameWindow::drawWindow(sf::RenderWindow& window, WebcamControl& webcamThread, sf::Sprite aimSprite, sf::Clock &animationTimer)
{
	if(playerHealth <= 0) return true;
	updateEntities();
	displayBackgroundAndUI(window);
	displayGameObjects(window);

	if (webcamThread.getX() < 0 || webcamThread.getY() < 0) {
		window.draw(gunpointNotFound);
	}
	else {
		window.draw(aimSprite);
	}
	if (playerHealth <= 0) {

		int gameOverCounter = 0;

		while (gameOverRect.top < 5040) {
			gameOverSprite.setTextureRect(gameOverRect);
			window.draw(gameOverSprite);
			window.display();
			if (gameOverCounter++ > 25) {
				gameOverRect.top += 720;
				gameOverCounter = 0;
			}
		}
		return true;
	}
	return false;
}

void GameWindow::playGunshotSound() {
	if (rand() % 2) shootSound1.play();
	else shootSound2.play();
}

void GameWindow::updateLifeBar() {
	std::stringstream textureNameStream;
	String textureName;

	textureNameStream << "bar_frame" << playerHealth << ".png";
	textureNameStream >> textureName;

	if (!lifeBarTexture.loadFromFile(textureName)) {
		cout << "Couldn't load the lifebar image" << endl;
	}
	lifeBarSprite.setTexture(lifeBarTexture);
}
int GameWindow::getPoints() {
	return this->points;
}
