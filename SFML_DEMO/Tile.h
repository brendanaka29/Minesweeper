#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "TextureManager.h"
using namespace std;

class Tile {
	sf::Sprite sprite;
	bool isMine; 
	bool flagged;
	bool revealed;
	vector <Tile*> adjacentTiles;
public: 
	Tile(sf::Sprite tile = sf::Sprite());
	sf::Sprite& GetSprite();
	void SetIsMine(bool isMine);
	bool GetIsMine();
	void SetFlagged(bool flagged);
	bool GetFlagged();
	void SetRevealed(bool revealed);
	bool GetRevealed();
	void SetAdjacentTiles(vector<Tile*>& adjacentTiles);
	vector<Tile*>& GetAdjacentTiles();
};