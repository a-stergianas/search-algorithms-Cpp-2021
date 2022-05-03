#ifndef STATE_H
#define STATE_H
#include <vector>
#include <string>

using namespace std;

class State {
    private:
        int **tiles;
        int blank_h;
        int blank_w;
        int width, height, size1;
        State* previous;
        int g;
        void moveBlank(int, int);
        void moveX(int);
        void moveY(int);
        bool moveUp(State &);
        bool moveDown(State &);
        bool moveLeft(State &);
        bool moveRight(State &);

    public:

        State();
        State(int, int);
        State(int, int, int*);
        State(const State &);
        ~State();
        void setG(int);
        int getG();
        State &operator=(const State &);
        friend bool operator<(const State &a, const State &b) {
            return a.toString() < b.toString();
        }
        bool isSolvable();
        bool isFinished();
        int manhattan();
        string toString() const;
        vector<State> expand();
        vector<string> getPath();
};

#endif // STATE_H
