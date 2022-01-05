#ifndef BITBOARD
#define BITBOARD


class BitBoard {
    public:
        BitBoard();
        BitBoard(BitBoard &copy);
        BitBoard** getLegalBoards(int color);
        static unsigned long long byteswap(unsigned long long in);
        unsigned long long getDiagonalMask(int sq);
        unsigned long long getAntiDiagonalMask(int sq);
        unsigned long long getKnightMask(int sq);
        
        //public only for testing:
        unsigned long long rookMoves(unsigned long long, 
        unsigned long long, int , int );
        
        unsigned long long bishopMoves(unsigned long long, 
        unsigned long long rev_occ, int square, int color);

        unsigned long long queenMoves(unsigned long long occ, 
        unsigned long long rev_occ, int square, int color);

        unsigned long long knightMoves(unsigned long long occ, 
        unsigned long long rev_occ, int square, int color);
        
        unsigned long long pawnPushes(unsigned long long occ, 
        unsigned long long rev_occ, int square, int color);
        
        unsigned long long pawnCaptures(unsigned long long occ, 
        unsigned long long rev_occ, int square, int color);

        unsigned long long kingMoves(unsigned long long occ, 
        unsigned long long rev_occ, int square, int color);

        unsigned long long color[2]; //color[0] is white, color[1] is black
        unsigned long long pieces[6][2];
    private:
        
        /*
        unsigned long long bishop[2];
        unsigned long long knight[2];
        unsigned long long rook[2];
        unsigned long long queen[2];
        unsigned long long king[2];
        unsigned long long pawn[2];
        */
       //pawn, rook, knight, bishop, queen, king

        int getHighestSquare(unsigned long long attackSet);
        int getNumPieces(unsigned long long pieceSet);
        

        

        
};
#endif