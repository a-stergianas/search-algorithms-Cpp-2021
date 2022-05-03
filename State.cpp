#include <algorithm>
#include <random>
#include <sstream>
#include <iostream>
#include "State.h"

using namespace std;

State::State() {
    tiles = nullptr;
    previous = nullptr;
    width = 0;
    height = 0;
    size1 = 0;
    blank_h = 0;
    blank_w = 0;
    g = 0;
}

State::State(int h, int w, int* numbers) {
    width = w;
    height = h;
    size1 = w * h;
    previous = nullptr;
    g=0;
    tiles = new int *[height];
    for (int i = 0; i < height; i++) {
        tiles[i] = new int[width];
        for (int j = 0; j < width; j++) {
            tiles[i][j] = numbers[i * width + j];
            if (tiles[i][j] == 0) {
                blank_h = i;
                blank_w = j;
            }
        }
    }
}

State::State(const State &s) {
    width = s.width;
    height = s.height;
    size1 = s.size1;
    previous = s.previous;
    g = 0;
    blank_h = s.blank_h;
    blank_w = s.blank_w;
    tiles = new int *[height];
    for (int i = 0; i < height; i++) {
        tiles[i] = new int[width];
        for (int j = 0; j < width; j++) {
            tiles[i][j] = s.tiles[i][j];
        }
    }
}

State::~State() {
    delete[] tiles;
}

void State::setG(int g){
    this->g = g;
}

int State::getG(){
    return g;
}

string State::toString() const {
    long maxLength = to_string(size1 - 1).length();
    stringstream out;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (tiles[i][j] == 0) {
                for (int k = 0; k < maxLength; k++) {
                    out << "_";
                }
                out << " ";
            } else {
                long numLength = to_string(tiles[i][j]).length();
                for (long k = numLength; k < maxLength; k++) {
                    out << " ";
                }
                out << tiles[i][j] << " ";
            }
        }
        out << endl;
    }
    return out.str();
}

State &State::operator=(const State &o) {
    width = o.width;
    height = o.height;
    size1 = o.size1;
    previous = o.previous;
    blank_h = o.blank_h;
    blank_w = o.blank_w;
    tiles = new int *[height];
    for (int i = 0; i < height; i++) {
        tiles[i] = new int[width];
        for (int j = 0; j < width; j++) {
            tiles[i][j] = o.tiles[i][j];
        }
    }

    return *this;
}

bool State::isSolvable() {
    int flatTiles[size1];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            flatTiles[height * i + j] = tiles[i][j];
        }
    }
    int inv_count = 0;
    for (int i = 0; i < size1 - 1; i++)
        for (int j = i+1; j < size1; j++)
             if (flatTiles[i] && flatTiles[j] &&  flatTiles[i] > flatTiles[j])
                  inv_count++;
    if(size1%2==1)
        return (inv_count%2==0);
    else{
        if(height%2==0)
            return (inv_count+blank_h+1) % 2 == 0;
        else
            return (inv_count+blank_h+1) % 2 == 1;
    }
}

bool State::isFinished() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (1 + i * width + j == size1) {
                if (tiles[i][j] != 0) {
                    return false;
                }
            } else {
                if (tiles[i][j] != 1 + i * width + j) {
                    return false;
                }
            }
        }
    }
    return true;
}

int State::manhattan() {
    int score = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int posX = (tiles[i][j] - 1) / width;
            int posY = (tiles[i][j] - 1) % width;
            score += abs(posX - i) + abs(posY - j);
        }
    }
    return score;
}

vector<State> State::expand() {
    vector<State> children;

    State s;
    if (moveLeft(s))
        children.push_back(s);

    if (moveRight(s))
        children.push_back(s);

    if (moveUp(s))
        children.push_back(s);

    if (moveDown(s))
        children.push_back(s);

    return children;
}

void State::moveBlank(int h, int w) {
    int moveFrom_h = blank_h;
    int moveFrom_w = blank_w;
    blank_h += h;
    blank_w += w;

    int num = tiles[blank_h][blank_w];
    tiles[moveFrom_h][moveFrom_w] = num;
    tiles[blank_h][blank_w] = 0;
}

void State::moveX(int x) {
    moveBlank(x, 0);
}

void State::moveY(int y) {
    moveBlank(0, y);
}

bool State::moveUp(State &s) {
    if (blank_h == 0) {
        return false;
    }
    s = *this;
    s.setG(g+1);
    s.moveX(-1);
    s.previous = new State(*this);
    return true;
}

bool State::moveDown(State &s) {
    if (blank_h == height - 1) {
        return false;
    }
    s = *this;
    s.setG(g+1);
    s.moveX(+1);
    s.previous = new State(*this);
    return true;
}

bool State::moveLeft(State &s) {
    if (blank_w == 0) {
        return false;
    }
    s = *this;
    s.setG(g+1);
    s.moveY(-1);
    s.previous = new State(*this);
    return true;
}

bool State::moveRight(State &s) {
    if (blank_w == width - 1) {
        return false;
    }
    s = *this;
    s.setG(g+1);
    s.moveY(+1);
    s.previous = new State(*this);
    return true;
}

vector<string> State::getPath() {
    if (previous == nullptr) {
        return vector<string>();
    }

    vector<string> path;
    path.emplace_back(toString());

    State* ptr = previous;
    do {
        path.emplace_back(ptr->toString());
        ptr = ptr->previous;
    } while (ptr != nullptr);

    reverse(path.begin(), path.end());
    return path;
}
