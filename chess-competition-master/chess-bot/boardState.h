#pragma once
#include <string>
#include <ctype.h>

/*  optimizations can be done later after a basic engine is set up
enum class Pieces: std::uint8_t {
	EMPTY = 0b000,
	PAWN = 0b001,
	KNIGHT = 0b010,
	BISHOP = 0b011,
	ROOK = 0b100,
	QUEEN = 0b101,
	KING = 0b110
};

enum class Color std::uint8_t {
	white = 0,
	black = 1
};

struct Square : uint8_t {
	Color color:0;

};

struct Board {

};
*/
class BoardState {
	BoardState();
	BoardState(std::string fen);
	std::string board;
	std::string getColumnLetter(int column);
	std::string getUCINotation(int startRow, int startColumn, int endRow, int endColumn);
	std::string getPieceAtSquare(int row, int column);

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