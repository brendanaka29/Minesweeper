#include "Board.h"

Board::Board() {
    tilesvector = {};
    rows = 0;
    columns = 0;
}

Board::Board(int rows, int columns, int mines) {
    this->rows = rows;
    this->columns = columns;
    this->mines = mines;
    this->tilesLeft = rows * columns - mines;

    win = false;
    revealed = false;
    debugState = false;

    //------------setting tiles----------------//
    for (int i = 0; i < rows; i++) {
        vector<Tile> tempvector;
        for (int j = 0; j < columns; j++) {
            sf::Sprite temp(TextureManager::GetTexture("tile_hidden"));
            temp.setPosition(j * 32, i * 32);
            Tile tile = Tile(temp);
            tempvector.push_back(tile);
        }
        tilesvector.push_back(tempvector);
    }

    int count = mines; 
    while (count != 0) {
        int row = Random::Int(0, rows - 1);
        int column = Random::Int(0, columns - 1);
        if (tilesvector[row][column].GetIsMine() == true) {
            continue;
        }
        else {
            tilesvector[row][column].SetIsMine(true);
            count--;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            adjacentTiles(i,j);
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            tilesvector[i][j].SetFlagged(false);
            tilesvector[i][j].SetRevealed(false);
        }
    }
}

vector <vector<Tile>>& Board::GetTilesVector() {
    return tilesvector;
}

void Board::setTile(sf::Texture& texture, int row, int column){
    tilesvector[row][column].GetSprite().setTexture(texture);
}

sf::Sprite Board::GetFace() {
    return face;
}

void Board::SetFace(sf::Texture& texture) {
    face.setTexture(texture);
    face.setPosition(columns * 16, rows * 32);
}

void Board::drawBoard(sf::RenderWindow& window){
    face.setPosition(columns/2 * 32 - 16, rows * 32);
    debug.setPosition(columns * 16 + 128-32, rows * 32);
    test1.setPosition(columns * 16 + (32 * 6)-32, rows * 32);
    test2.setPosition(columns * 16 + (32 * 8)-32, rows * 32);
    test3.setPosition(columns * 16 + (32 * 10)-32, rows * 32);

    //draw sprites
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            window.draw(tilesvector[i][j].GetSprite());
        }
    }


    //flags and numbers 
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (tilesvector[i][j].GetFlagged()) {
                sf::Sprite sprite(TextureManager::GetTexture("flag"));
                sprite.setPosition(j*32,i*32);
                window.draw(sprite);
            }

            if (tilesvector[i][j].GetRevealed()) {
                if (tilesvector[i][j].GetIsMine() == false) {
                    int count = adjacentTiles(i, j);
                    if (count > 0) {
                        sf::Sprite sprite1(TextureManager::GetTexture("tile_revealed"));
                        sprite1.setPosition(j * 32, i * 32);
                        window.draw(sprite1);
                        sf::Sprite sprite(TextureManager::GetTexture("number_" + to_string(count)));
                        sprite.setPosition(j * 32, i * 32);
                        window.draw(sprite);
                    }
                    else {
                        sf::Sprite sprite1(TextureManager::GetTexture("tile_revealed"));
                        sprite1.setPosition(j * 32, i * 32);
                        window.draw(sprite1);
                    }
                  
                }
            }
        }
    }

    //if debug 
    if (debugState) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (tilesvector[i][j].GetIsMine()) {
                    sf::Sprite sprite(TextureManager::GetTexture("mine"));
                    sprite.setPosition(j * 32, i * 32);
                    window.draw(sprite);
                }
            }
        }
    }

    //if lose 
    if (revealed) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (tilesvector[i][j].GetIsMine()) {
                    tilesvector[i][j].SetRevealed(true);
                    sf::Sprite sprite(TextureManager::GetTexture("mine"));
                    sprite.setPosition(j * 32, i * 32);
                    window.draw(sprite);
                }
            }
        }
    }

    //win condition
    int countWin = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (tilesvector[i][j].GetRevealed() == true) {
                countWin++;
            }
        }
    }
    //cout << tilesLeft << endl;
    //cout << countWin << endl;
    if (countWin == tilesLeft) {
        debugState = false;
        win = true;
        mines = 0;
        face.setTexture(TextureManager::GetTexture("face_win"));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                tilesvector[i][j].SetRevealed(true);
                if (tilesvector[i][j].GetIsMine()) {
                    tilesvector[i][j].SetFlagged(true);
                }
                else {
                    tilesvector[i][j].SetRevealed(true);
                }
            }

        }
    }

    window.draw(face);
    window.draw(debug);
    window.draw(test1);
    window.draw(test2);
    window.draw(test3);
    if (mines >= 0) {
        int firstDigit = mines / 100;
        int secondDigit = (mines / 10)%10; 
        int thirdDigit = mines % 10;
        counterfirst.setPosition(21,rows*32);
        countersecond.setPosition(42, rows * 32);
        counterthird.setPosition(63, rows * 32);
        counterfirst.setTextureRect(sf::IntRect(firstDigit*21, 0,21,32));
        countersecond.setTextureRect(sf::IntRect(secondDigit * 21, 0, 21, 32));
        counterthird.setTextureRect(sf::IntRect(thirdDigit * 21, 0, 21, 32));
        window.draw(counterfirst);
        window.draw(countersecond);
        window.draw(counterthird);
    }
    else {
        int firstDigit = mines*-1  / 100;
        int secondDigit = mines*-1 / 10;
        int thirdDigit = mines*-1 % 10;
        negative.setPosition(0, rows * 32);
        counterfirst.setPosition(21, rows * 32);
        countersecond.setPosition(42, rows * 32);
        counterthird.setPosition(63, rows * 32);
        negative.setTextureRect(sf::IntRect(10*21,0,21,32));
        counterfirst.setTextureRect(sf::IntRect(firstDigit * 21, 0, 21, 32));
        countersecond.setTextureRect(sf::IntRect(secondDigit * 21, 0, 21, 32));
        counterthird.setTextureRect(sf::IntRect(thirdDigit * 21, 0, 21, 32));
        window.draw(negative);
        window.draw(counterfirst);
        window.draw(countersecond);
        window.draw(counterthird);
    }
}

int Board::GetRows() {
    return rows;
}

int Board::GetColumns() {
    return columns;
}

sf::Sprite& Board::GetDebug() {
    return debug;
}

sf::Sprite& Board::GetTest1() {
    return test1;
}

sf::Sprite& Board::GetTest2() {
    return test2;
}

sf::Sprite& Board::GetTest3() {
    return test3;
}

sf::Sprite& Board::GetNegative() {
    return negative;
}

sf::Sprite& Board::GetCounterFirst() {
    return counterfirst;
}

sf::Sprite& Board::GetCounterSecond() {
    return countersecond;
}

void Board::setRevealed(bool revealed) {
    this->revealed = revealed;
}

bool Board::getRevealed() {
    return revealed;
}

int Board::GetMines() {
    return mines;
}

void Board::setMines(int mines) {
    this->mines = mines;
}

int Board::adjacentTiles(int row, int column) {
    //cout << "This doesnt work" << endl;
    int count = 0;
    vector<Tile*> temp;
    if (row > 0 && row < tilesvector.size() - 1 && column > 0 && column < tilesvector[0].size() - 1) {
        temp.push_back(&tilesvector[row - 1][column - 1]); //NW
        temp.push_back(&tilesvector[row - 1][column]); //N
        temp.push_back(&tilesvector[row - 1][column + 1]); //NE
        temp.push_back(&tilesvector[row][column - 1]); //W
        temp.push_back(&tilesvector[row][column + 1]); //E
        temp.push_back(&tilesvector[row + 1][column - 1]); //SW
        temp.push_back(&tilesvector[row + 1][column]); //S
        temp.push_back(&tilesvector[row + 1][column + 1]); //S

        if (tilesvector[row - 1][column - 1].GetIsMine()) {//NW
            count++;
        }
        if (tilesvector[row - 1][column].GetIsMine()) { //N
            count++;
        }
        if (tilesvector[row - 1][column + 1].GetIsMine()) { //NE
            count++;
        }
        if (tilesvector[row][column - 1].GetIsMine()) { //W
            count++;
        }
        if (tilesvector[row][column + 1].GetIsMine()) { //E
            count++;
        }
        if (tilesvector[row + 1][column - 1].GetIsMine()) { //SW
            count++;
        }
        if (tilesvector[row + 1][column].GetIsMine()) { //S
            count++;
        }
        if (tilesvector[row + 1][column + 1].GetIsMine()) { //SE 
            count++;
        }

    }

    else if (row == 0 && column == 0) {
        temp.push_back(nullptr); //NW
        temp.push_back(nullptr); //N
        temp.push_back(nullptr); //NE
        temp.push_back(nullptr); //W
        temp.push_back(&tilesvector[row][column + 1]);
        temp.push_back(nullptr); //SW
        temp.push_back(&tilesvector[row + 1][column]);
        temp.push_back(&tilesvector[row + 1][column + 1]);
        if (tilesvector[row][column + 1].GetIsMine()) { //E
            count++;
        }

        if (tilesvector[row + 1][column].GetIsMine()) { //S
            count++;
        }
        if (tilesvector[row + 1][column + 1].GetIsMine()) { //SE 
            count++;
        }
    }
    else if (row == 0 && column == tilesvector[0].size() - 1) {
        temp.push_back(nullptr); //NW
        temp.push_back(nullptr); //N
        temp.push_back(nullptr); //NE
        temp.push_back(&tilesvector[row][column - 1]); 
        temp.push_back(nullptr); //E
        temp.push_back(&tilesvector[row + 1][column - 1]);
        temp.push_back(&tilesvector[row + 1][column]);
        temp.push_back(nullptr); //SE
        if (tilesvector[row][column - 1].GetIsMine()) { //W
            count++;
        }
        if (tilesvector[row + 1][column - 1].GetIsMine()) { //SW
            count++;
        }
        if (tilesvector[row + 1][column].GetIsMine()) { //S
            count++;
        }
    }
    else if (row == 0 && column > 0 && column < tilesvector[0].size() - 1) {
        temp.push_back(nullptr); //NW
        temp.push_back(nullptr); //N
        temp.push_back(nullptr); //NE
        temp.push_back(&tilesvector[row][column - 1]);
        temp.push_back(&tilesvector[row][column + 1]);
        temp.push_back(&tilesvector[row + 1][column - 1]);
        temp.push_back(&tilesvector[row + 1][column]);
        temp.push_back(&tilesvector[row + 1][column + 1]);
        if (tilesvector[row][column - 1].GetIsMine()) { //W
            count++;
        }
        if (tilesvector[row][column + 1].GetIsMine()) { //E
            count++;
        }
        if (tilesvector[row + 1][column - 1].GetIsMine()) { //SW
            count++;
        }
        if (tilesvector[row + 1][column].GetIsMine()) { //S
            count++;
        }
        if (tilesvector[row + 1][column + 1].GetIsMine()) { //SE 
            count++;
        }
    }
    else if (row == tilesvector.size() - 1 && column == 0) {
        temp.push_back(nullptr); //NW
        temp.push_back(&tilesvector[row - 1][column]);
        temp.push_back(&tilesvector[row - 1][column + 1]);
        temp.push_back(nullptr); //W
        temp.push_back(&tilesvector[row][column + 1]);
        temp.push_back(nullptr); //SW
        temp.push_back(nullptr); //S
        temp.push_back(nullptr); //SE
        if (tilesvector[row - 1][column].GetIsMine()) { //N
            count++;
        }
        if (tilesvector[row - 1][column + 1].GetIsMine()) { //NE
            count++;
        }
        if (tilesvector[row][column + 1].GetIsMine()) { //E
            count++;
        }
    }
    else if (row == tilesvector.size() - 1 && column == tilesvector[0].size() - 1) {
        temp.push_back(&tilesvector[row - 1][column - 1]);
        temp.push_back(&tilesvector[row - 1][column]);
        temp.push_back(nullptr); //NE
        temp.push_back(&tilesvector[row][column - 1]);
        temp.push_back(nullptr); //E
        temp.push_back(nullptr); //SW
        temp.push_back(nullptr); //S
        temp.push_back(nullptr); //SE
        if (tilesvector[row - 1][column - 1].GetIsMine()) {//NW
            count++;
        }
        if (tilesvector[row - 1][column].GetIsMine()) { //N
            count++;
        }
        if (tilesvector[row][column - 1].GetIsMine()) { //W
            count++;
        }
    }
    else if (row == tilesvector.size() - 1 && column > 0 && column < tilesvector[0].size() - 1) {
        temp.push_back(&tilesvector[row - 1][column - 1]);
        temp.push_back(&tilesvector[row - 1][column]);
        temp.push_back(&tilesvector[row - 1][column + 1]);
        temp.push_back(&tilesvector[row][column - 1]);
        temp.push_back(&tilesvector[row][column + 1]);
        temp.push_back(nullptr); //SW
        temp.push_back(nullptr); //S
        temp.push_back(nullptr); //SE
        if (tilesvector[row - 1][column - 1].GetIsMine()) {//NW
            count++;
        }
        if (tilesvector[row - 1][column].GetIsMine()) { //N
            count++;
        }
        if (tilesvector[row - 1][column + 1].GetIsMine()) { //NE
            count++;
        }
        if (tilesvector[row][column - 1].GetIsMine()) { //W
            count++;
        }
        if (tilesvector[row][column + 1].GetIsMine()) { //E
            count++;
        }
    }
    else if (column == 0 && row > 0 && row < tilesvector.size() - 1) {
        temp.push_back(nullptr); //NW
        temp.push_back(&tilesvector[row - 1][column]);
        temp.push_back(&tilesvector[row - 1][column + 1]);
        temp.push_back(nullptr); //W
        temp.push_back(&tilesvector[row][column + 1]);
        temp.push_back(nullptr); //SW
        temp.push_back(&tilesvector[row + 1][column]);
        temp.push_back(&tilesvector[row + 1][column + 1]);
        if (tilesvector[row - 1][column].GetIsMine()) { //N
            count++;
        }
        if (tilesvector[row - 1][column + 1].GetIsMine()) { //NE
            count++;
        }
        if (tilesvector[row][column + 1].GetIsMine()) { //E
            count++;
        }
        if (tilesvector[row + 1][column].GetIsMine()) { //S
            count++;
        }
        if (tilesvector[row + 1][column + 1].GetIsMine()) { //SE 
            count++;
        }
    }
    else if (column == tilesvector[0].size() - 1 && row > 0 && row < tilesvector.size() - 1) {
        temp.push_back(&tilesvector[row - 1][column - 1]);
        temp.push_back(&tilesvector[row - 1][column]);
        temp.push_back(nullptr); //NE
        temp.push_back(&tilesvector[row][column - 1]);
        temp.push_back(nullptr); //E
        temp.push_back(&tilesvector[row + 1][column - 1]);
        temp.push_back(&tilesvector[row + 1][column]);
        temp.push_back(nullptr); //SE
        if (tilesvector[row - 1][column - 1].GetIsMine()) {//NW
            count++;
        }
        if (tilesvector[row - 1][column].GetIsMine()) { //N
            count++;
        }
        if (tilesvector[row][column - 1].GetIsMine()) { //W
            count++;
        }
        if (tilesvector[row + 1][column - 1].GetIsMine()) { //SW
            count++;
        }
        if (tilesvector[row + 1][column].GetIsMine()) { //S
            count++;
        }
    }
    tilesvector[row][column].SetAdjacentTiles(temp);
    //cout<< tilesvector[row][column].GetAdjacentTiles().size();
    return count;
}

void Board::SetDebugState(bool debugState) {
    this->debugState = debugState;
}

bool Board::GetDebugState() {
    return debugState;
}

void Board::SetWin(bool win) {
    this->win = win;
}
bool Board::GetWin() {
    return win;
}
void Board::SetTilesLeft(int tilesLeft){
    this->tilesLeft = tilesLeft;
}
int Board::GetTilesLeft(){
    return tilesLeft;
}
