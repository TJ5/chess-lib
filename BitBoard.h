#ifndef BITBOARD
#define BITBOARD

#include <string>

typedef unsigned long long pieceSet;
class BitBoard {
    public:
        /**
         * @brief Construct a new Bit Board object with default FEN
         * 
         */
        BitBoard();
        /**
         * @brief Construct a new Bit Board object
         * 
         * @param copy 
         */
        BitBoard(BitBoard &copy);
        /**
         * @brief Construct a new Bit Board object from board FEN
         * does not handle side to move, castling rights, turn number
         * 
         * @param fen 
         */
        BitBoard(std::string fen);
        /**
         * @brief return array of legal child positions
         * 
         * @param color 
         * @param moves 
         * @return BitBoard* 
         */
        BitBoard* getLegalBoards(int color, int* moves);
        
        static pieceSet byteswap(pieceSet in);
        static pieceSet bitswap(pieceSet in);
        
        pieceSet getDiagonalMask(int sq);
        pieceSet getAntiDiagonalMask(int sq);
        pieceSet getKnightMask(int sq);
        
        
        
        std::string fen();

        pieceSet color[2]; //color[0] is white, color[1] is black
        pieceSet pieces[6][2];
        

    private:
        
        /*
        pieceSet bishop[2];
        pieceSet knight[2];
        pieceSet rook[2];
        pieceSet queen[2];
        pieceSet king[2];
        pieceSet pawn[2];
        */
       //pawn, rook, knight, bishop, queen, king

        pieceSet rookMoves(pieceSet, pieceSet, int , int );
        pieceSet bishopMoves(pieceSet, pieceSet rev_occ, int square, int color);
        pieceSet queenMoves(pieceSet occ, pieceSet rev_occ, int square, int color);
        pieceSet knightMoves(pieceSet occ, pieceSet rev_occ, int square, int color);
        pieceSet pawnCaptures(pieceSet occ, pieceSet rev_occ, int square, int color);
        pieceSet kingMoves(pieceSet occ, pieceSet rev_occ, int square, int color);

        pieceSet pawnPushes(pieceSet occ, int color);

        pieceSet diagonalPins(pieceSet occ, pieceSet rev_occ, int color);

        
        int getHighestSquare(pieceSet attackSet);
        int getLowestSquare(pieceSet attackSet);
        int getNumPieces(pieceSet pieceSet);
        

        
};
#endif