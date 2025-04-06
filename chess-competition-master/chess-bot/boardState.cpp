#include "boardState.h"

BoardState::BoardState() {
	board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
}

BoardState::BoardState(std::string fen) {
	board = fen;
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

std::string BoardState::getUCINotation(int startRow, int startColumn, int endRow, int endColumn, char promotion) {
	std::string notation = getColumnLetter(startColumn) + std::to_string(startRow+1) + getColumnLetter(endColumn) + std::to_string(endRow+1) + promotion;
	return notation;
}

char BoardState::getPieceAtSquare(int row, int column) {
	// go from top left to bottom right until desired square is reached
	int currentColumn = 0;
	int currentRow = 7;
	for (int i = 0; i < board.length(); i++) {
		char c = board[i];
		if (c == ' ') {
			continue;
		}
		if (currentColumn == column && currentRow == row) {
			return c;
		}
		if (c == '/') {
			// go to next row, reset column
			currentRow--;
			currentColumn = 0;
		}
		else if (isdigit(c)) {
			currentColumn += c - '0';
			if (currentRow == row && currentColumn >= column) {
				return '-';
			}
		}
		else {
			currentColumn++;
		}
	}
}