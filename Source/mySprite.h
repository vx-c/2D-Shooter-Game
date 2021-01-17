#pragma once

#include <SFML/Graphics.hpp>

class MySprite
{
public:
//	MySprite(sf::Sprite &sprite, sf::Vector2i spriteSize, int maxSprites, float updateRate);
	MySprite(sf::Texture &texture, sf::Vector2i spriteSize,int end, float updateRate);
	MySprite(sf::Texture &texture, sf::Vector2i spriteSize,int start, int end, int row, float updateRate);
	MySprite();
	~MySprite();

	void Next();
	void ChangeRow(int x);

	void SetLoop(int start, int end, int row);

	void SetUpdateRate(float rate);

	int GetRow() const;

	int GetWidth() const;
	int GetHeight() const;

	sf::Sprite& GetCurrentSprite();

private:
	sf::Sprite sprite; //the sprite

	int start;
	int end;
	int row;

	std::vector<int> rowSizes; //number of sprites in the texture
	int currentSprite; //sprite we're currently at

	float updateRate; //rate at which the sprite updates
	sf::Clock animationTimer; //times when to update the sprite

	sf::Vector2i spriteSize; //size of the sprite in pixels
};

