#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "TextureManager.h"
#include "Board.h"
#include "Tile.h"
#include "Random.h"
using namespace std;


string loadTest(string testnum);
void revealTiles(Tile& t);
int adjacentMines(vector<Tile*> adjacentTiles);


int main()
{
    //------------reading from files----------------//
    string columns;
    string rows;
    string tmines;

    ifstream setup("config.cfg");
    getline(setup, columns);
    getline(setup, rows);
    getline(setup, tmines);
    setup.close();

    int width = stoi(columns) * 32;
    int height = stoi(rows) * 32 + 100;
    int mines = stoi(tmines);
    int tiles = stoi(columns) * stoi(rows);

    //------------make winder----------------//
    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper!");

    //------------default board----------------//
    Board board = Board(stoi(rows), stoi(columns), mines);
    board.SetWin(false);

    //------------window----------------//
    while (window.isOpen())
    {
        window.clear(sf::Color(255, 255, 255, 255));
        board.drawBoard(window);
        window.display();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed) {

                //------------tile click----------------//
                if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x <= stoi(columns) * 32 && event.mouseButton.y <= stoi(rows) * 32 && board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32].GetFlagged() == false && board.getRevealed() == false) {

                    board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32].SetRevealed(true); //if tile is clicked reveal it

                    //------------ mine----------------//
                    if (board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32].GetIsMine()) {
                        board.SetFace(TextureManager::GetTexture("face_lose")); //set face to lose
                        board.setRevealed(true); //reveal all tiles 
                    }

                    //------------no mine----------------//
                    else {
                        board.setTile(TextureManager::GetTexture("tile_revealed"), event.mouseButton.y / 32, event.mouseButton.x / 32); //reveals tile clicked on
                        revealTiles(board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32]); //reveals all the other tiles


                    }
                }

                //------------flag----------------//
                if (event.mouseButton.button == sf::Mouse::Right && event.mouseButton.x <= stoi(columns) * 32 && event.mouseButton.y <= stoi(rows) * 32 && board.getRevealed() == false && !board.GetWin() && !board.getRevealed()) { //if flag is clicked and game not won/lost
                    if (board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32].GetRevealed() == false && board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32].GetFlagged() == false) {
                        board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32].SetFlagged(true);
                        board.setMines(board.GetMines() - 1);
                    }
                    else if (board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32].GetRevealed() == false && board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32].GetFlagged() == true && !board.GetWin()) {
                        board.GetTilesVector()[event.mouseButton.y / 32][event.mouseButton.x / 32].SetFlagged(false);
                        board.setMines(board.GetMines() + 1);
                    }

                }

                //------------face----------------//
                if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x >= stoi(columns) / 2 * 32 - 16 && event.mouseButton.x <= (stoi(columns) / 2 * 32 - 16 + board.GetFace().getTexture()->getSize().x)) {
                    if (event.mouseButton.y >= stoi(rows) * 32 && event.mouseButton.y <= (stoi(rows) * 32 + board.GetFace().getTexture()->getSize().y)) {
                        board.SetWin(false); //win = false
                        board.setRevealed(false); //lost = false
                        for (int i = 0; i < stoi(rows); i++) { //set all tiles to unflagged
                            for (int j = 0; j < stoi(columns); j++) {
                                board.GetTilesVector()[i][j].SetFlagged(false);
                            }
                        }
                        if (board.GetDebugState()) {
                            board = Board(stoi(rows), stoi(columns), mines);
                            board.SetDebugState(true);
                        }
                        else {
                            board = Board(stoi(rows), stoi(columns), mines);
                        }
                        board.SetFace(TextureManager::GetTexture("face_happy")); //reset face 

                    }

                }

                //------------debug----------------//
                if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x >= stoi(columns) * 16 + 128 - 32 && event.mouseButton.x <= stoi(columns) * 16 + 128 - 32 + board.GetDebug().getTexture()->getSize().x && !board.GetWin() && !board.getRevealed()) {
                    if (event.mouseButton.y >= stoi(rows) * 32 && event.mouseButton.y <= (stoi(rows) * 32 + board.GetDebug().getTexture()->getSize().y)) {
                        if (board.GetDebugState() == false)
                            board.SetDebugState(true);
                        else {
                            board.SetDebugState(false);
                        }
                    }

                }

                //------------test1----------------//
                if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x >= stoi(columns) * 16 + (32 * 6) - 32 && event.mouseButton.x <= stoi(columns) * 16 + (32 * 6) - 32 + board.GetTest1().getTexture()->getSize().x) {
                    if (event.mouseButton.y >= stoi(rows) * 32 && event.mouseButton.y <= stoi(rows) * 32 + board.GetTest1().getTexture()->getSize().y) {
                        board.SetWin(false);
                        board.setRevealed(false);
                        board.SetFace(TextureManager::GetTexture("face_happy"));
                        for (int i = 0; i < stoi(rows); i++) {
                            for (int j = 0; j < stoi(columns); j++) {
                                board.GetTilesVector()[i][j].SetFlagged(false);
                            }
                        }

                        string board1string = loadTest("1");
                        int count = 0;
                        int mines1 = 0;
                        for (int i = 0; i < 16; i++) {
                            for (int j = 0; j < 25; j++) {
                                board.GetTilesVector()[i][j].SetRevealed(false);
                                board.GetTilesVector()[i][j].GetSprite().setTexture(TextureManager::GetTexture("tile_hidden"));
                                board.GetTilesVector()[i][j].SetIsMine(board1string[count] - '0');
                                count++;
                                if (board.GetTilesVector()[i][j].GetIsMine())
                                    mines1++;
                            }
                        }
                        board.setMines(mines1);
                        board.SetTilesLeft(stoi(rows) * stoi(columns) - board.GetMines());
                    }
                }

                //------------test2----------------//
                if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x >= stoi(columns) * 16 + (32 * 8) - 32 && event.mouseButton.x <= stoi(columns) * 16 + (32 * 8) - 32 + board.GetTest2().getTexture()->getSize().x) {
                    if (event.mouseButton.y >= stoi(rows) * 32 && event.mouseButton.y <= stoi(rows) * 32 + board.GetTest2().getTexture()->getSize().y) {
                        board.SetWin(false);
                        board.setRevealed(false);
                        board.SetFace(TextureManager::GetTexture("face_happy"));
                        for (int i = 0; i < stoi(rows); i++) {
                            for (int j = 0; j < stoi(columns); j++) {
                                board.GetTilesVector()[i][j].SetFlagged(false);
                            }
                        }

                        string board2string = loadTest("2");
                        int count2 = 0;
                        int mines2 = 0;
                        for (int m = 0; m < 16; m++) {
                            for (int n = 0; n < 25; n++) {
                                board.GetTilesVector()[m][n].SetRevealed(false);
                                board.GetTilesVector()[m][n].GetSprite().setTexture(TextureManager::GetTexture("tile_hidden"));

                                board.GetTilesVector()[m][n].SetIsMine(board2string[count2] - '0');
                                count2++;
                                if (board.GetTilesVector()[m][n].GetIsMine())
                                    mines2++;
                            }
                        }
                        board.setMines(mines2);
                        board.SetTilesLeft(stoi(rows) * stoi(columns) - board.GetMines());
                    }
                }

                //------------test3----------------//
                if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x >= stoi(columns) * 16 + (32 * 10) - 32 && event.mouseButton.x <= stoi(columns) * 16 + (32 * 10) - 32 + board.GetTest3().getTexture()->getSize().x) {
                    if (event.mouseButton.y >= stoi(rows) * 32 && event.mouseButton.y <= stoi(rows) * 32 + board.GetTest3().getTexture()->getSize().y) {
                        board.SetWin(false);
                        board.setRevealed(false);
                        board.SetFace(TextureManager::GetTexture("face_happy"));
                        for (int i = 0; i < stoi(rows); i++) {
                            for (int j = 0; j < stoi(columns); j++) {
                                board.GetTilesVector()[i][j].SetFlagged(false);
                            }
                        }

                        string board3string = loadTest("3");
                        int count3 = 0;
                        int mines3 = 0;
                        for (int u = 0; u < 16; u++) {
                            for (int v = 0; v < 25; v++) {
                                board.GetTilesVector()[u][v].SetRevealed(false);
                                board.GetTilesVector()[u][v].GetSprite().setTexture(TextureManager::GetTexture("tile_hidden"));

                                board.GetTilesVector()[u][v].SetIsMine(int(board3string[count3]) - '0');
                                count3++;
                                if (board.GetTilesVector()[u][v].GetIsMine())
                                    mines3++;
                            }
                        }
                        board.setMines(mines3);
                        board.SetTilesLeft(stoi(rows) * stoi(columns) - board.GetMines());
                    }
                }
            }
        }
        if (event.type == sf::Event::Closed)
            window.close();
    }
    TextureManager::Clear();
    return 0;
}

string loadTest(string testnum) {
    string boardstring = "";
    string boardtemp = "";
    string fileName = "testboard" + testnum + ".brd";
    ifstream board(fileName);
    while (getline(board, boardtemp)) {
        boardstring += boardtemp;
    }
    board.close();
    return boardstring;
}

void revealTiles(Tile& t) {
    vector <Tile*> adjacentTiles = t.GetAdjacentTiles();
    int count = adjacentMines(adjacentTiles);
    if (count == 0) {
        for (int i = 0; i < 8; i++) {
            if (adjacentTiles[i] != nullptr && !adjacentTiles[i]->GetRevealed() && !adjacentTiles[i]->GetFlagged()) {

                adjacentTiles[i]->SetRevealed(true);
                revealTiles(*adjacentTiles[i]);
            }
        }
    }
}

int adjacentMines(vector<Tile*> adjacentTiles) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (adjacentTiles[i] != nullptr && adjacentTiles[i]->GetIsMine()) {
            count++;
        }
    }
    return count;
}
