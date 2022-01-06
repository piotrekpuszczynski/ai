//
// Created by Piotr Puszczyński on 09/11/2021.
//

#include <random>
#include <iostream>
#include "Node.h"
#include "PriorityQueue.h"

using namespace std;


void printBoard(int* board, int n) {
    for (int i = 0; i < n * n - 1; i++) {
        if (i % n == 0) cout << "[";
        cout << board[i];
        if (i % n == n - 1) cout << "]" << endl;
        else cout << ", ";
    }
    cout << "0]" << endl;
}

int* generateRandomArray(int n) {
    int* array = new int[n * n - 1];
    for (int i = 0; i < n * n - 1; i++) {
        array[i] = i + 1;
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0,n * n - 2);

    for (int i = 0; i < n * n; i++) {
        int r1 = dist(gen);
        int r2 = dist(gen);

        int temp = array[r1];
        array[r1] = array[r2];
        array[r2] = temp;
    }
    //{4, 5, 2, 1, 12, 15, 6, 10, 3, 7, 11, 13, 9, 14, 8, 0}
    //{7, 2, 8, 15, 10, 13, 9, 6, 14, 11, 4, 5, 3, 12, 1, 0}
    //{14, 2, 8, 4, 11, 9, 3, 13, 12, 6, 7, 10, 15, 1, 5, 0}
    array = new int[n * n]{6, 5, 4, 15, 10, 9, 2, 13, 1, 8, 3, 14, 7, 11, 12, 0};

    int counter = 0;
    for (int i = 0; i < n * n - 1; i++) {
        for (int j = i + 1; j < n * n - 1; j++) {
            if (array[i] > array[j]) {
                counter++;
            }
        }
    }
    if (counter % 2 != 0) {
        printBoard(array, n);
        cout << "is unsolvable" << endl;
        return nullptr;
    }

    return array;
}

int** generateRandomBoard(int n) {
    int* array = generateRandomArray(n);
    if (array == nullptr) return nullptr;

    int** board = new int*[n];
    for (int i = 0; i < n * n - 1; i++) {
        int y = i % n;
        int x = i / n;
        if (x == 0) board[y] = new int[n];

        board[y][x] = array[i];
    }
    board[n - 1][n - 1] = 0;

    return board;
}

void solve(int** board, int n, char type) {
    Node* root = new Node(board, ' ', nullptr, 0, n, type);
    root->printBoard();
    Node* current = root;
    auto* priorityQueue = new PriorityQueue();

    int visited = 0;
    while (current->h != 0) {
        current->addNextSteps();

        for (int i = 0; i < current->nextNodes; i++) {
            priorityQueue->insert(current->next[i]);
        }

        current = priorityQueue->pop();
        visited++;
    }

    current->printPath();
    cout << "steps: " << current->g << endl;
    cout << "visited: " << visited << endl;
}

int main() {
    int n = 4;

    int** board = generateRandomBoard(n);
    if (board != nullptr) {
        solve(board, n, '3');
        //solve(board, n, '1');
    }

    return 0;
}
