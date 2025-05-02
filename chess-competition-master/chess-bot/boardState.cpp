#include "boardState.h"
#include "iostream"

BoardState::~BoardState() {
	//delete boardArray;


//	int i = 0;
//	for (Threats* t : boardThreats) {
//		delete t;
//		boardThreats.at(i) = nullptr;
//		i++;
//	}
//	boardThreats.clear();
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
	boardArray.reserve(64);
	
//	for (int i = 0; i < 64; i++) {
//		Threats* t = new Threats;
//		boardThreats.push_back(t);
//	}


	for (int i = 0; i < fen.length(); i++) {
		char c = fen.at(i);
		if (currentColumn > 7) {
			currentColumn = 0;
			currentRow--;
		}
		if (currentRow < 0) {
			break;
		}	
		if (isalpha(c)){
			boardArray.push_back(c);
			currentColumn++;
		}
		else if (isdigit(c)) {
			for (int j = 0; j < (int)(c - '0'); j++) {
				boardArray.push_back('-');
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

ChessMove BoardState::getUCINotation(int startRow, int startColumn, int endRow, int endColumn, char promotion, bool castle) {
	ChessMove notation;
	notation.from = getColumnLetter(startColumn) + std::to_string(startRow+1);
	notation.to = getColumnLetter(endColumn) + std::to_string(endRow+1);
	if (promotion != 0){
		notation.promotion = promotion;
	}
	notation.castle = castle;
	return notation;
}


char BoardState::getPieceAtSquare(int row, int column) {
	if (row < 0 || row > 7 || column < 0 || column > 7) {
		//std::cout << "out of bounds";
		return '-';
	}
	return boardArray.at((7 - row) * 8 + column);
	}