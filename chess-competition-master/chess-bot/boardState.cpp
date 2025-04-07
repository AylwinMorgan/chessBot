#include "boardState.h"

BoardState::~BoardState() {
	//delete boardArray;
}

BoardState::BoardState() {
	board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
}

// setup boardArray by filling it with chars
BoardState::BoardState(std::string fen) {
	board = fen;
	// fills an array with characters to represent the board
	// IMPORTANT: the top row is row 8 in chess so start from the bottom of the array and work upwards
	int currentRow = 7;
	int currentColumn = 0;
	for (int i = 0; i < fen.length(); i++) {
		char c = fen[i];
		if (currentColumn > 7) {
			currentColumn = 0;
			currentRow--;
		}
		if (currentRow < 0) {
			break;
		}	
		if (isalpha(c)){
			boardArray[currentRow][currentColumn] = c;
			currentColumn++;
		}
		else if (isdigit(c)) {
			for (int j = 0; j < (int)(c - '0'); j++) {
				boardArray[currentRow][currentColumn] = '-';
				currentColumn++;
			}
		}
	}
}

std::string BoardState::getColumnLetter(int column) {
	switch (column) {
		case 0:
			return "a";
		case 1:
			return "b";
		case 2:
			return "c";
		case 3:
			return "d";
		case 4:
			return "e";
		case 5:
			return "f";
		case 6:
			return "g";
		case 7:
			return "h";
		default:
			return "-";
	}
}

ChessMove BoardState::getUCINotation(int startRow, int startColumn, int endRow, int endColumn, char promotion) {
	ChessMove notation;
	notation.from = getColumnLetter(startColumn) + std::to_string(startRow+1);
	notation.to = getColumnLetter(endColumn) + std::to_string(endRow+1);
	notation.promotion = promotion;
	return notation;
}


char BoardState::getPieceAtSquare(int row, int column) {
	return boardArray[row][column];
}