// import libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

// define types
typedef unsigned long long U64;

// define enums
enum colour
{
    WHITE,
    BLACK
};
// define constants
#define pawn_value 100
#define bishop_value 350
#define knight_value 300
#define rook_value 500
#define queen_value 900
#define king_value 10000000

const U64 file_a = 0x0101010101010101Ull;
const U64 file_b = 0x0202020202020202Ull;
const U64 file_c = 0x0404040404040404Ull;
const U64 file_d = 0x0808080808080808Ull;
const U64 file_e = 0x1010101010101010Ull;
const U64 file_f = 0x2020202020202020Ull;
const U64 file_g = 0x4040404040404040Ull;
const U64 file_h = 0x8080808080808080Ull;

const U64 rank_1 = 0xFF;
const U64 rank_2 = 0xFF00;
const U64 rank_3 = 0xFF0000;
const U64 rank_4 = 0xFF000000;
const U64 rank_5 = 0xFF00000000;
const U64 rank_6 = 0xFF0000000000;
const U64 rank_7 = 0xFF000000000000;
const U64 rank_8 = 0xFF00000000000000;

// define macros
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define clear_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

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

    printf("\n\n\n");
    printf("%d\n", __builtin_popcountll(bitboard));
    printf("%llu\n", bitboard);
}

// get least significant 1. bit
static inline int get_lsb(U64 bitboard)
{
    return __builtin_ctzll(bitboard);
}

void fen_to_bitboard(char *fen)
{
    int rank = 7, file = 0, i = 0;
    while (fen[i] != '\0')
    {
        if (fen[i] >= '1' && fen[i] <= '8')
        {
            file += fen[i] - '0'; // skip empty squares
        }
        else if (fen[i] == '/')
        {
            rank--;
            file = 0;
        }
        else
        {
            int index = rank * 8 + file;
            switch (fen[i])
            {
            case 'P':
                white_pawns |= (1ULL << index);
                break;
            case 'B':
                white_bishops |= (1ULL << index);
                break;
            case 'N':
                white_knights |= (1ULL << index);
                break;
            case 'R':
                white_rooks |= (1ULL << index);
                break;
            case 'Q':
                white_queens |= (1ULL << index);
                break;
            case 'K':
                white_king |= (1ULL << index);
                break;
            case 'p':
                black_pawns |= (1ULL << index);
                break;
            case 'b':
                black_bishops |= (1ULL << index);
                break;
            case 'n':
                black_knights |= (1ULL << index);
                break;
            case 'r':
                black_rooks |= (1ULL << index);
                break;
            case 'q':
                black_queens |= (1ULL << index);
                break;
            case 'k':
                black_king |= (1ULL << index);
                break;
            default:
                break;
            }
            file++;
        }
        i++;
    }
}

int static_evaluation(U64 white_pawns, U64 white_bishops, U64 white_knights, U64 white_rooks, U64 white_queens, U64 white_king, U64 black_pawns, U64 black_bishops, U64 black_knights, U64 black_rooks, U64 black_queens, U64 black_king)
{
    int score = 0;
    score += __builtin_popcountll(white_pawns) * pawn_value;
    score += __builtin_popcountll(white_bishops) * bishop_value;
    score += __builtin_popcountll(white_knights) * knight_value;
    score += __builtin_popcountll(white_rooks) * rook_value;
    score += __builtin_popcountll(white_queens) * queen_value;
    score += __builtin_popcountll(white_king) * king_value;

    score -= __builtin_popcountll(black_pawns) * pawn_value;
    score -= __builtin_popcountll(black_bishops) * bishop_value;
    score -= __builtin_popcountll(black_knights) * knight_value;
    score -= __builtin_popcountll(black_rooks) * rook_value;
    score -= __builtin_popcountll(black_queens) * queen_value;
    score -= __builtin_popcountll(black_king) * king_value;

    return score;
}

// attack tables, attack masks and magic bitboards

// attack masks
U64 generate_pawn_attack_mask(int colour, int square)
{
    U64 mask = 0ULL;
    if (colour == WHITE)
    {
        if (!(get_bit(file_a, square - 7))) // not on file a
        {
            set_bit(mask, square - 7);
        }
        if (!(get_bit(file_h, square - 9))) // not on file h
        {
            set_bit(mask, square - 9);
        }
    }
    else
    {
        if (!(get_bit(file_h, square + 7))) // not on file h
        {
            printf("%d\n", !(get_bit(file_h, square + 7)));
            set_bit(mask, square + 7);
        }
        if (!(get_bit(file_a, square + 9))) // not on file a
        {
            printf("%d\n", !(get_bit(file_a, square + 9)));
            set_bit(mask, square + 9);
            printf("%d\n", square + 9);
        }
    }
    return mask;
}

U64 generate_bishop_attack_mask(int square)
{
    U64 mask = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    int i = 1;
    while (rank + i < 8 && file + i < 8)
    {
        set_bit(mask, square + i * 9);
        i++;
    }
    i = 1;
    while (rank - i >= 0 && file + i < 8)
    {
        set_bit(mask, square - i * 7);
        i++;
    }
    i = 1;
    while (rank + i < 8 && file - i >= 0)
    {
        set_bit(mask, square + i * 7);
        i++;
    }
    i = 1;
    while (rank - i >= 0 && file - i >= 0)
    {
        set_bit(mask, square - i * 9);
        i++;
    }
    return mask;
}

U64 generate_knight_attack_mask(int square)
{
    U64 mask = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    if (rank + 2 < 8 && file + 1 < 8)
    {
        set_bit(mask, square + 17);
    }
    if (rank + 2 < 8 && file - 1 >= 0)
    {
        set_bit(mask, square + 15);
    }
    if (rank - 2 >= 0 && file + 1 < 8)
    {
        set_bit(mask, square - 15);
    }
    if (rank - 2 >= 0 && file - 1 >= 0)
    {
        set_bit(mask, square - 17);
    }
    if (rank + 1 < 8 && file + 2 < 8)
    {
        set_bit(mask, square + 10);
    }
    if (rank + 1 < 8 && file - 2 >= 0)
    {
        set_bit(mask, square + 6);
    }
    if (rank - 1 >= 0 && file + 2 < 8)
    {
        set_bit(mask, square - 6);
    }
    if (rank - 1 >= 0 && file - 2 >= 0)
    {
        set_bit(mask, square - 10);
    }
    return mask;
}

U64 generate_rook_attack_mask(int square)
{
    U64 mask = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    int i = 1;
    while (rank + i < 8)
    {
        set_bit(mask, square + i * 8);
        i++;
    }
    i = 1;
    while (rank - i >= 0)
    {
        set_bit(mask, square - i * 8);
        i++;
    }
    i = 1;
    while (file + i < 8)
    {
        set_bit(mask, square + i);
        i++;
    }
    i = 1;
    while (file - i >= 0)
    {
        set_bit(mask, square - i);
        i++;
    }
    return mask;
}

U64 generate_queen_attack_mask(int square)
{
    U64 mask = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    int i = 1;
    while (rank + i < 8)
    {
        set_bit(mask, square + i * 8);
        i++;
    }
    i = 1;
    while (rank - i >= 0)
    {
        set_bit(mask, square - i * 8);
        i++;
    }
    i = 1;
    while (file + i < 8)
    {
        set_bit(mask, square + i);
        i++;
    }
    i = 1;
    while (file - i >= 0)
    {
        set_bit(mask, square - i);
        i++;
    }
    i = 1;
    while (rank + i < 8 && file + i < 8)
    {
        set_bit(mask, square + i * 9);
        i++;
    }
    i = 1;
    while (rank - i >= 0 && file + i < 8)
    {
        set_bit(mask, square - i * 7);
        i++;
    }
    i = 1;
    while (rank + i < 8 && file - i >= 0)
    {
        set_bit(mask, square + i * 7);
        i++;
    }
    i = 1;
    while (rank - i >= 0 && file - i >= 0)
    {
        set_bit(mask, square - i * 9);
        i++;
    }
    return mask;
}

U64 generate_king_attack_mask(int square)
{
    U64 mask = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    if (rank + 1 < 8)
    {
        set_bit(mask, square + 8);
    }
    if (rank - 1 >= 0)
    {
        set_bit(mask, square - 8);
    }
    if (file + 1 < 8)
    {
        set_bit(mask, square + 1);
    }
    if (file - 1 >= 0)
    {
        set_bit(mask, square - 1);
    }
    if (rank + 1 < 8 && file + 1 < 8)
    {
        set_bit(mask, square + 9);
    }
    if (rank + 1 < 8 && file - 1 >= 0)
    {
        set_bit(mask, square + 7);
    }
    if (rank - 1 >= 0 && file + 1 < 8)
    {
        set_bit(mask, square - 7);
    }
    if (rank - 1 >= 0 && file - 1 >= 0)
    {
        set_bit(mask, square - 9);
    }
    return mask;
}

// attack tables
U64 pawn_attack_table[2][64];
U64 bishop_attack_table[64];
U64 knight_attack_table[64];
U64 rook_attack_table[64];
U64 queen_attack_table[64];
U64 king_attack_table[64];

int main(int argc, char *argv[])
{
    // setup board
    fen_to_bitboard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    // print_bitboard(generate_bishop_attack_mask(0));
}