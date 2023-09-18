#pragma GCC target("avx")
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-fwhole-program")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-skip-blocks")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("-funsafe-loop-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")

#include "bits/stdc++.h"
using namespace std;

const float TLE = 0.01;

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

void output(int &x, int &y, int &n) {
    Move(x, y, n, 0);
    cout << char(x + 'A') << char(y + 'a') << n << endl;
}

void win(int &x, int &y, int &n) {
    Move(x, y, n, 0);
    cout << char(x + 'A') << char(y + 'a') << n << '!' << endl;
}

clock_t Time;
int sol, Size;
pair<int,int> Empty[81];

bool cmp(pair<int,int> &x, pair<int,int> &y) {
    return (row[x.first] & col[x.second] & box[num[x.first][x.second]]).count() <
            (row[y.first] & col[y.second] & box[num[y.first][y.second]]).count();
}

void solve(int idx) {
    if (idx == Size) {
        ++sol;
        return;
    }

    nth_element(Empty+idx, Empty+idx, Empty+Size, cmp);

    auto &[x, y] = Empty[idx];
    bitset<10> tmp = row[x] & col[y] & box[num[x][y]];

    for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
        if (sol > 1  || float(clock() - Time) / CLOCKS_PER_SEC >= TLE) return;

        Move(x, y, n, 0);
        solve(idx + 1);
        Move(x, y, n, 1);
    }
}

void solve_sudoku() {
    Time = clock();
    sol = Size = 0;

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            if (!board[x][y])
                Empty[Size++] = {x, y};

    solve(0);
}

tuple<int,int,int> valid_move[729];
mt19937 rd(chrono::high_resolution_clock::now().time_since_epoch().count());

void random_move() {
    int sz = 0;
    bitset<10> tmp;

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            if (!board[x][y]) {
                tmp = row[x] & col[y] & box[num[x][y]];
                for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n))
                    valid_move[sz++] = {x, y, n};
            }

    int Rand;
    do {
        Rand = rd() % sz;
        auto &[x, y, n] = valid_move[Rand];
        Move(x, y, n, 0);
        solve_sudoku();
        Move(x, y, n, 1);
    } while (sol == 0);

    auto &[x, y, n] = valid_move[Rand];
    output(x, y, n);
}

int X, Y, N;

int minimax(int depth, int &max_depth) {
    solve_sudoku();

    if (depth & 1) {
        if (sol == 1) return -1;
        if (sol == 0) return 1;
    } else {
        if (sol == 1) return 1;
        if (sol == 0) return -1;
    }
    if (depth > max_depth) return 0;

    if (depth & 1) {
        int ma = -999, Tmp;
        bitset<10> tmp;

        for (int x = 0; x < 9; ++x)
            for (int y = 0; y < 9; ++y)
                if (!board[x][y]) {
                    tmp = row[x] & col[y] & box[num[x][y]];
                    for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
                        if (ma == 1) break;

                        Move(x, y, n, 0);
                        Tmp = minimax(depth+1, max_depth);
                        Move(x, y, n, 1);

                        if (ma < Tmp) {
                            ma = Tmp;
                            if (depth == 1) {
                                X = x;
                                Y = y;
                                N = n;
                            }
                        }
                    }
                }

        return ma;
    } else {
        int mi = 999, Tmp;
        bitset<10> tmp;

        for (int x = 0; x < 9; ++x)
            for (int y = 0; y < 9; ++y)
                if (!board[x][y]) {
                    tmp = row[x] & col[y] & box[num[x][y]];
                    for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
                        if (mi == -1) break;

                        Move(x, y, n, 0);
                        Tmp = minimax(depth+1, max_depth);
                        Move(x, y, n, 1);

                        if (mi > Tmp) mi = Tmp;
                    }
                }

        return mi;
    }
}

void best_move(int depth) {
    minimax(1, depth);

    if (board[X][Y]) {
        random_move();
        return;
    }

    Move(X, Y, N, 0);
    solve_sudoku();

    if (sol == 1) win(X, Y, N);
    else output(X, Y, N);
}

signed main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	init();

	string input;
	int turn = 0;

	while (cin >> input) {
        if (input == "Quit") break;

        if (input != "Start") {
            ++turn;
            int x = input[0] - 'A';
            int y = input[1] - 'a';
            int n = input[2] - '0';
            Move(x, y, n, 0);
        }

        ++turn;
        if (turn <= 15) random_move();
        else best_move(turn / 30 + 1);
	}
}
