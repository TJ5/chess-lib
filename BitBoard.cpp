#include "BitBoard.h"
#include <cmath>

static unsigned long long singleMask[64]; 
static unsigned long long verticalMask[64];
static unsigned long long diagonalMask[64];
static unsigned long long horizontalMask[64];
static unsigned long long antidiagonalMask[64];
static unsigned long long knightMask[64];
static unsigned long long pawnCaptureMask[2][64];
static unsigned long long kingMask[64];
static bool initialized = false;
BitBoard::BitBoard() {
    //initialize starting pos
    BitBoard::color[0] = 0x000000000000ffff;
    BitBoard::color[1] = 0xffff000000000000;
    BitBoard::pawn[0] = 0x000000000000ff00;
    BitBoard::pawn[1] = 0x00ff000000000000;
    BitBoard::knight[0] = 0x0000000000000042;
    BitBoard::knight[1] = 0x4200000000000000;
    BitBoard::bishop[0] = 0x0000000000000024;
    BitBoard::bishop[1] = 0x2400000000000000;
    BitBoard::rook[0] = 0x0000000000000081;
    BitBoard::rook[1] = 0x8100000000000000;
    BitBoard::queen[0] = 0x0000000000000008;
    BitBoard::queen[1] = 0x0800000000000000;
    BitBoard::king[0] = 0x0000000000000010;
    BitBoard::king[1] = 0x1000000000000000;

    //generate masks just once for all bitboards:
    if (!(initialized)) {
        initialized = true;
        unsigned long long mask = 0x0000000000000001;
        unsigned long long hmask = 0x00000000000000ff; //the first row
        unsigned long long vmask = 0x0101010101010101; //first column
        unsigned long long dmask = 0x0000000000000080; //diagonal mask of h1
        unsigned long long admask = 0x0000000000000001; //antidiagonal mask of a1
        singleMask[0] = mask;
        horizontalMask[0] = hmask;
        //Vertical mask:
        for (int i = 0; i < 8; i++) {
            //loop through each column
            for (int j = i; j <= (56 + i); j+= 8) {
                verticalMask[j] = vmask;
            }
            vmask = vmask << 1;
        }
        
        
        //horizontal and single square masks:
        for (int j = 1; j < 64; j++) {
            mask = mask << 1;
            singleMask[j] = mask;
            vmask = vmask << 1;
            if (j % 8 == 0) {
                hmask = hmask << 8;
            }
            horizontalMask[j] = hmask;
        }
        //Diagonal mask:
        //use 2 loops, h8 to a1, then a8 to a2
        //inner loops traverse each diagonal starting from above squares
        int dIndex = 15;
        for (int i = 7; i >= 0; i--) { 
            int square = i;
            for (int j = 8; j > i; j--) {
                diagonalMask[square] = dmask;
                square += 9;
            }
            dmask = dmask >> 1;
            dmask += singleMask[dIndex];
            dIndex += 8;

        }
        dIndex = 57;
        dmask = 0x0100000000000000;
        for (int i = 7; i > 0; i--) { 
            int square = 56 - (8 * (7 - i));
            for (int j = 8; j > i; j--) {
                diagonalMask[square] = dmask;
                square += 9;
            }
            dmask = dmask >> 8;
            dmask += singleMask[dIndex];
            dIndex++;

        }
        //Antidiagonal mask:
        //Two loops again, first one starts from a1 to a8, then h8 to h1
        int adIndex = 8;
        for (int i = 0; i < 8; i++) {
            int square = i;
            for (int j = 0; j <= i; j++) {
                antidiagonalMask[square] = admask;
                square += 7;
            }
            admask = admask << 1;
            admask += singleMask[adIndex];
            adIndex += 8;

        }
        adIndex = 55;
        admask = 0x8000000000000000; //h8
        for (int i = 0; i < 7; i++) { 
            int square = 63 - i;
            for (int j = i; j >= 0; j--) {
                antidiagonalMask[square] = admask;
                square -= 7;
            }
            admask = admask >> 1;
            admask += singleMask[adIndex];
            adIndex -= 8;

        }

        //Knight mask:
        unsigned long long kmask = 0x0;
        
        for(int i = 0; i < 64; i++) {
            kmask = 0x0;
            //check knight's location for each 8 possible moves to 
            //make sure its attacks are not wrapping around the board
            
            //Last two columns
            if (!(singleMask[i] & verticalMask[6])) {
                if (!(singleMask[i] & verticalMask[7])) {
                    //top row
                    if (!(singleMask[i] & horizontalMask[63])) {
                        kmask ^= singleMask[i + 10];
                    }
                    //bottom row
                    if (!(singleMask[i] & horizontalMask[7])) {
                        kmask ^= singleMask[i - 6];
                    }
                }
            }
            //First two columns
            if (!(singleMask[i] & verticalMask[0])) {
                if (!(singleMask[i] & verticalMask[1])) {
                    //top row
                    if (!(singleMask[i] & horizontalMask[63])) {
                        kmask ^= singleMask[i + 6];
                    }
                    if (!(singleMask[i] & horizontalMask[7])) {
                        kmask ^= singleMask[i - 10];
                    }
                }
            }

            //top two rows
            if (!(singleMask[i] & horizontalMask[63])) {
                if (!(singleMask[i] & horizontalMask[55])) {
                    //last column
                    if (!(singleMask[i] & verticalMask[63])) {
                        kmask ^= singleMask[i + 17];
                    }
                    //first column
                    if (!(singleMask[i] & verticalMask[0])) {
                        kmask ^= singleMask[i + 15];
                    }
                }
            }
            //bottom two rows
            if (!(singleMask[i] & horizontalMask[0])) {
                if (!(singleMask[i] & horizontalMask[8])) {
                    //last column
                    if (!(singleMask[i] & verticalMask[63])) {
                        kmask ^= singleMask[i - 15];
                    }
                    //first column
                    if (!(singleMask[i] & verticalMask[0])) {
                        kmask ^= singleMask[i - 17];
                    }
                }
            }
            knightMask[i] = kmask;
            
        }

        //Pawn captures:
        //0th index is for white pawns, 1st index is for black pawns
        unsigned long long pawnCapture = 0x0;
        
        for (int i = 0; i < 64; i++) {  
            pawnCapture = 0x0;
            if (i >= 56 || i < 8) { //captures illegal on top row for white
                pawnCaptureMask[0][i] = pawnCapture;
                continue;
            }

            if (!(verticalMask[0] & singleMask[i])) { //i is not on first column
                pawnCapture ^= singleMask[i + 7];
                
            }
            if (!(verticalMask[7] & singleMask[i])) { //i is not on last column
                pawnCapture ^= singleMask[i + 9];
            }
            pawnCaptureMask[0][i] = pawnCapture;
            
        }
        for (int i = 0; i < 64; i++) {
            pawnCapture = 0x0;
            if (i >= 56 || i < 8) { 
                pawnCaptureMask[1][i] = pawnCapture;
                continue;
            }
            if (!(verticalMask[0] & singleMask[i])) { //i is not on first column
                pawnCapture ^= singleMask[i - 9];
                
            }
            if (!(verticalMask[7] & singleMask[i])) { //i is not on last column
                pawnCapture ^= singleMask[i - 7];
            }
            pawnCaptureMask[1][i] = pawnCapture;
            
        }
        //King Mask: 
        
        for (int i = 0; i < 64; i++) {
            kmask = 0x0;
            //not first column
            if (!(singleMask[i] & verticalMask[0])) {
                kmask ^= singleMask[i - 1];
                if (!(singleMask[i] & horizontalMask[0])) {
                    kmask ^= singleMask[i - 9];
                    kmask ^= singleMask[i - 8];
                }
                if (!(singleMask[i] & horizontalMask[56])) {
                    kmask ^= singleMask[i + 7];
                    kmask ^= singleMask[i + 8];
                }
            }
            if (!(singleMask[i] & verticalMask[7])) {
                kmask ^= singleMask[i + 1];
                if (!(singleMask[i] & horizontalMask[0])) {
                    kmask ^= singleMask[i - 7];
                    kmask ^= singleMask[i - 8];
                }
                if (!(singleMask[i] & horizontalMask[56])) {
                    kmask ^= singleMask[i + 9];
                    kmask ^= singleMask[i + 8];
                }
            }
            kingMask[i] = kmask;
        }
    }    
}

BitBoard::BitBoard(BitBoard &copy) {
    //Copy constructor, initialize a new bitboard identical to the current one
    initialized = true;
    BitBoard::color[0] = copy.color[0];
    BitBoard::color[1] = copy.color[1];
    BitBoard::pawn[0] = copy.pawn[0];
    BitBoard::pawn[1] = copy.pawn[1];
    BitBoard::knight[0] = copy.knight[0];
    BitBoard::knight[1] = copy.knight[1];
    BitBoard::bishop[0] = copy.bishop[0];
    BitBoard::bishop[1] = copy.bishop[1];
    BitBoard::rook[0] = copy.rook[0];
    BitBoard::rook[1] = copy.rook[1];
    BitBoard::queen[0] = copy.queen[0];
    BitBoard::queen[1] = copy.queen[1];
    BitBoard::king[0] = copy.king[0];
    BitBoard::king[1] = copy.king[1];

}

BitBoard** BitBoard::getLegalBoards(int color) {
    //Returns an array of pointers to all legal child bitboards of the
    //current position.

    //Loop through each piece type of side to move
    
    unsigned long long occupied = BitBoard::color[0] | BitBoard::color[1];
    unsigned long long rev_occupied = byteswap(occupied);
    unsigned long long slider, rev_slider;
    int oppositeColor = !((bool)(color));
    unsigned long long tempPiece;
    unsigned long long tempColor = BitBoard::color[color];
    unsigned long long tempOppCol, tempOppKnight, tempOppPawn, tempOppQueen, tempOppKing, 
    tempOppRook, tempOppBishop;
    //Dupicate pawn here because pawns have to be evaluated twice for pushes and attacks
    unsigned long long pieces[7] = 
    {BitBoard::pawn[color], BitBoard::pawn[color], 
    BitBoard::knight[color], 
    BitBoard::rook[color], BitBoard::queen[color], 
    BitBoard::bishop[color], BitBoard::king[color]};

    unsigned long long (BitBoard::*pieceAttacks[7])(unsigned long long, unsigned long long, int, int);
    pieceAttacks[0] = &BitBoard::pawnCaptures;
    pieceAttacks[1] = &BitBoard::pawnPushes;
    pieceAttacks[2] = &BitBoard::knightMoves;
    pieceAttacks[3] = &BitBoard::rookMoves;
    pieceAttacks[4] = &BitBoard::queenMoves;
    pieceAttacks[5] = &BitBoard::bishopMoves;
    pieceAttacks[6] = &BitBoard::kingMoves;

    //For each piece type:
    for (int i = 0; i < 6; i++) {
        
        //For each existent piece of that type on the board:
        for (int j = BitBoard::getNumPieces(tempPiece); j > 0; j--) {
            //Create temp vars for anything that can change during the move
            //This includes the long for the piece moving, the set of all pieces of the side to move,
            //And any enemy pieces which can be captured.
            //These longs will then be used to initialize the new "child" bitboard.
            tempPiece = pieces[i];
            tempColor = BitBoard::color[color];
            tempOppCol = BitBoard::color[oppositeColor];
            tempOppPawn = BitBoard::pawn[oppositeColor];
            tempOppRook = BitBoard::rook[oppositeColor];
            tempOppKnight = BitBoard::knight[oppositeColor];
            tempOppBishop = BitBoard::bishop[oppositeColor];
            tempOppQueen = BitBoard::queen[oppositeColor];
            tempOppKing = BitBoard::king[oppositeColor];
            int square = BitBoard::getLowestSquare(tempPiece);

            unsigned long long attack = (*this.*pieceAttacks[i])(occupied, rev_occupied, square, color);
            //we now want to make new boards for each possible attack of the piece
            int destSquare = BitBoard::getLowestSquare(attack);
            tempPiece ^= singleMask[destSquare];
            tempPiece ^= singleMask[square];
            //tempPiece now stores the new value of the moved piece

            tempColor ^= singleMask[destSquare];
            tempColor ^= singleMask[square];

            //in the event of a capture, we must remove the relevant bits from the opposite color
            //Bitwise AND with set of occupied squares of side to move and all other fields with 
            //opposite side gives the sets of squares with multiple values
            //We then subtract these out.
            tempOppCol -= (tempOppCol & tempColor);
            tempOppPawn -=(tempOppPawn & tempColor);
            tempOppRook -=(tempOppRook & tempColor);
            tempOppKnight -=(tempOppKnight & tempColor);
            tempOppBishop -=(tempOppBishop & tempColor);
            tempOppQueen -=(tempOppQueen & tempColor);
            tempOppKing -=(tempOppKing & tempColor);
            
            BitBoard child (*this);
            child.color[color] = tempColor;
            
        }
    }
    
    

    
    




    //temporary code, does nothing
    BitBoard s(*this);
    BitBoard *ret[1];
    ret[0] = &s;
    return ret;
}

unsigned long long BitBoard::byteswap(unsigned long long in) {
    //Reverses bits in long by swapping each
    //bit with its neighbor, then swapping neighbors, and so on
    
    
    //swaps bytes
    in = (((in & 0xff00ff00ff00ff00) >> 8) | ((in & 0x00ff00ff00ff00ff) << 8));
    //swaps 2 bytes
    in = (((in & 0xffff0000ffff0000) >> 16) | ((in & 0x0000ffff0000ffff) << 16));
    //swaps 4 bytes
    in = (((in & 0xffffffff00000000) >> 32) | ((in & 0x00000000ffffffff) << 32));
    return in;
}

unsigned long long BitBoard::rookMoves(unsigned long long occ, unsigned long long rev_occ, int square, int color) {
    unsigned long long rookAttacks = 0x0;
    unsigned long long slider = BitBoard::color[color] & singleMask[square];
    unsigned long long rev_slider = byteswap(slider);
    unsigned long long up, down;
    //these are positive and negative horizontal "ray" attacks,
    //which begin from squares adjacent to the rook and 
    //stop at the square of another piece


    //Take the rook, bitshift left, and subtract from the set of other occupied squares
    //The bits that change were vacant so they are legal, so we XOR to isolate them
    //repeat in reverse for ray attacks to the left
    rookAttacks = rookAttacks ^ ((occ - (slider << 1)) ^ byteswap(rev_occ - (rev_slider << 1)));
    //Now, remove pieces of the same color as self captures are illegal
    rookAttacks = rookAttacks ^ (horizontalMask[square] & BitBoard::color[color] & rookAttacks);
    
    //Vertical attacks:
    up = (occ & verticalMask[square]) - slider;
    down = byteswap(up);
    up -= slider;
    down -= rev_slider;
    up = up ^ byteswap(down); 
    up = up & verticalMask[square];
    rookAttacks = rookAttacks ^ up;
    //remove self captures
    rookAttacks = rookAttacks ^ (verticalMask[square] & BitBoard::color[color] & rookAttacks);
    return rookAttacks;
}

unsigned long long BitBoard::bishopMoves(unsigned long long occ, unsigned long long rev_occ, int square, int color) {
    unsigned long long bishopAttacks = 0x0;
    unsigned long long slider = BitBoard::color[color] & singleMask[square];
    unsigned long long rev_slider = byteswap(slider);
    unsigned long long up, down;
    //Bishop moves are union of diagonal and antidiagonal attacks
    //Each of these are done in the same way vertical attacks are done
    //Diagonal:
    up = (occ & diagonalMask[square]) - slider;
    down = byteswap(up);
    up -= slider;
    down -= byteswap(slider);
    up = up ^ byteswap(down);
    bishopAttacks = bishopAttacks ^ (up & diagonalMask[square]);
    
    //Remove self captures:
    bishopAttacks = bishopAttacks ^ (diagonalMask[square] & BitBoard::color[color] & bishopAttacks);
    
    //Antidiagonal:
    up = (occ & antidiagonalMask[square]) - slider;
    down = byteswap(up);
    up -= slider;
    down -= byteswap(slider);
    up = up ^ byteswap(down);
    bishopAttacks = bishopAttacks ^ (up & antidiagonalMask[square]);
    
    //Remove self captures:
    bishopAttacks = bishopAttacks ^ (antidiagonalMask[square] & BitBoard::color[color] & bishopAttacks);
    
    return bishopAttacks;
}

unsigned long long BitBoard::queenMoves(unsigned long long occ, unsigned long long rev_occ, int square, int color) {
    //Where queen attacks are simply the union of rook and 
    //bishop attacks
    unsigned long long queenAttacks = 0x0;


    queenAttacks = queenAttacks ^ bishopMoves(occ, rev_occ, square, color);
    queenAttacks = queenAttacks ^ rookMoves(occ, rev_occ, square, color);
    return queenAttacks;
}

unsigned long long BitBoard::knightMoves(unsigned long long occ, 
        unsigned long long rev_occ, int square, int color) {
    unsigned long long knightAttacks = knightMask[square];
    //Now just remove self captures by XOR'ing out common bits between 
    //The set of pieces of the same color and the squares the knight attacks
    knightAttacks ^= (knightAttacks & BitBoard::color[color]); 
    return knightAttacks;
}

unsigned long long BitBoard::getDiagonalMask(int square) {
    return diagonalMask[square];
}

unsigned long long BitBoard::getAntiDiagonalMask(int sq) {
    return antidiagonalMask[sq];
}

unsigned long long BitBoard::getKnightMask(int sq) {
    return knightMask[sq];
}

unsigned long long BitBoard::pawnPushes(unsigned long long occ, 
        unsigned long long rev_occ, int square, int color) {
    //pawn movement in general is the union of pushes and captures
    //pushes are either by two squares or one, so we calculate both possibilities
    unsigned long long empty_squares = ~(BitBoard::color[0] | BitBoard::color[1]);
    unsigned long long singlepushes;
    unsigned long long doublepushes;
    if (color == 0) { //white
        //empty squares immediately ahead of the pawn
        singlepushes = (BitBoard::pawn[0] << 8) & empty_squares; 
        //double pushes:
        const unsigned long long rank4 = 0x00000000FF000000;
        doublepushes = (singlepushes << 8) & empty_squares & rank4;
        return singlepushes | doublepushes;
    } 
    //black
    singlepushes = (BitBoard::pawn[0] >> 8) & empty_squares; 
    const unsigned long long rank5 = 0x000000FF00000000;
    doublepushes = (singlepushes >> 8) & empty_squares & rank5;
    return singlepushes | doublepushes;

}
unsigned long long BitBoard::pawnCaptures(unsigned long long occ, 
        unsigned long long rev_occ, int square, int color) {
    //Look up square in pawn capture lookup table and bitwise and
    //the result with the set of occupied squares of the opposite color
    unsigned long long captureMask = pawnCaptureMask[color][square];
    return captureMask & BitBoard::color[!((bool)(color))]; 
}

unsigned long long BitBoard::kingMoves(unsigned long long occ, 
        unsigned long long rev_occ, int square, int color) {
    //Does not include castling
    unsigned long long kingAttacks = kingMask[square];
    //remove self captures:
    kingAttacks ^= (kingAttacks & BitBoard::color[color]); 
    return kingAttacks;
}

int BitBoard::getLowestSquare(unsigned long long attackSet) {
    //given 0b10001, returns 0, for the index of the least significant set bit
    //used for iterating over squares of an attack set
    return log2(attackSet & -(attackSet));
}

int BitBoard::getNumPieces(unsigned long long pieceSet) {
    //returns number of set bits by Kernighan's algorithm
    int count;
    for (count = 0; pieceSet; count++) {
        pieceSet &= pieceSet - 1;
    }
    return count;
}