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

const float TLE = 0.1;

int board[9][9], num[9][9];
bitset<10> row[9], col[9], box[9];
long long ZobristTable[9][9][10];
mt19937_64 RandInt(chrono::high_resolution_clock::now().time_since_epoch().count());

void init() {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            num[i][j] = i/3 * 3 + j/3;

    for (int i = 0; i < 9; ++i)
        for (int j = 1; j <= 9; ++j)
            row[i][j] = col[i][j] = box[i][j] = 1;

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            for (int k = 1; k <= 9; ++k)
                ZobristTable[i][j][k] = RandInt();
}

void Move(const int &x, const int &y, const int &n, const bool &b) {
    board[x][y] = b ? 0 : n;
    row[x][n] = col[y][n] = box[num[x][y]][n] = b;
}

void output(const int &x, const int &y, const int &n) {
    Move(x, y, n, 0);
    cout << char(x + 'A') << char(y + 'a') << n << endl;
}

void win(const int &x, const int &y, const int &n) {
    Move(x, y, n, 0);
    cout << char(x + 'A') << char(y + 'a') << n << '!' << endl;
}

clock_t Time;
int sol, Size;
pair<int,int> Empty[81];

bool cmp(const pair<int,int> &x, const pair<int,int> &y) {
    return (row[x.first] & col[x.second] & box[num[x.first][x.second]]).count() <
            (row[y.first] & col[y.second] & box[num[y.first][y.second]]).count();
}

void solve(const int &idx) {
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

long long board_hash() {
    long long res = 0;

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            if (board[x][y])
                res ^= ZobristTable[x][y][board[x][y]];

    return res;
}

unordered_map<long long, int> dp;

void solve_sudoku() {
    long long Hash = board_hash();

    auto k = dp.find(Hash);
    if (k != dp.end()) {
        sol = (*k).second;
        return;
    }

    Time = clock();
    sol = Size = 0;

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            if (!board[x][y])
                Empty[Size++] = {x, y};

    solve(0);

    dp[Hash] = sol;
}

bool check_valid(const int &x, const int &y) {
    if (board[x][y]) return false;

    bitset<10> tmp = row[x] & col[y] & box[num[x][y]];
    int cnt = 0;

    if ((int)tmp.count() == 1) return false;

    for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
        Move(x, y, n, 0);
        solve_sudoku();
        Move(x, y, n, 1);
        cnt += sol > 0;
        if (cnt > 1) break;
    }

    return cnt != 1;
}

void bad_move() {
    bitset<10> tmp;

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            if (!board[x][y]) {
                tmp = row[x] & col[y] & box[num[x][y]];
                for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
                    output(x, y, n);
                    return;
                }
            }
}

int X, Y, N;

void good_move() {
    bitset<10> tmp;
    int ma = X = -1;

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y) {
            if (check_valid(x, y)) {
                tmp = row[x] & col[y] & box[num[x][y]];
                for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
                    Move(x, y, n, 0);
                    solve_sudoku();
                    Move(x, y, n, 1);

                    if (sol == 1) {
                        win(x, y, n);
                        return;
                    }

                    if (sol > ma) {
                        ma = sol;
                        X = x;
                        Y = y;
                        N = n;
                    }
                }
            }
        }

    if (X == -1) bad_move();
    else output(X, Y, N);
}

void opening_move() {
    bitset<10> tmp;
    int ma = X = -1;

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y) {
            if (ma > 1) break;

            if (check_valid(x, y)) {
                tmp = row[x] & col[y] & box[num[x][y]];
                for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
                    if (ma > 1) break;

                    Move(x, y, n, 0);
                    solve_sudoku();
                    Move(x, y, n, 1);

                    if (sol > ma) {
                        ma = sol;
                        X = x;
                        Y = y;
                        N = n;
                    }
                }
            }
        }

    if (X == -1) bad_move();
    else output(X, Y, N);
}

const int MAX = 999;
const int MIN = -MAX;

unordered_map<long long, int> visited;

int minimax(const int &depth, int alpha, int beta, const int &max_depth) {
    long long Hash = board_hash();

    auto k = visited.find(Hash);
    if (k != visited.end()) return (*k).second;

    solve_sudoku();

    if (depth & 1) {
        if (sol == 1) return -1;
        else if (sol == 0) return 1;
    } else {
        if (sol == 1) return 1;
        else if (sol == 0) return -1;
    }
    if (depth > max_depth) return 0;

    if (depth & 1) {
        bool ok = 0;
        int val = MIN, TMP;
        bitset<10> tmp;

        for (int x = 0; x < 9; ++x)
            for (int y = 0; y < 9; ++y) {
                if (ok) break;
                if (check_valid(x, y)) {
                    tmp = row[x] & col[y] & box[num[x][y]];
                    for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
                        Move(x, y, n, 0);
                        TMP = minimax(depth + 1, alpha, beta, max_depth);
                        Move(x, y, n, 1);

                        if (val < TMP) {
                            val = TMP;
                            if (depth == 1) {
                                X = x;
                                Y = y;
                                N = n;
                            }
                        }
                        if (val > beta) {
                            ok = 1;
                            break;
                        }
                        alpha = max(alpha, val);
                    }
                }
            }

        return visited[Hash] = val;
    } else {
        bool ok = 0;
        int val = MAX;
        bitset<10> tmp;

        for (int x = 0; x < 9; ++x)
            for (int y = 0; y < 9; ++y) {
                if (ok) break;
                if (check_valid(x, y)) {
                    tmp = row[x] & col[y] & box[num[x][y]];
                    for (int n = tmp._Find_first(); n < 10; n = tmp._Find_next(n)) {
                        Move(x, y, n, 0);
                        val = min(val, minimax(depth + 1, alpha, beta, max_depth));
                        Move(x, y, n, 1);

                        if (val < alpha) {
                            ok = 1;
                            break;
                        }
                        beta = min(beta, val);
                    }
                }
            }

        return visited[Hash] = val;
    }
}

void best_move(const int &depth) {
    if (depth == 1) {
        good_move();
        return;
    }

    unordered_map<long long, int> tmp;
    swap(visited, tmp);
    X = -1;
    minimax(1, MIN, MAX, depth);

    if (X == -1) {
        good_move();
        return;
    }

    Move(X, Y, N, 0);
    solve_sudoku();

    if (sol == 1) win(X, Y, N);
    else output(X, Y, N);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	init();

	string input;
	int turn = 0;

	while (cin >> input) {
        if (input == "Quit") break;

        if (input != "Start") {
            ++turn;
            Move(input[0]-'A', input[1]-'a', input[2]-'0', 0);
        }

        ++turn;
        if (turn < 17) opening_move();
        else best_move(turn / 30 + 1);
	}
}
