#include <iostream>
#include "State.h"
#include <set>
#include <stack>
#include <queue>

using namespace std;

struct compareStates1 {
    bool operator()(State &a, State &b) {
        return a.manhattan() > b.manhattan();
    }
};

struct compareStates2 {
    bool operator()(State &a, State &b) {
        return a.manhattan() + a.getG() > b.manhattan() + b.getG();
    }
};

int dfs(State &goal, set<State> &closed, stack<State> &open) {
    int iter = 0;
    while (!open.empty()) {
        State nextState = open.top();
        open.pop();
        if (nextState.isFinished()) {
            goal = nextState;
            return iter;
        }
        if (closed.empty() || closed.find(nextState) == closed.end()) {
            closed.insert(nextState);
            vector<State> children = nextState.expand();
            for (State &s : children) {
                open.push(s);
            }
        }
        iter++;
    }
    return -1;
}

int bfs(State &goal, set<State> &closed, queue<State> &open) {
    int iter = 0;
    while (!open.empty()) {
        State nextState = open.front();
        open.pop();
        if (nextState.isFinished()) {
            goal = nextState;
            return iter;
        }
        if (closed.empty() || closed.find(nextState) == closed.end()) {
            closed.insert(nextState);
            vector<State> children = nextState.expand();
            for (State &s : children) {
                open.push(s);
            }
        }
        iter++;
    }
    return -1;
}

int bestFS(State &goal, set<State> &closed, priority_queue<State, vector<State>, compareStates1> &open) {
    int iter = 0;
    while (!open.empty()) {
        State nextState = open.top();
        open.pop();
        if (nextState.isFinished()) {
            goal = nextState;
            return iter;
        }
        if (closed.empty() || closed.find(nextState) == closed.end()) {
            closed.insert(nextState);
            vector<State> children = nextState.expand();
            for (State &s : children) {
                open.push(s);
            }
        }
        iter++;
    }
    return -1;
}

int AStar(State &goal, set<State> &closed, priority_queue<State, vector<State>, compareStates2> &open) {
    int iter = 0;
    while (!open.empty()) {
        State nextState = open.top();
        open.pop();
        if (nextState.isFinished()) {
            goal = nextState;
            return iter;
        }
        if (closed.empty() || closed.find(nextState) == closed.end()) {
            closed.insert(nextState);
            vector<State> children = nextState.expand();
            for (State &s : children) {
                open.push(s);
            }
        }
        iter++;
    }
    return -1;
}

int main() {
    int selection=2;
    while (selection==2){
        State initialState;
        int width = 3, height = 3;

        cout << "Do you want to use the default initial state?" << endl
             << "1) Yes." << endl
             << "2) No I want to enter initial state manualy." << endl;
        cin >> selection;

        while(selection!=1 && selection!=2){
            cout << "Wrong input. Try again:" << endl;
            cin >> selection;
        }

        if (selection == 1){
            int startNumbers[]={3, 6, 0, 1, 4, 2, 7, 5, 8};
            initialState = State(3, 3, startNumbers);
            cout << endl
                 << "Initial state: " << endl
                 << initialState.toString() << endl;
        }
        else {
            cout << endl << "Insert height: ";
            cin >> height;
            cout << endl << "Insert width: ";
            cin >> width;
            int startNumbers[width*height];

            bool flag = false;
            while (!flag) {
                cout << endl
                     << "Enter initial state: " << endl;
                for(int i=0;i<width*height;i++){
                    cin >> startNumbers[i];
                }
                initialState = State(height, width, startNumbers);
                cout << endl
                     << "Initial state: " << endl
                     << initialState.toString() << endl;

                flag = initialState.isSolvable();

                if (!flag)
                    cout << "Initial state is not solvable. Try again." << endl;
            }
        }

        State goalState;
        int goalNumbers[height*width];
        for(int i=0;i<height*width;i++){
            goalNumbers[i] = i + 1;
            if(i==height*width-1)
                goalNumbers[i] = 0;
        }
        goalState = State(height, width, goalNumbers);
        selection=1;
        while(selection==1){
            cout << endl << "Select algorithm:" << endl
                         << "1) Depth First Search" << endl
                         << "2) Breadth First Search" << endl
                         << "3) Best First Search" << endl
                         << "4) A*" << endl;
            cin >> selection;
            cout << endl;

            while(selection!=1 && selection!=2 && selection!=3 && selection!=4){
                cout << "Wrong input. Try again:" << endl;
                cin >> selection;
            }

            int iterations;
            set<State> closedList;

            switch (selection) {
                case 1: {
                    cout << "Depth First Search algorithm selected." << endl
                         << "Calculating solution, please wait..." << endl;
                    stack<State> openList;
                    openList.push(initialState);
                    iterations = dfs(goalState, closedList, openList);
                    break;
                }

                case 2: {
                    cout << "Breadth First Search algorithm selected." << endl
                         << "Calculating solution, please wait..." << endl;
                    queue<State> openList;
                    openList.push(initialState);
                    iterations = bfs(goalState, closedList, openList);
                    break;
                }

                case 3: {
                    cout << "Best First Search Algorithm selected." << endl
                         << "Calculating solution, please wait..." << endl;
                    priority_queue<State, vector<State>, compareStates1> openList;
                    openList.push(initialState);
                    iterations = bestFS(goalState, closedList, openList);
                    break;
                }

                case 4: {
                    cout << "A* Algorithm selected." << endl
                         << "Calculating solution, please wait..." << endl;
                    priority_queue<State, vector<State>, compareStates2> openList;
                    openList.push(initialState);
                    iterations = AStar(goalState, closedList, openList);
                    break;
                }
            }

            if (iterations > 0) {
                cout << endl << "Solution:" << endl << endl;
                vector<string> path = goalState.getPath();
                for (string &s : path) {
                    cout << s << endl;
                }
                cout << "Completed in " << path.size() - 1 << " steps." << endl
                     << "(" << iterations << " iterations)" << endl;
            } else {
                cout << "No solution!" << endl;
            }

            cout << endl << "1) Use different search algorithm for the same initial state." << endl
                         << "2) Select different initial state." << endl
                         << "3) Exit." << endl;
            cin >> selection;

            while(selection!=1 && selection!=2 && selection!=3){
                cout << "Wrong input. Try again:" << endl;
                cin >> selection;
            }
        }
    }
    return 0;
}
