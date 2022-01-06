//
// Created by Piotr Puszczyński on 08/11/2021.
//

#ifndef LISTA1_NODE_H
#define LISTA1_NODE_H
#include <iostream>
#include <cstdlib>

using namespace std;

class Node {
public:
    int** board;
    char direction;
    Node* prev;
    int nextNodes = 0;
    Node** next = new Node*[nextNodes];
    int g;
    int h = 0;
    int n;
    char type;

    Node(int** board, char direction, Node* prev, int g, int n, char type) {
        this->board = board;
        this->direction = direction;
        this->prev = prev;
        this->g = g;
        this->n = n;
        this->type = type;

        if (type == '1') {
            h1();
        } else if (type == '2') {
            h2();
        } else if (type == '3') {
            h3();
        }

//        printBoard();
//        cout << h << endl;

    }

    void h1() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (j * n + i == board[i][j] - 1) {
                    h++;
                }
            }
        }
        h = n * n - 1 - h;
    }

    void h2() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[j][i] == 0) continue;

                int dy = (board[j][i] - 1) / n;
                int dx = (board[j][i] - 1) - dy * n;

                h += abs(dx - j);
                h += abs(dy - i);
            }
        }
    }

    void h3() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[j][i] == 0) continue;

                int dy = (board[j][i] - 1) / n;
                int dx = (board[j][i] - 1) - dy * n;

                h += abs(dx - j);
                h += abs(dy - i);

                if (corRow(i, j)) {
                    for (int l = 0; l < n; l++) {
                        if (corRow(i, l)
                            && (((l < j && board[l][i] > board[j][i])
                            || (l > j && board[l][i] < board[j][i])))) {
//                            cout << board[l][i] << "row" << board[j][i] << endl;
                            h++;
                        }
                    }
                }

                if (corCol(i, j)) {
                    for (int l = 0; l < n; l++) {
                        if (corCol(l, j) &&
                            ((l < i && board[j][l] > board[j][i]) ||
                            (l > i && board[j][l] < board[j][i]))) {
//                            cout << board[j][l] << "col" << board[j][i] << endl;
                            h++;
                        }
                    }
                }
//                for (int k = 0; k < n; k++) {
//                    // if number is in its row
//                    if ((board[j][i] - 1) == i * n + k) {
//                        for (int l = 0; l < n; l++) {
//                            if ((l < i && board[j][l] > board[j][i]) ||
//                                (l > i && board[j][l] < board[j][i])) {
//                                h++;
//                            }
//                        }
//                        break;
//                    }
//                }
//                for (int k = 0; k < n; k++) {
//                    // if number is in its column
//                    if ((board[j][i] - 1) == k * n + j) {
//                        for (int l = 0; l < n; l++) {
//                            if ((l < j && board[l][i] > board[j][i])
//                            || (l > j && board[l][i] < board[j][i])) {
//                                h++;
//                            }
//                        }
//                        break;
//                    }
//                }
            }
        }
    }

    bool corRow(int i, int j) {
        for (int k = 0; k < n; k++) {
            // if number is in its row
            if ((board[j][i] - 1) == i * n + k) {
                return true;
            }
        }
        return false;
    }

    bool corCol(int i, int j) {
        for (int k = 0; k < n; k++) {
            // if number is in its column
            if ((board[j][i] - 1) == k * n + j) {
                return true;
            }
        }
        return false;
    }

    void add(int** nextBoard, char nextDirection) {
        nextNodes++;
        Node** temp = new Node*[nextNodes];
        for (int i = 0; i < nextNodes - 1; i++) {
            temp[i] = this->next[i];
        }
        temp[nextNodes - 1] = new Node(nextBoard, nextDirection, this, g + 1, n, type);
        this->next = temp;
    }

    void printBoard() const {
        for (int i = 0; i < n; i++) {
            cout << "[";
            for (int j = 0; j < n; j++) {
                cout << board[j][i];
                if (j != n - 1) {
                    cout << ", ";
                }
            }
            cout << "]" << endl;
        }
    }

    void printState() const {
        cout << "direction: " << direction << endl;
        printBoard();
    }

    void printPath() {
        Node** path = new Node*[g];
        Node* temp = this;
        int i = g - 1;
        while (temp->prev != nullptr) {
            path[i] = temp;
            temp = temp->prev;
            i--;
        }
        for (int j = 0; j < g; j++) {
            path[j]->printState();
        }

        //delete[] path;
    }

    int** cloneBoard() const {

        int** clonedBoard = new int*[n];
        for (int i = 0; i < n * n; i++) {
            int y = i % n;
            int x = i / n;
            if (x == 0) {
                clonedBoard[y] = new int[n];
            }
            clonedBoard[x][y] = board[x][y];
        }

        return clonedBoard;
    }

    bool compare(int** b) {
        Node* temp = this;
        while (temp->prev != nullptr) temp = temp->prev;

        return recursion(temp, b);
    }

    bool recursion(Node* node, int** b) {
        if (c(node->board, b)) {
            return true;
        }

        for (int i = 0; i < node->nextNodes; i++) {
            if (recursion(node->next[i], b)) {
                return true;
            }
        }

        return false;
    }

    bool c(int** a, int** b) const {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (a[i][j] != b[i][j]) return false;
            }
        }
        return true;
    }

    void addNextSteps() {
        int zeroX;
        int zeroY;
        int breakLoop = false;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (0 == board[i][j]) {
                    zeroX = i;
                    zeroY = j;
                    breakLoop = true;
                }
                if (breakLoop) {
                    break;
                }
            }
            if (breakLoop) {
                break;
            }
        }

        if (zeroX > 0) {
            int** tempBoard = cloneBoard();
            int temp = tempBoard[zeroX][zeroY];
            tempBoard[zeroX][zeroY] = tempBoard[zeroX - 1][zeroY];
            tempBoard[zeroX - 1][zeroY] = temp;
            if (!compare(tempBoard))
                add(tempBoard, 'l');
        }
        if (zeroX < n - 1) {
            int** tempBoard = cloneBoard();
            int temp = tempBoard[zeroX][zeroY];
            tempBoard[zeroX][zeroY] = tempBoard[zeroX + 1][zeroY];
            tempBoard[zeroX + 1][zeroY] = temp;
            if (!compare(tempBoard))
                add(tempBoard, 'r');
        }
        if (zeroY > 0) {
            int** tempBoard = cloneBoard();
            int temp = tempBoard[zeroX][zeroY];
            tempBoard[zeroX][zeroY] = tempBoard[zeroX][zeroY - 1];
            tempBoard[zeroX][zeroY - 1] = temp;
            if (!compare(tempBoard))
                add(tempBoard, 'u');
        }
        if (zeroY < n - 1) {
            int** tempBoard = cloneBoard();
            int temp = tempBoard[zeroX][zeroY];
            tempBoard[zeroX][zeroY] = tempBoard[zeroX][zeroY + 1];
            tempBoard[zeroX][zeroY + 1] = temp;
            if (!compare(tempBoard))
                add(tempBoard, 'd');
        }
    }
};


#endif //LISTA1_NODE_H
