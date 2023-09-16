#include "bits/stdc++.h"
using namespace std;

int board[9][9], num[9][9];
bitset<10> row[9], col[9], box[9];

void init() {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            num[i][j] = i/3 * 3 + j/3;

    for (int i = 0; i < 9; ++i)
        for (int j = 1; j <= 9; ++j)
            row[i][j] = col[i][j] = box[i][j] = 1;
}

void Move(int &x, int &y, int &n, bool b) {
    board[x][y] = b ? 0 : n;
    row[x][n] = col[y][n] = box[num[x][y]][n] = b;
}

int Size;
pair<int,int> Empty[81];

bool cmp(pair<int,int> &x, pair<int,int> &y) {
    return (row[x.first] & col[x.second] & box[num[x.first][x.second]]).count() <
            (row[y.first] & col[y.second] & box[num[y.first][y.second]]).count();
}

void solve(int idx) {
    if (idx == Size) {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                cout << board[i][j] << " \n"[j+1 == 9];
        exit(0);
    }

    nth_element(Empty+idx, Empty+idx, Empty+Size, cmp);

    auto &[x, y] = Empty[idx];
    bitset<10> tmp = row[x] & col[y] & box[num[x][y]];

    for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
        Move(x, y, n, 0);
        solve(idx + 1);
        Move(x, y, n, 1);
    }
}

void solve_sudoku() {
    Size = 0;
    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            if (!board[x][y])
                Empty[Size++] = {x, y};
    solve(0);
}

int main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);

	init();

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j) {
            char input;
            cin >> input;
            int tmp = input - '0';
            if (tmp) Move(i, j, tmp, 0);
        }

    solve_sudoku();

    cout << "No solution!\n";
}
