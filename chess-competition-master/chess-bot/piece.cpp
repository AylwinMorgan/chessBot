#include "piece.h"

bool Piece::squareIsValid(int row, int column, BoardState board) {
	if (row < 0 || row > 7 || column < 0 || column > 7) {
		return false;
	}
	char content = board.getPieceAtSquare(row,column);
	// empty spaces are always valid
	if (content == '-') {
		return true;
	}
	// opposite color spaces are valid
	Color squareColor;
	if (isupper(content)) {
		squareColor = Color::white;
	}
	else {
		squareColor = Color::black;
	}

	if (color != squareColor) {
		return true;
	}

	return false;
}

std::unordered_set<ChessMove> Pawn::getLegalMoves(int row, int column, BoardState board) {
	std::unordered_set<ChessMove> legalMoves;
	if (color == Color::white) {
		// check next row
		char nextRow = board.getPieceAtSquare(row + 1, column);
		if (nextRow == '-' && row < 7) {
			char promotion = 0;
			if (row == 6) {
				promotion = 'q';
			}
			legalMoves.insert(board.getUCINotation(row,column,row+1,column,promotion));
			if (row == 1) {
				char doubleMove = board.getPieceAtSquare(row + 2, column);
				if (doubleMove == '-') {
					legalMoves.insert(board.getUCINotation(row,column,row+2,column));
				}
			}
		}
		// check diagonals in next row for black pieces
		char diagonalLeft = board.getPieceAtSquare(row+1, column - 1);
		if ( squareIsValid(row+1, column-1, board) && diagonalLeft != '-') {
			legalMoves.insert(board.getUCINotation(row,column,row+1,column-1));
		}
		char diagonalRight = board.getPieceAtSquare(row+1, column + 1);
		if (squareIsValid(row+1,column+1, board) && diagonalRight != '-') {
			legalMoves.insert(board.getUCINotation(row, column, row + 1, column + 1));
		}
	}
	else {
		// check next row
		char nextRow = board.getPieceAtSquare(row - 1, column);
		if (nextRow == '-' && row > 0) {
			char promotion = '0';
			if (row == 1) {
				promotion = 'q';
			}
			legalMoves.insert(board.getUCINotation(row, column, row - 1, column));
			if (row == 6) {
				char doubleMove = board.getPieceAtSquare(row - 2, column);
				if (doubleMove == '-') {
					legalMoves.insert(board.getUCINotation(row, column, row - 2, column));
				}
			}
		}
		// check diagonals in next row for white pieces
		char diagonalLeft = board.getPieceAtSquare(row - 1, column - 1);
		char diagonalRight = board.getPieceAtSquare(row - 1, column + 1);
		if (squareIsValid(row-1,column-1, board) && diagonalLeft != '-') {
			legalMoves.insert(board.getUCINotation(row, column, row - 1, column - 1));
		}
		if (squareIsValid(row-1,column+1, board) && diagonalRight != '-') {
			legalMoves.insert(board.getUCINotation(row, column, row - 1, column + 1));
		}
	}
	return legalMoves;
}

std::unordered_set<ChessMove> Rook::getLegalMoves(int row, int column, BoardState board) {
	// look at all squares to the left, stopping when a square with a piece is found or the edge of the board is reached
	// include the square if the piece on it is a different color
	// repeat for forward, right, and backward directions
	std::unordered_set<ChessMove> legalMoves;
	// get squares to left, breaking the loop when an invalid square is reached
	for (int i = column - 1; i >= 0; i--) {
		if (squareIsValid(row, i, board)) {
			legalMoves.insert(board.getUCINotation(row,column,row,i));
			if (board.getPieceAtSquare(row, i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	// get valid squares to right
	for (int i = column + 1; i <= 7; i++) {
		if (squareIsValid(row, i, board)) {
			legalMoves.insert(board.getUCINotation(row,column,row,i));
			if (board.getPieceAtSquare(row, i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	// get valid squares towards bottom
	for (int i = row - 1; i >= 0; i--) {
		if (squareIsValid(i, column, board)) {
			legalMoves.insert(board.getUCINotation(row, column, i, column));
			if (board.getPieceAtSquare(i, column) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	// get valid squares towards top
	for (int i = row + 1; i <= 7; i++) {
		if (squareIsValid(i, column, board)) {
			legalMoves.insert(board.getUCINotation(row, column, i, column));
			if (board.getPieceAtSquare(i, column) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	return legalMoves;
}

std::unordered_set<ChessMove> Bishop::getLegalMoves(int row, int column, BoardState board) {
	// look at all squares in all 4 diagonals, stopping when an invalid square is reached
	// get the number of squares by finding the minimum value between rows to edge and columns to edge
	int squaresInTopLeftDirection = std::min(7-row,column);
	int squaresInTopRightDirection = std::min(7-row,7-column);
	int squaresInBottomLeftDirection = std::min(row,column);
	int squaresInBottomRightDirection = std::min(row,7-column);
	std::unordered_set<ChessMove> legalMoves;

	for (int i = 1; i <= squaresInTopLeftDirection; i++) {
		if (squareIsValid(row+i, column-i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row+i, column-i));
			if (board.getPieceAtSquare(row+i, column-i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	for (int i = 1; i <= squaresInTopRightDirection; i++) {
		if (squareIsValid(row + i, column + i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row + i, column + i));
			if (board.getPieceAtSquare(row + i, column + i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	for (int i = 1; i <= squaresInBottomLeftDirection; i++) {
		if (squareIsValid(row - i, column - i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row - i, column - i));
			if (board.getPieceAtSquare(row - i, column - i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	for (int i = 1; i <= squaresInBottomRightDirection; i++) {
		if (squareIsValid(row - i, column + i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row - i, column + i));
			if (board.getPieceAtSquare(row - i, column + i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	return legalMoves;
}

std::unordered_set<ChessMove> Knight::getLegalMoves(int row, int column, BoardState board) {
	std::unordered_set<ChessMove> legalMoves;
	// check all 8 possible knight moves to see if the square is valid
	if (squareIsValid(row+2, column-1, board)) {
		legalMoves.insert(board.getUCINotation(row, column, row+2, column-1));
	}
	if (squareIsValid(row+2, column+1, board)) {
		legalMoves.insert(board.getUCINotation(row, column, row+2, column+1));
	}
	if (squareIsValid(row + 1, column + 2, board)) {
		legalMoves.insert(board.getUCINotation(row, column, row + 1, column + 2));
	}
	if (squareIsValid(row - 1, column + 2, board)) {
		legalMoves.insert(board.getUCINotation(row, column, row - 1, column + 2));
	}
	if (squareIsValid(row - 2, column + 1, board)) {
		legalMoves.insert(board.getUCINotation(row, column, row - 2, column + 1));
	}
	if (squareIsValid(row - 2, column - 1, board)) {
		legalMoves.insert(board.getUCINotation(row, column, row - 2, column - 1));
	}
	if (squareIsValid(row - 1, column - 2, board)) {
		legalMoves.insert(board.getUCINotation(row, column, row - 1, column - 2));
	}
	if (squareIsValid(row + 1, column - 2, board)) {
		legalMoves.insert(board.getUCINotation(row, column, row + 1, column - 2));
	}
	return legalMoves;
}

std::unordered_set<ChessMove> King::getLegalMoves(int row, int column, BoardState board) {
	std::unordered_set<ChessMove> legalMoves;
	// check all 8 possible king moves to see if they are valid
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (squareIsValid(row + i, column + j, board)) {
				legalMoves.insert(board.getUCINotation(row,column,row+i,column+j));
			}
		}
	}
	return legalMoves;
}

std::unordered_set<ChessMove> Queen::getLegalMoves(int row, int column, BoardState board) {
	std::unordered_set<ChessMove> legalMoves;
	int squaresInTopLeftDirection = std::min(7 - row, column);
	int squaresInTopRightDirection = std::min(7 - row, 7 - column);
	int squaresInBottomLeftDirection = std::min(row, column);
	int squaresInBottomRightDirection = std::min(row, 7 - column);
	// get squares to left, breaking the loop when an invalid square is reached
	for (int i = column - 1; i >= 0; i--) {
		if (squareIsValid(row, i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row, i));
			if (board.getPieceAtSquare(row, i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	// get valid squares to right
	for (int i = column + 1; i <= 7; i++) {
		if (squareIsValid(row, i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row, i));
			if (board.getPieceAtSquare(row, i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	// get valid squares towards bottom
	for (int i = row - 1; i >= 0; i--) {
		if (squareIsValid(i, column, board)) {
			legalMoves.insert(board.getUCINotation(row, column, i, column));
			if (board.getPieceAtSquare(i, column) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	// get valid squares towards top
	for (int i = row + 1; i <= 7; i++) {
		if (squareIsValid(i, column, board)) {
			legalMoves.insert(board.getUCINotation(row, column, i, column));
			if (board.getPieceAtSquare(i, column) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}

	// diagonals
	for (int i = 1; i <= squaresInTopLeftDirection; i++) {
		if (squareIsValid(row + i, column - i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row + i, column - i));
			if (board.getPieceAtSquare(row + i, column - i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	for (int i = 1; i <= squaresInTopRightDirection; i++) {
		if (squareIsValid(row + i, column + i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row + i, column + i));
			if (board.getPieceAtSquare(row + i, column + i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	for (int i = 1; i <= squaresInBottomLeftDirection; i++) {
		if (squareIsValid(row - i, column - i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row - i, column - i));
			if (board.getPieceAtSquare(row - i, column - i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}
	for (int i = 1; i <= squaresInBottomRightDirection; i++) {
		if (squareIsValid(row - i, column + i, board)) {
			legalMoves.insert(board.getUCINotation(row, column, row - i, column + i));
			if (board.getPieceAtSquare(row - i, column + i) != '-') {
				break;
			}
		}
		else {
			break;
		}
	}

	return legalMoves;
}