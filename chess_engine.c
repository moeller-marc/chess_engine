// import libraries
#include <stdio.h>

// define macros
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))

// define types
typedef unsigned long long U64;
// the board
U64 white_pawns;
U64 white_bishops;
U64 white_knights;
U64 white_rooks;
U64 white_queens;
U64 white_king;

U64 black_pawns;
U64 black_bishops;
U64 black_knights;
U64 black_rooks;
U64 black_queens;
U64 black_king;

void print_bitboard(U64 bitboard)
{
    for (int i = 0; i < 8; i++)
    {
        printf("|");
        for (int j = 0; j < 8; j++)
        {
            int square = i * 8 + j;
            printf("%d|", (get_bit(bitboard, square) ? 1 : 0));
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
}