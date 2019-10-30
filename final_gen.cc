// Generate a final layout of white & black cells that obeys the rules:
// - black cells are connected
// - there is no pool (2x2 square of black cells)

#include <cstdio>
#include <chrono>
#include <random>

using namespace std;

#define FR(i,a,b) for(int i=(a);i<(b);++i)
#define FOR(i,n) FR(i,0,n)

std::default_random_engine prng;
void InitPRNG()
{
    prng.seed(std::chrono::system_clock::now().time_since_epoch().count());
}
int RandInt(int lo, int hi)
{
    std::uniform_int_distribution<int> distribution(lo,hi-1);
    return distribution(prng);
}

int const dir_dr[4] = {  0, -1,  0,  1 };
int const dir_dc[4] = {  1,  0, -1,  0 };

int const R = 10;
int const C = 10;

enum class CellType
{
    WHITE, BLACK
};

CellType grid[R][C];

void rand_fill_grid()
{
    {
        int r = RandInt(0, R);
        int c = RandInt(0, C);

        grid[r][c] = CellType::BLACK;
    }

    vector<tuple<int,int>> legal;
    while (true) {
        legal.clear();
        FOR(r,R) FOR(c,C) if (grid[r][c] == CellType::WHITE) {
            bool has_adj_black = false;
            FOR(k,4) {
                int r2 = r+dir_dr[k];
                int c2 = c+dir_dc[k];

                if (0 <= r2 && r2 < R && 0 <= c2 && c2 < C && grid[r2][c2] == CellType::BLACK) {
                    has_adj_black = true;
                }
            }

            bool forms_pool = false;
            FR(r2, r-1, r+1) FR(c2, c-1, c+1) {
                int black_count = 0;
                FR(r3, r2, r2+2) FR(c3, c2, c2+2) {
                    if (0 <= r3 && r3 < R && 0 <= c3 && c3 < C && grid[r3][c3] == CellType::BLACK) ++black_count;
                }
                if (black_count == 3) forms_pool = true;
            }

            if (has_adj_black && !forms_pool) legal.push_back(make_tuple(r,c));
        }

        if (legal.empty()) break;

        int i = RandInt(0, legal.size());

        auto [r, c] = legal[i];
        grid[r][c] = CellType::BLACK;
    }
}

int main()
{
    InitPRNG();

    FOR(r,R) FOR(c,C) grid[r][c] = CellType::WHITE;

    rand_fill_grid();

    FOR(r,R) {
        FOR(c,C) {
            char ch = '.';
            if (grid[r][c] == CellType::BLACK) ch = '#';
            printf("%c", ch);
        }
        printf("\n");
    }
}
