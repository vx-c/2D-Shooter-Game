// Game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

#include "Player.h"
#include "Renderer.h"
#include "Bullet.h"
#include "Enemy.h"
#include "CollisionManager.h"
#include "EnemyFactory.h"
#include "Background.h"
#include "BulletPattern.h"
#include "Config.h"
#include "Setup.h"
#include "HUD.h"
#include "Boss.h"


int main() {

	sf::RenderWindow window({ config::WINDOW_RESOLUTION[0], config::WINDOW_RESOLUTION[1] }, config::WINDOW_NAME); //Window created
	window.setFramerateLimit(config::FPS_LIMIT); //set framerate limit

	sf::Event event; // sf::Event

	CollisionManager collisionManager; //collision manager setup

//PLAYER SETUP //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//players texture and sprite
	sf::Texture playerTex;
	playerTex.loadFromFile(config::PLAYER_TEXTURE_FILE);
	MySprite playerMySprite(playerTex, config::PLAYER_SIZE, config::PLAYER_SPRITE_LENGTH, config::PLAYER_SPRITE_UPDATE_RATE); //setup the player MySprite

	//Hitbox
	sf::Texture hitboxTex;
	hitboxTex.loadFromFile("Textures/playerHitbox.png");
	sf::Sprite hitboxSprite;
	hitboxSprite.setTexture(hitboxTex);

	//players bullets texture and sprite
	sf::Texture pBulletTex;
	pBulletTex.loadFromFile("Textures/playerBullet.png");
	MySprite pBulletSprite(pBulletTex, { 30,30 }, 1, 1000);

	BulletPattern playerBulletPattern(pBulletSprite, 1, 0.08, 0, 1, 0, patternType::simple, { 105, 75, 130, 50 }, { 70.f, 70.f, 60.f, 60.f }, { -15.f, 15.f, -25.f, 25.f }, { 800, 800, 800, 800 }, { 0.f, 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f, 0.f }); //player bullet pattern

	Player player(playerMySprite, hitboxSprite, config::PLAYER_INITIAL_POSITION, config::PLAYER_UNFOCUSED_SPEED, config::PLAYER_FOCUSED_SPEED, playerBulletPattern); //player setup

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//HUD Sprite and setup
	sf::Texture lifeTexture;
	lifeTexture.loadFromFile("Textures/heart.png");
	MySprite lifeSprite(lifeTexture, { 40,40 }, 1, 1000);

	sf::Texture bombTexture;
	bombTexture.loadFromFile("Textures/bomb.png");
	MySprite bombSprite(bombTexture, { 40, 40 }, 1, 1000);

	HUD hud(lifeSprite, bombSprite, player.GetDescription());

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//WinScreen
	sf::Texture winScreenTex;
	winScreenTex.loadFromFile("Textures/winScreen.png");
	sf::Sprite winScreenSprite;
	winScreenSprite.setTexture(winScreenTex);
	
	//MainMenuScreen
	sf::Texture mainScreenTex;
	mainScreenTex.loadFromFile("Textures/mainMenuScreen.png");
	sf::Sprite mainScreenSprite;
	mainScreenSprite.setTexture(mainScreenTex);

	//LoseScreen
	sf::Texture loseScreenTex;
	loseScreenTex.loadFromFile("Textures/gameOver.png");
	sf::Sprite loseScreenSprite;
	loseScreenSprite.setTexture(loseScreenTex);

	sf::Texture backgroundTex;
	backgroundTex.loadFromFile(config::BACKGROUND_SPRITE_FILE);
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTex);

	Background background(backgroundSprite, config::BACKGROUND_SCROLLSPEED, config::WINDOW_RESOLUTION[1]);
	
	Setup setup; //start setup
	setup.LoadEnemies(); // Load enemies and bosses

	EnemyFactory enemyFactory(setup.GetEnemies(), setup.GetBosses()); //enemy factory setup

	Renderer renderer(window); //renderer setup

	std::vector<Bullet*> bullets; //vector containing our bullets

	std::vector<Enemy*> enemies; //vector containing our enemies

	std::vector<Boss*> bosses; //vector containing our bosses

	sf::Music music;
	music.openFromFile("music.wav");
	music.play();
	music.setLoop(true);

	enum GameState {
		MainMenu,
		MainGame,
		LoseScreen,
		WinScreen
	};


	GameState gameState = MainMenu;
	

	while (true) { //Main loop
		
		while (gameState == MainGame) {	//The main game loop

			
			if (enemyFactory.Remaining() == 0 && enemies.size() == 0 && bosses.size() == 0) { //if all enemies and bosses have been defeated
				gameState = WinScreen;	//change gameState to WinScreen
			}

			if (player.CheckGameOver()) {	//gameover
				gameState = LoseScreen;
			}

			
			while (window.pollEvent(event)) {
				switch (event.type)
				{
				case sf::Event::Closed: //window is closed
					std::cout << "Window closed" << std::endl;
					window.close();

					return 0;

				default:
					break;
				}
			}

			
			//Move objects
			for (std::size_t i = 0; i < bullets.size(); ++i) { //move bullets
				bullets[i]->Move();
			}
			for (std::size_t i = 0; i < enemies.size(); ++i) { //move enemies
				enemies[i]->Move();
			}

			
			window.clear(); //clear the window


			//Create objects
			enemyFactory.MakeEnemies(enemies);
			enemyFactory.MakeBosses(bosses);
			player.Shoot(bullets); //player shoots if pressing Z
			for (std::size_t i = 0; i < enemies.size(); ++i) {
				enemies[i]->Shoot(bullets, { player.GetHitbox().x, player.GetHitbox().y });
			}
			for (std::size_t i = 0; i < bosses.size(); ++i) {
				bosses[i]->Shoot(bullets, { player.GetHitbox().x, player.GetHitbox().y });
			}


			//bomb
			player.Bomb(bullets); //the player bombs if pressing X


			//Check collisions
			for (std::size_t i = 0; i < bullets.size(); ++i) { //check for collisions between bullets and the player
				collisionManager.CheckCollision(*bullets[i], player);
			}
			for (std::size_t i = 0; i < enemies.size(); ++i) { //check for collisions between enemies and the player
				collisionManager.CheckCollision(*enemies[i], player);
			}
			for (std::size_t i = 0; i < bosses.size(); ++i) { //check for collisions between bosses and the player
				collisionManager.CheckCollision(*bosses[i], player);
			}
			for (std::size_t i = 0; i < bullets.size(); ++i) { //check for collisions between bullets and enemies
				for (unsigned int ii = 0; ii < enemies.size(); ++ii) {
					collisionManager.CheckCollision(*bullets[i], *enemies[ii]);
				}
			}
			for (std::size_t i = 0; i < bullets.size(); ++i) { //check for collisions between bullets and the bosses 
				for (unsigned int ii = 0; ii < bosses.size(); ++ii) {
					collisionManager.CheckCollision(*bullets[i], *bosses[ii]);
				}
			}


			//delete destroyed objects
			for (std::vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end();) { //delete destroyed bullets
				if ((*it)->IsDestroyed()) {
					delete *it; *it = nullptr;
					it = bullets.erase(it);
				}
				else {
					it++;
				}
			}
			for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();) { //delete destroyed enemies
				if ((*it)->IsDestroyed()) {
					delete *it;
					*it = nullptr;
					it = enemies.erase(it);
				}
				else {
					it++;
				}
			}
			for (std::vector<Boss*>::iterator it = bosses.begin(); it != bosses.end();) { //delete destroyed bosses
				if ((*it)->Get_isDestroyed()) {
					delete *it;
					*it = nullptr;
					it = bosses.erase(it);
				}
				else {
					it++;
				}
			}

			
			//Updates
			player.Update();
			background.Update();
			hud.Update(player.GetDescription());
			for (std::size_t i = 0; i < bosses.size(); ++i) {
				bosses[i]->Update();
			}


			//update sprites
			for (std::size_t i = 0; i < enemies.size(); ++i) {
				enemies[i]->UpdateSprite();
			}
			

			//render objects
			renderer.Render(background.GetRenderDescription()); //render the background

			for (std::size_t i = 0; i < bosses.size(); ++i) { //render bosses
				renderer.Render(bosses[i]->GetRenderDescription());
			}

			for (std::size_t i = 0; i < enemies.size(); ++i) { //render enemies
				renderer.Render(enemies[i]->GetRenderDescription());
			}

			renderer.Render(player.GetRenderDescription()); //render player
			renderer.Render(player.GetHitboxRenderDescription()); //render player hitbox

			for (std::size_t i = 0; i < bullets.size(); ++i) { //render bullets
				renderer.Render(bullets[i]->GetRenderDescription());
			}

			renderer.Render(hud.GetRenderDescription()); //render the hud


			window.display(); //update the display
		}
		
		
		while (gameState == MainMenu) {	//The main menu loop

			while (window.pollEvent(event)) {
				switch (event.type)
				{
				case sf::Event::Closed: //window is closed
					std::cout << "Window closed" << std::endl;
					window.close();

					return 0;

				default:
					break;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				gameState = MainGame;
				enemyFactory.ResetTimers();	//reset the enemyFactory's timer
			}
			
			window.clear(); //clear the window

			background.Update();

			renderer.Render(background.GetRenderDescription());
			renderer.Render(player.GetRenderDescription());
			renderer.Render(RenderDescription({ 0,0 }, mainScreenSprite));

			window.display(); //update the display
		}
		

		while (gameState == LoseScreen) {	//The lose screen
			
			while (window.pollEvent(event)) {
				switch (event.type)
				{
				case sf::Event::Closed: //window is closed
					std::cout << "Window closed" << std::endl;
					window.close();

					return 0;

				default:
					break;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				gameState = MainGame;
				enemyFactory.ResetQueues(setup.GetEnemies(), setup.GetBosses());
				enemyFactory.ResetTimers();	//reset the enemyFactory's timer
				player.Reset(); //reset the player

				//clear the screen
				bullets.clear();
				enemies.clear();
				bosses.clear();
						
			}

			window.clear(); //clear the window

			//Render objects
			renderer.Render(background.GetRenderDescription()); //render the background
			for (std::size_t i = 0; i < bosses.size(); ++i) { //render bosses
				renderer.Render(bosses[i]->GetRenderDescription());
			}
			for (std::size_t i = 0; i < enemies.size(); ++i) { //render enemies
				renderer.Render(enemies[i]->GetRenderDescription());
			}
			renderer.Render(player.GetRenderDescription()); //render player
			renderer.Render(player.GetHitboxRenderDescription()); //render player hitbox
			for (std::size_t i = 0; i < bullets.size(); ++i) { //render bullets
				renderer.Render(bullets[i]->GetRenderDescription());
			}
			renderer.Render(hud.GetRenderDescription()); //render the hud
			renderer.Render(RenderDescription({ 0,0 }, loseScreenSprite));

			window.display(); //update the display
		}
		

		while (gameState == WinScreen) {	//the WinScren

			while (window.pollEvent(event)) {
				switch (event.type)
				{
				case sf::Event::Closed: //window is closed
					std::cout << "Window closed" << std::endl;
					window.close();

					return 0;

				default:
					break;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				gameState = MainGame;
				enemyFactory.ResetQueues(setup.GetEnemies(), setup.GetBosses());
				enemyFactory.ResetTimers();	//reset the enemyFactory's timer
				player.Reset(); //reset the player

				//clear the screen
				bullets.clear();
				enemies.clear();
				bosses.clear();
			}

			window.clear(); //clear the window

			//Render objects
			renderer.Render(background.GetRenderDescription()); //render the background
			for (std::size_t i = 0; i < bosses.size(); ++i) { //render bosses
				renderer.Render(bosses[i]->GetRenderDescription());
			}
			for (std::size_t i = 0; i < enemies.size(); ++i) { //render enemies
				renderer.Render(enemies[i]->GetRenderDescription());
			}
			renderer.Render(player.GetRenderDescription()); //render player
			renderer.Render(player.GetHitboxRenderDescription()); //render player hitbox
			for (std::size_t i = 0; i < bullets.size(); ++i) { //render bullets
				renderer.Render(bullets[i]->GetRenderDescription());
			}
			renderer.Render(hud.GetRenderDescription()); //render the hud
			renderer.Render(RenderDescription({ 0,0 }, winScreenSprite));

			window.display(); //update the display
		}
	}

    return 1;	//we shouldn't get here
}

