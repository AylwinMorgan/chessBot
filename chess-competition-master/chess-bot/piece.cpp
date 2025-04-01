#include "piece.h"

std::unordered_set<std::string> Pawn::getLegalMoves(BoardState board, int row, int column) {
	std::unordered_set<std::string> legalMoves;
	if (color == Color.white) {
		// check next row
		char nextRow = board.getPieceAtSquare(row + 1, column);
		if (nextRow == '-') {
			legalMoves.insert(getUCINotation(row,column,row+1,column));
			if (row == 1) {
				char doubleMove = board.getPieceAtSquare(row + 2, column);
				if (doubleMove == '-') {
					legalMoves.insert(getUCINotation(row,column,row+2,column));
				}
			}
		}
		// check diagonals in next row for black pieces
		char diagonalLeft = board.getPieceAtSquare(row + 1, column - 1);
		char diagonalRight = board.getPieceAtSquare(row + 1, column + 1);
		if (diagonalLeft != '-' && islower(diagonalLeft)) {
			legalMoves.insert(gettUCINotation(row,column,row+1,column-1));
		}
		if (diagonalRight != '-' && islower(diagonalRight)) {
			legalMoves.insert(gettUCINotation(row, column, row + 1, column + 1));
		}
	}
	else {
		// check next row
		char nextRow = board.getPieceAtSquare(row - 1, column);
		if (nextRow == '-') {
			legalMoves.insert(getUCINotation(row, column, row - 1, column));
			if (row == 6) {
				char doubleMove = board.getPieceAtSquare(row - 2, column);
				if (doubleMove == '-') {
					legalMoves.insert(getUCINotation(row, column, row - 2, column));
				}
			}
		}
		// check diagonals in next row for white pieces
		char diagonalLeft = board.getPieceAtSquare(row - 1, column - 1);
		char diagonalRight = board.getPieceAtSquare(row - 1, column + 1);
		if (diagonalLeft != '-' && isupper(diagonalLeft)) {
			legalMoves.insert(gettUCINotation(row, column, row - 1, column - 1));
		}
		if (diagonalRight != '-' && isupper(diagonalRight)) {
			legalMoves.insert(gettUCINotation(row, column, row - 1, column + 1));
		}
	}
}

std::unordered_set<Move> Rook::getLegalMoves(std::string* board, int row, int column) {

}

std::unordered_set<Move> Bishop::getLegalMoves(std::string* board, int row, int column) {

}

std::unordered_set<Move> Knight::getLegalMoves(std::string* board, int row, int column) {

}

std::unordered_set<Move> King::getLegalMoves(std::string* board, int row, int column) {

}

std::unordered_set<Move> Queen::getLegalMoves(std::string* board, int row, int column) {

}