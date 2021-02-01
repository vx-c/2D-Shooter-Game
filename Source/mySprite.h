#pragma once

#include <SFML/Graphics.hpp>

class MySprite
{
public:
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
	sf::Sprite sprite; 

	int start;
	int end;
	int row;

	std::vector<int> rowSizes; //number of sprites in the texture
	int currentSprite; 

	float updateRate; 
	sf::Clock animationTimer; 

	sf::Vector2i spriteSize; 
};

