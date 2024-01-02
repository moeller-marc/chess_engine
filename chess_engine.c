// import libraries
#include <stdio.h>

// define constants
#define pawn_value 100
#define bishop_value 350
#define knight_value 300
#define rook_value 500
#define queen_value 900
#define king_value 10000000

// define macros
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define clear_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

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

#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
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

int main(int argc, char *argv[])
{
    // setup board
    fen_to_bitboard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}