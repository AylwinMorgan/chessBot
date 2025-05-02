#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
//#include "chess.hpp"
#include <random>
#include <iostream>
#include <cmath>
#include <limits>
using namespace ChessSimulator;


// use to look up distance from center of board
const int arrCenterDistance[64] = {
  3, 3, 3, 3, 3, 3, 3, 3,
  3, 2, 2, 2, 2, 2, 2, 3,
  3, 2, 1, 1, 1, 1, 2, 3,
  3, 2, 1, 0, 0, 1, 2, 3,
  3, 2, 1, 0, 0, 1, 2, 3,
  3, 2, 1, 1, 1, 1, 2, 3,
  3, 2, 2, 2, 2, 2, 2, 3,
  3, 3, 3, 3, 3, 3, 3, 3
};

// use following tables to get piece value at board location
// credit: chessprogramming.org for the numerical values
const int whitePawnValue[64] = {
  100,  100,  100,  100,  100,  100,  100,  100,
  50, 50, 50, 50, 50, 50, 50, 50,
  10, 10, 20, 30, 30, 20, 10, 10,
  5,  5, 10, 25, 25, 10,  5,  5,
  0,  0,  0, 20, 20,  0,  0,  0,
  5, -5,-10,  0,  0,-10, -5,  5,
  5, 10, 10,-20,-20, 10, 10,  5,
  0,  0,  0,  0,  0,  0,  0,  0
};

const int blackPawnValue[64] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10,-20,-20, 10, 10,  5,
  5, -5,-10,  0,  0,-10, -5,  5,
  0,  0,  0, 20, 20,  0,  0,  0,
  5,  5, 10, 25, 25, 10,  5,  5,
  10, 10, 20, 30, 30, 20, 10, 10,
  50, 50, 50, 50, 50, 50, 50, 50,
  100,  100,  100,  100,  100,  100,  100,  100
};

const int whiteKnightValue[64] = {
  -50,-40,-30,-30,-30,-30,-40,-50,
  -40,-20,  0,  0,  0,  0,-20,-40,
  -30,  0, 10, 15, 15, 10,  0,-30,
  -30,  5, 15, 20, 20, 15,  5,-30,
  -30,  0, 15, 20, 20, 15,  0,-30,
  -30,  5, 10, 15, 15, 10,  5,-30,
  -40,-20,  0,  5,  5,  0,-20,-40,
  -50,-40,-30,-30,-30,-30,-40,-50,
};

const int blackKnightValue[64] = {
  -50,-40,-30,-30,-30,-30,-40,-50,
  -40,-20,  0,  5,  5,  0,-20,-40,
  -30,  5, 10, 15, 15, 10,  5,-30,
  -30,  0, 15, 20, 20, 15,  0,-30,
  -30,  5, 15, 20, 20, 15,  5,-30,
  -30,  0, 10, 15, 15, 10,  0,-30,
  -40,-20,  0,  0,  0,  0,-20,-40,
  -50,-40,-30,-30,-30,-30,-40,-50,
};

const int whiteBishopValue[64] = {
  -20,-10,-10,-10,-10,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5, 10, 10,  5,  0,-10,
  -10,  5,  5, 10, 10,  5,  5,-10,
  -10,  0, 10, 10, 10, 10,  0,-10,
  -10, 10, 10, 10, 10, 10, 10,-10,
  -10,  5,  0,  0,  0,  0,  5,-10,
  -20,-10,-10,-10,-10,-10,-10,-20
};

const int blackBishopValue[64] = {
  -20,-10,-10,-10,-10,-10,-10,-20,
  -10,  5,  0,  0,  0,  0,  5,-10,
  -10, 10, 10, 10, 10, 10, 10,-10,
  -10,  0, 10, 10, 10, 10,  0,-10,
  -10,  5,  5, 10, 10,  5,  5,-10,
  -10,  0,  5, 10, 10,  5,  0,-10,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -20,-10,-10,-10,-10,-10,-10,-20
};

const int whiteRookValue[64] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0
};

const int blackRookValue[64] = {
  0,  0,  0,  5,  5,  0,  0,  0,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  5, 10, 10, 10, 10, 10, 10,  5,
  0,  0,  0,  0,  0,  0,  0,  0
};

const int whiteQueenValue[64] = {
  -20,-10,-10, -5, -5,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5,  5,  5,  5,  0,-10,
  -5,  0,  5,  5,  5,  5,  0, -5,
   0,  0,  5,  5,  5,  5,  0, -5,
  -10,  5,  5,  5,  5,  5,  0,-10,
  -10,  0,  5,  0,  0,  0,  0,-10,
  -20,-10,-10, -5, -5,-10,-10,-20
};

const int blackQueenValue[64] = {
  -20,-10,-10, -5, -5,-10,-10,-20,
  -10,  0,  5,  0,  0,  0,  0,-10,
  -10,  5,  5,  5,  5,  5,  0,-10,
   0,  0,  5,  5,  5,  5,  0, -5,
  -5,  0,  5,  5,  5,  5,  0, -5,
  -10,  0,  5,  5,  5,  5,  0,-10,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -20,-10,-10, -5, -5,-10,-10,-20
};

const int whiteKingValue[64] = {
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -20,-30,-30,-40,-40,-30,-30,-20,
  -10,-20,-20,-20,-20,-20,-20,-10,
   20, 20,  0,  0,  0,  0, 20, 20,
   20, 30, 10,  0,  0, 10, 30, 20
};

const int blackKingValue[64] = {
   20, 30, 10,  0,  0, 10, 30, 20,
   20, 20,  0,  0,  0,  0, 20, 20,
  -10,-20,-20,-20,-20,-20,-20,-10,
  -20,-30,-30,-40,-40,-30,-30,-20,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30
};

BoardState ChessSimulator::getNewBoardStateFromMove(BoardState& board, ChessMove move) {
	BoardState newBoard = BoardState();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			newBoard.boardArray.push_back(board.boardArray.at(i * 8 + j));
		}
	}

	int fromColumn = (int)move.from.at(0) - (int)'a';
	int fromRow = (int)move.from.at(1) - (int)'1';

	int toColumn = (int)move.to.at(0) - (int)'a';
	int toRow = (int)move.to.at(1) - (int)'1';

	char piece = newBoard.boardArray.at((7 - fromRow) * 8 + fromColumn);
	char targetSquareContents = newBoard.boardArray.at((7 - toRow) * 8 + toColumn);

	if (!move.promotion.empty()) {
		if ((bool)isupper(piece)){
			piece = toupper(move.promotion.at(0));
		}
		else {
			piece = move.promotion.at(0);
		}
	}
	// set ChessMove.to in the new board to be the piece at ChessMove.from
	newBoard.boardArray.at((7 - toRow) * 8 + toColumn) = piece;
	// set ChessMove.from in the new board to empty
	// if the move was a castle, move the rook as well
	if (move.castle) {
		char rook = 'r';
		if ((bool)isupper(piece)) {
			rook = 'R';
		}
		if (toColumn == 7) {
			newBoard.boardArray.at((7 - toRow) * 8 + toColumn - 1) = rook;
		}
		else {
			newBoard.boardArray.at((7 - toRow) * 8 + toColumn + 1) = rook;
		}
	}
	newBoard.boardArray.at((7 - fromRow) * 8 + fromColumn) = '-';
	return newBoard;
}

// checkwhite = check for threatening white pieces
bool ChessSimulator::squareIsThreatened(BoardState& board, int row, int column, bool checkWhite) {
	std::string pieceColumnString = board.getColumnLetter(column);
	std::string pieceRowString = std::to_string(row + 1);
	
	// check orthogonals for rooks and queens
	
	// vertical (up)
	for (int i = row - 1; i >= 0; i--) {
		char piece = board.getPieceAtSquare(i, column);

		char lowerPiece = std::tolower(piece);

		if (piece != '-'){
			char rookValue = 'r';
			char queenValue = 'q';

			if ((lowerPiece == rookValue || lowerPiece == queenValue) && (bool)isupper(piece) == checkWhite) {
				return true;
			}
			break;
		}
	}
	
	// vertical (down)
	for (int i = row + 1; i < 8; i++) {
		char piece = board.getPieceAtSquare(i, column);

		char lowerPiece = std::tolower(piece);

		if (piece != '-') {
			char rookValue = 'r';
			char queenValue = 'q';

			if ((lowerPiece == rookValue || lowerPiece == queenValue) && (bool)isupper(piece) == checkWhite) {
				return true;
			}
			break;
		}
	}

	// horizontal (left)
	for (int i = column - 1; i >= 0; i--) {
		char piece = board.getPieceAtSquare(row, i);

		char lowerPiece = std::tolower(piece);

		if (piece != '-') {
			char rookValue = 'r';
			char queenValue = 'q';

			if ((lowerPiece == rookValue || lowerPiece == queenValue) && (bool)isupper(piece) == checkWhite) {
				return true;
			}
			break;
		}
	}

	// horizontal (right)
	for (int i = column + 1; i < 8; i++) {
		char piece = board.getPieceAtSquare(row, i);

		char lowerPiece = std::tolower(piece);

		if (piece != '-') {
			char rookValue = 'r';
			char queenValue = 'q';

			if ((lowerPiece == rookValue || lowerPiece == queenValue) && (bool)isupper(piece) == checkWhite) {
				return true;
			}
			break;
		}
	}

	// check diagonals for bishops and queens
	int dRow;
	int dCol;

	// piece -> top left
	dRow = row - 1;
	dCol = column - 1;
	while (dRow >= 0 && dRow < 8 && dCol >= 0 && dCol < 8) {
		char piece = board.getPieceAtSquare(dRow, dCol);

		char lowerPiece = std::tolower(piece);

		if (piece != '-') {
			char bishopValue = 'b';
			char queenValue = 'q';

			if ((lowerPiece == bishopValue || lowerPiece == queenValue) && (bool)isupper(piece) == checkWhite) {
				return true;
			}
			break;
		}

		dRow--;
		dCol--;
	}

	// piece -> top right
	dRow = row - 1;
	dCol = column + 1;
	while (dRow >= 0 && dRow < 8 && dCol >= 0 && dCol < 8) {
		char piece = board.getPieceAtSquare(dRow, dCol);

		char lowerPiece = std::tolower(piece);

		if (piece != '-') {
			char bishopValue = 'b';
			char queenValue = 'q';

			if ((lowerPiece == bishopValue || lowerPiece == queenValue) && (bool)isupper(piece) == checkWhite) {
				return true;
			}
			break;
		}

		dRow--;
		dCol++;
	}

	// piece -> bottom right
	dRow = row + 1;
	dCol = column + 1;
	while (dRow >= 0 && dRow < 8 && dCol >= 0 && dCol < 8) {
		char piece = board.getPieceAtSquare(dRow, dCol);

		char lowerPiece = std::tolower(piece);

		if (piece != '-') {
			char bishopValue = 'b';
			char queenValue = 'q';

			if ((lowerPiece == bishopValue || lowerPiece == queenValue) && (bool)isupper(piece) == checkWhite) {
				return true;
			}
			break;
		}

		dRow++;
		dCol++;
	}

	// piece -> bottom left
	dRow = row + 1;
	dCol = column - 1;
	while (dRow >= 0 && dRow < 8 && dCol >= 0 && dCol < 8) {
		char piece = board.getPieceAtSquare(dRow, dCol);

		char lowerPiece = std::tolower(piece);

		if (piece != '-') {
			char bishopValue = 'b';
			char queenValue = 'q';

			if ((lowerPiece == bishopValue || lowerPiece == queenValue) && (bool)isupper(piece) == checkWhite) {
				return true;
			}
			break;
		}

		dRow++;
		dCol--;
	}

	// check knight positions
	char knightChar = 'n';
	if (checkWhite) {
		knightChar = 'N';
	}
	
	if (board.getPieceAtSquare(row+2, column-1) == knightChar) {
		return true;
	}
	if (board.getPieceAtSquare(row+2, column+1) == knightChar) {
		return true;
	}
	if (board.getPieceAtSquare(row+1, column+2) == knightChar) {
		return true;
	}
	if (board.getPieceAtSquare(row-1, column+2) == knightChar) {
		return true;
	}
	if (board.getPieceAtSquare(row-2, column+1) == knightChar) {
		return true;
	}
	if (board.getPieceAtSquare(row-2, column-1) == knightChar) {
		return true;
	}
	if (board.getPieceAtSquare(row-1, column-2) == knightChar) {
		return true;
	}
	if (board.getPieceAtSquare(row+1, column-2) == knightChar) {
		return true;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			char c = board.getPieceAtSquare(i,j);
			if (std::tolower(c) == 'p') {
				std::unordered_set<ChessMove> moves = getLegalMoves(board,i,j,checkWhite,false,true);
				for (ChessMove m : moves) {
					if (m.to.at(0) == board.getColumnLetter(j).at(0) && m.to.at(1) == i + 1) {
						return true;
					}
				}
			}
		}
	}

	// check pawn positions
	if (checkWhite) {
		if (board.getPieceAtSquare(row+1,column-1) == 'P' || board.getPieceAtSquare(row+1,column+1) == 'P') {
			return true;
		}
	}
	else {
		if (board.getPieceAtSquare(row - 1, column - 1) == 'p' || board.getPieceAtSquare(row - 1, column + 1) == 'p') {
			return true;
		}
	}

	// check surrounding squares for king
	for (int r = -1; r <= 1; r++) {
		for (int c = -1; c <= 1; c++) {
			if (r == 0 && c == 0){
				continue;
			}
			int newRow = row + r;
			int newCol = column + c;
			char piece = board.getPieceAtSquare(newRow, newCol);
			if ((checkWhite && piece == 'K') || (!checkWhite && piece == 'k')) {
				return true;
			}
		}
	}

	/*
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// get all moves that the enemy piece can do on the new board
			std::unordered_set<ChessMove> enemyMoves = getLegalMoves(board, i, j, !checkWhite, false, true);
			// if any of the moves end on the piece's square, return true
			for (ChessMove move : enemyMoves) {
				if (move.to == pieceColumnString + pieceRowString) {
					return true;
				}
			}
		}
	}
	*/
	return false;
}

// uses board state and move to calculate new board state
bool ChessSimulator::kingIsInCheck(BoardState& board, ChessMove move, Color color) {
	// get a potential board state based on the move and see if the king of 'color' is in check
	// do this by looking at all enemy pieces and checking if any of their legal moves end at the 'color' king's position
	bool kingIsReached = false;

	BoardState newBoard = getNewBoardStateFromMove(board,move);

	// get all 
	bool wtm = color == Color::black;

	int kingRow;
	int kingColumn;
	std::string kingColumnString;
	std::string kingRowString;

	// find the king
	for (kingRow = 0; kingRow < 8; kingRow++) {
		for (kingColumn = 0; kingColumn < 8; kingColumn++) {
			char square = newBoard.boardArray.at((7 - kingRow) * 8 + kingColumn);
			if (((color == Color::white && (bool)isupper(square)) || (color == Color::black && (bool)islower(square))) && tolower(square) == 'k') {
				kingColumnString = board.getColumnLetter(kingColumn);
				kingRowString = std::to_string(kingRow + 1);
				//std::cout << "king found at: " << kingColumnString << kingRowString << std::endl;
				goto end_loop;
			}
		}
	}
	end_loop:

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// get all moves that the enemy piece can do on the new board
			std::unordered_set<ChessMove> enemyMoves = getLegalMoves(newBoard, i, j, wtm, false);
			// if any of the moves end on the king's square, return true
			for (ChessMove move : enemyMoves) {
				if (move.to == kingColumnString + kingRowString) {
					return true;
				}
			}
		}
	}
	return false;
}

// takes in already calculated new board state
bool ChessSimulator::kingIsInCheck(BoardState& board, bool checkWhite) {
	int kingRow;
	int kingColumn;
	std::string kingColumnString;
	std::string kingRowString;

	// find the king
	for (kingRow = 0; kingRow < 8; kingRow++) {
		for (kingColumn = 0; kingColumn < 8; kingColumn++) {
			char square = board.boardArray.at((7 - kingRow) * 8 + kingColumn);
			if ((((checkWhite && (bool)isupper(square)) || !checkWhite && (bool)islower(square))) && tolower(square) == 'k') {
				kingColumnString = board.getColumnLetter(kingColumn);
				kingRowString = std::to_string(kingRow + 1);
				//std::cout << "king found at: " << kingColumnString << kingRowString << std::endl;
				goto end_loop;
			}
		}
	}
end_loop:

/*
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// get all moves that the enemy piece can do on the new board
			std::unordered_set<ChessMove> enemyMoves = getLegalMoves(board, i, j, !checkWhite, false);
			// if any of the moves end on the king's square, return true
			for (ChessMove move : enemyMoves) {
				if (move.to == kingColumnString + kingRowString) {
					return true;
				}
			}
		}
	}
	return false;
	*/
	return squareIsThreatened(board, kingRow, kingColumn, !checkWhite);
}

std::unordered_set<ChessMove> ChessSimulator::getLegalMoves(BoardState& board, int row, int column, bool whiteToMove, bool considerCheck, bool threatenOwnPieces) {

	char piece = board.getPieceAtSquare(row, column);
	if (!isalpha(piece)) {
		return std::unordered_set<ChessMove>();
	}
	Pawn* pawn = new Pawn();
	Rook* rook = new Rook();
	Knight* knight = new Knight();
	Bishop* bishop = new Bishop();
	Queen* queen = new Queen();
	King* king = new King();

	bool pieceIsCorrectColor = whiteToMove;
	Color color = Color::white;
	char p;
	std::unordered_set<ChessMove> moves = std::unordered_set<ChessMove>();

	Piece* pieceStruct;
	if ((bool)islower(piece)){
		color = Color::black;
		if (whiteToMove) {
			pieceIsCorrectColor = false;
		}
		else {
			pieceIsCorrectColor = true;
		}
	}
	p = toupper(piece);
	if (pieceIsCorrectColor){
		switch (p)
		{
			case 'P':
			{
			    pieceStruct = pawn;
				break;
			}
			case 'R':
			{
				pieceStruct = rook;
				break;
			}
			case 'N':
			{
				pieceStruct = knight;
				break;
			}
			case 'B':
			{
				pieceStruct = bishop;
				break;
			}
			case 'Q':
			{
				pieceStruct = queen;
				break;
			}
			case 'K':
			{
				pieceStruct = king;
				break;
			}
			default:
			{
				pieceStruct = pawn;
			}
		}
		pieceStruct->color = color;
		moves = pieceStruct->getLegalMoves(row,column,board, threatenOwnPieces);
		std::unordered_set<ChessMove> toDelete;
		if (considerCheck) {
			for (ChessMove move : moves) {
				if (kingIsInCheck(board, move, color)) {
					toDelete.insert(move);
				}
			}
			for (ChessMove move : toDelete) {
				moves.erase(move);
			}
		}
	}
	delete pawn;
	delete rook;
	delete bishop;
	delete knight;
	delete queen;
	delete king;
	return moves;
}

std::unordered_set<ChessMove> ChessSimulator::getAllLegalMoves(BoardState& board, bool whiteToMove, bool considerCheck) {
	std::unordered_set<ChessMove> allMoves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			std::unordered_set<ChessMove> newMoves = getLegalMoves(board, i, j, whiteToMove, true);
			allMoves.insert(newMoves.begin(), newMoves.end());
		}
	}
	return allMoves;
}

std::string ChessSimulator::Move(std::string fen) {
	std::srand(std::time(nullptr));
	// get all pieces,
    // for each piece:
    // get all legal moves
    // pick a random move from this set
	BoardState board(fen);
	//std::string tempFen = "r1b1nb1r / 2p4q / 3n2p1 / pp1pP2p / PPPP1PkP / 6pR / 3K4 / R1BQNB2 b - e3 0 15";
	//BoardState board(tempFen);
	bool whiteToMove = fen.find('w') != std::string::npos;
	std::unordered_set<ChessMove> allMoves = getAllLegalMoves(board,whiteToMove,true);

	ChessMove move = getBestMove(allMoves, board, whiteToMove);

	std::string finalMove = move.from + move.to + move.promotion;
	return finalMove;
	/*
	auto iterator = allMoves.begin();
	if (allMoves.size() > 0){
    	std::advance(iterator, rand() % allMoves.size());

    	ChessMove move = *iterator;

    	std::string finalMove = move.from + move.to + move.promotion;
	    return finalMove;
	}
	*/
	//return "";
}

// uses minimax algorithm to get most favorable move based on heuristic
ChessMove ChessSimulator::getBestMove(std::unordered_set<ChessMove> moves, BoardState& board, bool whiteToMove) {
	// look at all moves
	// for each move:
	// get all subsequent moves 
	
	// fill node children set with a node for all moves in moves
	// for each child, fill its child set with a node for each new possible set of moves
	ChessSimulator::minmaxNode* root = new minmaxNode;
	root->board = board;
	int depth = 3;
	int bestValue = ChessSimulator::minmax(root, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), true, whiteToMove, whiteToMove);
	ChessMove bestMove;

	for (minmaxNode* child : root->children) {
		if (child->heuristic == root->heuristic) {
			bestMove = child->move;
			break;
		}
	}

	delete root;
	return bestMove;
}

/*   minimax pseudocode for reference (from wikipedia)
function minimax(node, depth, maximizingPlayer) is
   if depth = 0 or node is a terminal node then
	   return the heuristic value of node
   if maximizingPlayer then
	   value := −∞
	   for each child of node do
		   value := max(value, minimax(child, depth − 1, FALSE))
	   return value
   else (* minimizing player *)
	   value := +∞
	   for each child of node do
		   value := min(value, minimax(child, depth − 1, TRUE))
	   return value
*/

// get value of chess move and apply it to node
// to do: add alpha-beta pruning to make this more efficient
int ChessSimulator::minmax(minmaxNode* node, int depth, int alpha, int beta, bool maximize, bool whiteToMove, bool originallyWhite) {
	if (depth == 0) {
		node->heuristic = ChessSimulator::getValueOfBoard(node->board, originallyWhite, originallyWhite);
		return node->heuristic;
	}

	
	// if deepest depth reached, return h-value of node based on board state
	std::unordered_set<ChessMove> moves = getAllLegalMoves(node->board, whiteToMove, true);


	// if there are no moves and the moving 

	// if there are no moves, get current board value 
	// (this means checkmate or draw, examine this state carefully)
	if (moves.empty()) {
		node->heuristic = ChessSimulator::getValueOfBoard(node->board, originallyWhite, originallyWhite);
		// if there are no valid moves and moving color piece is in check
		// return very low value
		// if non moving color 
		// if depth is even, then minmax is currently looking at the non starting color
		bool originalColor = whiteToMove;
		if (depth % 2 == 0) {
			originalColor = !originalColor;
		}
		// if original color is in checkmate, avoid this outcome 
		if (kingIsInCheck(node->board, originalColor)) {
			node->heuristic = -10000000 - depth;
			return node->heuristic;
		}
		// if the other color is in checkmate, prioritize this state above all else
		else if (kingIsInCheck(node->board, !originalColor)) {
			node->heuristic = 10000000 + depth;
			return node->heuristic;
		}
		return node->heuristic;
	}

	// otherwise, look at all possible moves and pick the one with the highest or lowest minmax value
	// for each possible move, create a child node and run minmax again from there (with 1 less depth and maximize switched)
	int value = maximize ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

	for (ChessMove move : moves) {
		BoardState nextState = ChessSimulator::getNewBoardStateFromMove(node->board, move);
		
		ChessSimulator::minmaxNode* child = new ChessSimulator::minmaxNode;
		child->board = nextState;
		child->move = move;

		int childScore = ChessSimulator::minmax(child, depth - 1, alpha, beta, !maximize, !whiteToMove, originallyWhite);
		node->children.insert(child);

		if (maximize) {
			value = std::max(value, childScore);
			alpha = std::max(alpha, value);
		}
		else {
			value = std::min(value, childScore);
			beta = std::min(beta, value);
		}

		// alpha beta pruning
		if (beta <= alpha) {
			break;
		}
	}

	node->heuristic = value;
	return value;
}

// calculate value of board state based on which pieces both sides have
// also calculate value of piece value based on its location
int ChessSimulator::getColorScore(BoardState& board, bool checkWhite, bool originallyWhite) {
	int total = 0;
	int boardPosition = 0;

	int compositionValueFactor = 15;
	float positionValueFactor = 1;


	for (char c : board.boardArray) {
		int row = (63 - boardPosition) / 8;
		int column = boardPosition % 8;
		int scoreFactor = 1;
		bool pieceIsWhite = (bool)isupper(c);
		if ((bool)isupper(c) != checkWhite){
			scoreFactor = -1;
		}
		char lower = tolower(c);
		int compositionValue = 0;
		int positionValue = 0;
		switch (lower)
		{
			case 'p':
				// pawns are worth 1 point overall and give value based on how far they have advanced
				compositionValue = 1;
				if (pieceIsWhite) {
					positionValue = whitePawnValue[boardPosition];
				}
				else {
					positionValue = blackPawnValue[boardPosition];
				}
				break;
			case 'n':
				compositionValue = 3;
				if (pieceIsWhite) {
					positionValue = whiteKnightValue[boardPosition];
				}
				else {
					positionValue = blackKnightValue[boardPosition];
				}
				break;
			case 'b':
				// knight and bishop are worth more when closer to the center 4 squares of the board
				compositionValue = 3;
				if (pieceIsWhite) {
					positionValue = whiteBishopValue[boardPosition];
				}
				else {
					positionValue = blackBishopValue[boardPosition];
				}
				break;
			case 'r':
				compositionValue = 5;
				if (pieceIsWhite) {
					positionValue = whiteRookValue[boardPosition];
				}
				else {
					positionValue = blackRookValue[boardPosition];
				}
				break;
			case 'q':
				compositionValue = 9;
				if (pieceIsWhite) {
					positionValue = whiteQueenValue[boardPosition];
				}
				else {
					positionValue = blackQueenValue[boardPosition];
				}
				break;
			case 'k':
				compositionValue = 0;
				if (pieceIsWhite) {
					positionValue = whiteKingValue[boardPosition];
				}
				else {
					positionValue = blackKingValue[boardPosition];
				}
				break;
		}

		if (checkWhite && originallyWhite) {
			if (squareIsThreatened(board, row, column, false)) {
				compositionValue = 0;
				positionValueFactor = 0.25;
				if (squareIsThreatened(board, row, column, true)) {
					compositionValue = 1;
					positionValueFactor = 1;
				}
			}
		}
		else if (!checkWhite && !originallyWhite){
			if (squareIsThreatened(board, row, column, true)) {
				compositionValue = 0;
				positionValueFactor = 0.25;
				if (squareIsThreatened(board, row, column, false)) {
					compositionValue = 1;
					positionValueFactor = 1;
				}
			}
		}

		compositionValue *= compositionValueFactor;
		positionValue *= positionValueFactor;
		positionValueFactor = 1;
		//bool whiteThreatens = board.boardThreats.at(boardPosition).white;
		//bool blackThreatens = board.boardThreats.at(boardPosition).black;

		// pieces being threatened are bad unless the piece is protected
		//if ((checkWhite && blackThreatens && !whiteThreatens) || (!checkWhite && !blackThreatens && whiteThreatens)) {
		//	compositionValue /= 2;
		//}


		total += (compositionValue + positionValue) * scoreFactor;
		boardPosition++;
	}
	return total;
}

// to do: make this analyze the resulting board state
int ChessSimulator::getValueOfBoard(BoardState& board, bool whiteToMove, bool originallyWhite) {
	int value = 0;

	/*
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			Threats squareThreats;
			if (board.boardArray.at(row * 8 + col) != '-'){
				if (squareIsThreatened(board, row, col, false)) {
					squareThreats.black = true;
			    }
			    if (squareIsThreatened(board,row,col,true)) {
					squareThreats.white = true;
			    }
			}
			board.boardThreats.push_back(squareThreats);
		}
	}
	*/

	if (kingIsInCheck(board, !whiteToMove)) {
		value += 50;
	}
	return ChessSimulator::getColorScore(board, whiteToMove, originallyWhite) + value;
}

// delete all child nodes when node is deleted
ChessSimulator::minmaxNode::~minmaxNode() {
	for (minmaxNode* child : children) {
		delete child;
	}
}

  // create your board based on the board string following the FEN notation
  // search for the best move using minimax / monte carlo tree search /
  // alpha-beta pruning / ... try to use nice heuristics to speed up the search
  // and have better results return the best move in UCI notation you will gain
  // extra points if you create your own board/move representation instead of
  // using the one provided by the library

  // 
  
  // here goes a random movement
  //chess::Board board(fen);
  //chess::Movelist moves;
  //chess::movegen::legalmoves(moves, board);
  //if(moves.size() == 0)
  //  return "";



  //// get random move
  //std::random_device rd;
  //std::mt19937 gen(rd());
  //std::uniform_int_distribution<> dist(0, moves.size() - 1);
  //auto move = moves[dist(gen)];
  //return chess::uci::moveToUci(move);