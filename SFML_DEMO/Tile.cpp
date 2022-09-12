#include "Tile.h"
Tile::Tile(sf::Sprite sprite) {
	this->sprite = sprite;
	isMine = false;
	flagged = false;
	revealed = false;
}

sf::Sprite& Tile::GetSprite() {
	return sprite; 
}

void Tile::SetIsMine(bool isMine) {
	this->isMine = isMine;
}
bool Tile::GetIsMine() {
	return isMine;
}

void Tile::SetFlagged(bool flagged) {
	this->flagged = flagged;
}
bool Tile::GetFlagged() {
	return flagged;
}
void Tile::SetRevealed(bool revealed) {
	this->revealed = revealed;
}
bool Tile::GetRevealed() {
	return revealed;
}
void Tile::SetAdjacentTiles(vector<Tile*>& adjacentTiles) {
	this->adjacentTiles = adjacentTiles;
}
vector<Tile*>& Tile::GetAdjacentTiles() {
	return adjacentTiles;
}