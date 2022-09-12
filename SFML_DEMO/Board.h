#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "TextureManager.h"
#include "Tile.h"
#include "Random.h"
using namespace std;

class Board {
	vector <vector<Tile>> tilesvector;
	bool revealed = false;
	bool debugState = false;
	int rows;
	int columns;
	int mines;
	bool win = false;
	int tilesLeft;
	sf::Sprite face = sf::Sprite(TextureManager::GetTexture("face_happy"));
	sf::Sprite debug = sf::Sprite(TextureManager::GetTexture("debug"));
	sf::Sprite test1 = sf::Sprite(TextureManager::GetTexture("test_1"));
	sf::Sprite test2 = sf::Sprite(TextureManager::GetTexture("test_2"));
	sf::Sprite test3 = sf::Sprite(TextureManager::GetTexture("test_3"));
	sf::Sprite negative = sf::Sprite(TextureManager::GetTexture("digits"));
	sf::Sprite counterfirst = sf::Sprite(TextureManager::GetTexture("digits"));
	sf::Sprite countersecond = sf::Sprite(TextureManager::GetTexture("digits"));
	sf::Sprite counterthird = sf::Sprite(TextureManager::GetTexture("digits"));
	int adjacentTiles(int row, int column);
	
public: 
	Board();
	Board(int rows, int columns, int mines);
	vector <vector<Tile>>& GetTilesVector();
	void setTile(sf::Texture& texture, int row, int column);
	sf::Sprite GetFace();
	void SetFace(sf::Texture& texture);
	void drawBoard(sf::RenderWindow& window);
	int GetRows();
	int GetColumns();
	sf::Sprite& GetDebug();
	sf::Sprite& GetTest1();
	sf::Sprite& GetTest2();
	sf::Sprite& GetTest3();
	sf::Sprite& GetNegative();
	sf::Sprite& GetCounterFirst();
	sf::Sprite& GetCounterSecond();
	void setRevealed(bool revealed);
	bool getRevealed();
	int GetMines();
	void setMines(int mines);
	void SetDebugState(bool debugState);
	bool GetDebugState();
	void SetWin(bool win);
	bool GetWin();
	void SetTilesLeft(int tilesLeft);
	int GetTilesLeft();
};


