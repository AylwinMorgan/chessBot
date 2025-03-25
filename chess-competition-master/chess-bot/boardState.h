#pragma once


enum class Pieces: std::char {
	BLACK_PAWN = "p",
	BLACK_ROOK = "r",
	BLACK_KNIGHT = "n",
	BLACK_BISHOP = "b",
	BLACK_QUEEN = "q",
	BLACK_KING = "k",
	WHITE_PAWN = "P",
	WHITE_ROOK = "R",
	WHITE_KNIGHT = "N",
	WHITE_BISHOP = "B",
	WHITE_QUEEN = "Q",
	WHITE_KING = "K"
};

class BoardState {
	std::string board[8];
	static const inline std::string startingBoard[8] = {
		"rnbqkbnr",
		"pppppppp",
		"8",
		"8",
		"8",
		"8",
		"PPPPPPPP",
		"RNBQKBNR"
	}

	void setBoard(std::string[8] state = startingBoard);

	int halfTurnCount = 0;
	int fullTurnCount = 0;

	std::string enPassantColumn = "-";

	struct castleEligibility {
		bool whiteQueenside = false;
		bool whiteKingside = false;
		bool blackQueenside = false;
		bool blackKingside = false;
	};
};