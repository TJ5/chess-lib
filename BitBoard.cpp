#include "BitBoard.h"
#include <cmath>
#include <string>


static unsigned long long singleMask[64]; 
static unsigned long long verticalMask[64];
static unsigned long long diagonalMask[64];
static unsigned long long horizontalMask[64];
static unsigned long long antidiagonalMask[64];
static unsigned long long knightMask[64];
static unsigned long long pawnCaptureMask[2][64];
static unsigned long long kingMask[64];
static unsigned long long zeroMask[2][64];
static bool initialized = false;
BitBoard::BitBoard() {
    //initialize starting pos
    BitBoard::color[0] = 0x000000000000ffff;
    BitBoard::color[1] = 0xffff000000000000;
    BitBoard::pieces[0][0] = 0x000000000000ff00;
    BitBoard::pieces[0][1] = 0x00ff000000000000;
    BitBoard::pieces[2][0] = 0x0000000000000042;
    BitBoard::pieces[2][1] = 0x4200000000000000;
    BitBoard::pieces[3][0] = 0x0000000000000024;
    BitBoard::pieces[3][1] = 0x2400000000000000;
    BitBoard::pieces[1][0] = 0x0000000000000081;
    BitBoard::pieces[1][1] = 0x8100000000000000;
    BitBoard::pieces[4][0] = 0x0000000000000008;
    BitBoard::pieces[4][1] = 0x0800000000000000;
    BitBoard::pieces[5][0] = 0x0000000000000010;
    BitBoard::pieces[5][1] = 0x1000000000000000;

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
            

            //Zero mask: used for pawn pushes,
            //all bits of n or greater significance are 0, all others are 1
            //for white, inverse for black
            unsigned long long zMask = 0xffffffffffffffff;
            for (int i = 63; i >= 0; i--) {
                zeroMask[1][i] = ~zMask;
                zMask ^= singleMask[i];
                zeroMask[0][i] = zMask;
                
            }
            
            
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
    /**Copy constructor, 
     * initialize a new bitboard identical to the current one
     */
    initialized = true;
    BitBoard::color[0] = copy.color[0];
    BitBoard::color[1] = copy.color[1];
    BitBoard::pieces[0][0] = copy.pieces[0][0];
    BitBoard::pieces[0][1] = copy.pieces[0][1];
    BitBoard::pieces[2][0] = copy.pieces[2][0];
    BitBoard::pieces[2][1] = copy.pieces[2][1];
    BitBoard::pieces[3][0] = copy.pieces[3][0];
    BitBoard::pieces[3][1] = copy.pieces[3][1];
    BitBoard::pieces[1][0] = copy.pieces[1][0];
    BitBoard::pieces[1][1] = copy.pieces[1][1];
    BitBoard::pieces[4][0] = copy.pieces[4][0];
    BitBoard::pieces[4][1] = copy.pieces[4][1];
    BitBoard::pieces[5][0] = copy.pieces[5][0];
    BitBoard::pieces[5][1] = copy.pieces[5][1];

}

BitBoard::BitBoard(std::string fen) {
    /**
     * Initializes bitboard from board fen string
     * The fen should not include information such as turn and castling rights
     * 
     */
    //initialize all fields to zero
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            BitBoard::pieces[i][j] = 0x0;
        }
    }
    
    //Break fen up into rows
    std::string rows[8];
    for (int i = 0; i < 8; i++) {
        rows[i] = "";
    }

    int r = 7;
    for (int i = 0; i < fen.length(); i++) {
        if (fen.at(i) == '/') {
            r--;
            continue;
        }
        rows[r] += fen.at(i);
    }


    int square = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < rows[i].length(); j++) {
            char piece = rows[i].at(j);
            int col;
            if ((int)(piece) <= 57 && (int)(piece) >= 48) {
                square += (int)(piece) - 48;
                continue;
            }
            

            switch(piece) {
                case 'R' :
                    BitBoard::pieces[1][0] |= singleMask[square];
                    break;
                case 'N' :
                    BitBoard::pieces[2][0] |= singleMask[square];
                    break;
                case 'B' :
                    BitBoard::pieces[3][0] |= singleMask[square];
                    break;
                case 'Q' :
                    BitBoard::pieces[4][0] |= singleMask[square];
                    break;
                case 'K' :
                    BitBoard::pieces[5][0] |= singleMask[square];
                    break;
                case 'P' :
                    BitBoard::pieces[0][0] |= singleMask[square];
                    break;
                case 'r' :
                    BitBoard::pieces[1][1] |= singleMask[square];
                    break;
                case 'n' :
                    BitBoard::pieces[2][1] |= singleMask[square];
                    break;
                case 'b' :
                    BitBoard::pieces[3][1] |= singleMask[square];
                    break;
                case 'q' :
                    BitBoard::pieces[4][1] |= singleMask[square];
                    break;
                case 'k' :
                    BitBoard::pieces[5][1] |= singleMask[square];
                    break;
                case 'p' :
                    BitBoard::pieces[0][1] |= singleMask[square];
                    break;
            }
            square++;
        }
    }

    BitBoard::color[0] = 0x0, BitBoard::color[1] = 0x0;
    BitBoard::color[0] |= BitBoard::pieces[0][0];
    BitBoard::color[0] |= BitBoard::pieces[1][0];
    BitBoard::color[0] |= BitBoard::pieces[2][0];
    BitBoard::color[0] |= BitBoard::pieces[3][0];
    BitBoard::color[0] |= BitBoard::pieces[4][0];
    BitBoard::color[0] |= BitBoard::pieces[5][0];
    BitBoard::color[1] |= BitBoard::pieces[0][1];
    BitBoard::color[1] |= BitBoard::pieces[1][1];
    BitBoard::color[1] |= BitBoard::pieces[2][1];
    BitBoard::color[1] |= BitBoard::pieces[3][1];
    BitBoard::color[1] |= BitBoard::pieces[4][1];
    BitBoard::color[1] |= BitBoard::pieces[5][1];

}



BitBoard* BitBoard::getLegalBoards(int color, int* moves) {
    /**
     * Returns dynamically allocated array of legal child positions
     * Sets *moves to the length of the array
     * Caller must deallocate when done using
     * 
     * STILL TO BE IMPLEMENTED
     * CHECKS
     * CASTLES
     * EN PASSANT
     * 
     */

    //We need to allocate room for bitboards equal to the number of
    //set bits in all attacksets
    unsigned long long occupied = BitBoard::color[0] | BitBoard::color[1];
    unsigned long long rev_occupied = byteswap(occupied);
    
    int oppositeColor = !((bool)(color));

    
    int numPieces[6];
    int pieceCount = 0;
    for (int i = 0; i < 6; i++) {
        numPieces[i] = BitBoard::getNumPieces(BitBoard::pieces[i][color]);
        pieceCount += numPieces[i];
    }

    //Array of all attack set generation functions, excepting pawnpushes
    unsigned long long (BitBoard::*pieceAttacks[6])(unsigned long long, unsigned long long, int, int);
    pieceAttacks[0] = &BitBoard::pawnCaptures;
    pieceAttacks[1] = &BitBoard::rookMoves;
    pieceAttacks[2] = &BitBoard::knightMoves;
    pieceAttacks[3] = &BitBoard::bishopMoves;
    pieceAttacks[4] = &BitBoard::queenMoves;
    pieceAttacks[5] = &BitBoard::kingMoves;

    int index = 0;
    *moves = 0;
    unsigned long long* attackSets;
    int* indices;
    int* pieceTypes;

    attackSets = new unsigned long long[pieceCount];
    indices = new int[pieceCount];
    pieceTypes = new int[pieceCount];

    unsigned long long pieceIterator;
    unsigned long long att;

    for (int i = 0; i < 6; i++) {
        pieceIterator = BitBoard::pieces[i][color];
        for (int j = 0; j < numPieces[i]; j++) {
            int square = getHighestSquare(pieceIterator);
            pieceIterator -= singleMask[square];
            att = (*this.*pieceAttacks[i])(occupied, rev_occupied, square, color);
            
            indices[index] = square;
            pieceTypes[index] = i;
            attackSets[index++] = att;
            *moves += getNumPieces(att);
        }
    }
    //moves does not currently contain pawnpushes, which are calculated
    //simultaneously for all pawns, so we now add those.
    unsigned long long pawnMoves = pawnPushes(occupied, color);
    int pushes = getNumPieces(pawnMoves);
    *moves += pushes;
    BitBoard* childBoards = new BitBoard[*moves];
    //Now, loop through each attackset to generate new Bitboards
    int m = 0;

    for (int i = 0; i < pieceCount; i++) {
        int attacks = getNumPieces(attackSets[i]);
        pieceIterator = attackSets[i];
        for (int j = 0; j < attacks; j++) {
            int dest_square = getHighestSquare(pieceIterator);
            pieceIterator -= singleMask[dest_square];
            BitBoard child(*this);

            unsigned long long move = BitBoard::pieces[pieceTypes[i]][color];
            unsigned long long col = BitBoard::color[color];
            move ^= singleMask[indices[i]];
            move ^= singleMask[dest_square];
            col ^= singleMask[indices[i]];
            col ^= singleMask[dest_square];
            unsigned long long tempOpp;
            //loop through opposite pieces and subtract collisions for
            //captures
            for (int p = 0; p < 6; p++) {
                tempOpp = BitBoard::pieces[p][oppositeColor];
                tempOpp -= (tempOpp & col);
                child.pieces[p][oppositeColor] = tempOpp;
            }
            child.pieces[pieceTypes[i]][color] = move;
            child.color[color] = col;
            child.color[oppositeColor] -= (child.color[oppositeColor] & child.color[color]);

            childBoards[m++] = child;
            

        }
    }
    
    //Now generating pawn push bitboards
    pieceIterator = pawnMoves;
    for (int i = 0; i < pushes; i++) {
        BitBoard child(*this);
        int dest_square = getHighestSquare(pieceIterator);
        pieceIterator -= singleMask[dest_square];
        
        unsigned long long move = BitBoard::pieces[0][color];
        unsigned long long col = BitBoard::color[color];
        move ^= singleMask[dest_square];
        col ^= singleMask[dest_square];

        //Find source square of pawn move:
        //We take the pawn set and isolate the column of the pawn push with the vertical mask
        //The pawn that was pushed is the most significant bit that is less than the dest square bit
        //So, zero out all bits of greater or equal significance to dest square and get the highest bit
        int square = getHighestSquare(zeroMask[color][dest_square] & verticalMask[dest_square] & BitBoard::pieces[0][color]);

        move ^= singleMask[square];
        col ^= singleMask[square];

        child.pieces[0][color] = move;
        child.color[color] = col;
        childBoards[m++] = child;
    }
    
    



    delete[] indices;
    delete[] attackSets;
    delete[] pieceTypes;
    return childBoards;
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
    rookAttacks ^= ((occ - (slider << 1)) ^ byteswap(rev_occ - (rev_slider << 1)));
    //Now, remove pieces of the same color as self captures are illegal
    rookAttacks &= horizontalMask[square]; //bound to row
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

unsigned long long BitBoard::pawnPushes(unsigned long long occ, int color) {
    //pawn movement in general is the union of pushes and captures
    //pushes are either by two squares or one, so we calculate both possibilities
    unsigned long long empty_squares = ~(occ);
    unsigned long long singlepushes;
    unsigned long long doublepushes;
    if (color == 0) { //white
        //empty squares immediately ahead of the pawn
        singlepushes = (BitBoard::pieces[0][0] << 8) & empty_squares; 
        //double pushes:
        const unsigned long long rank4 = 0x00000000FF000000;
        doublepushes = (singlepushes << 8) & empty_squares & rank4;
        return singlepushes | doublepushes;
    } 
    //black
    singlepushes = (BitBoard::pieces[0][1] >> 8) & empty_squares; 
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

int BitBoard::getHighestSquare(unsigned long long attackSet) {
    //given 0b10001, returns 0, for the index of the least significant set bit
    //used for iterating over squares of an attack set
    return log2(attackSet);
}

int BitBoard::getNumPieces(unsigned long long pieceSet) {
    //returns number of set bits by Kernighan's algorithm
    int count;
    for (count = 0; pieceSet; count++) {
        pieceSet &= pieceSet - 1;
    }
    return count;
}


std::string BitBoard::fen() {
    /**
     * Generates board fen of the position
     * Does not include info such as turn to move and castling rights
     * 
     */
    std::string fen = "";
    std::string rows[8];
    for (int i = 0; i < 8; i++) {
        rows[i] = "";
    }
    int space = 0;
    char* fenChars[2];
    char wChars[6] = {'P', 'R', 'N', 'B', 'Q', 'K'};
    char bChars[6] = {'p', 'r', 'n', 'b', 'q', 'k'};
    fenChars[0] = wChars;
    fenChars[1] = bChars;
    int row = 0;

    for (int i = 0; i < 64; i++) {
        
        bool isPiece = false;
        for (int j = 0; j < 2; j++) {
            for (int p = 0; p < 6; p++) {
                if (singleMask[i] & pieces[p][j]) {
                    isPiece = true;
                    if (space) {
                        //space is always <= 8 and so is representable as a single char
                        rows[row] += (char)(space + 48);
                        space = 0;
                    }
                    rows[row] += fenChars[j][p];
                }
            }
        }
        if (!isPiece) {
            space++;
        }
        if (singleMask[i] & verticalMask[63]) { //end of row

            if (space) {
                rows[row] += (char)(space + 48); 
                space = 0;
            }
            if (i != 7) {
                rows[row] += '/';
            }
            row++;
        }
        
    }
    for (int i = 7; i >= 0; i--) {
        fen += rows[i];
    }

    return fen;
}