#pragma once
#include <string>
#include <ctype.h>
#include <vector>

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

struct Square : uint8_t {
	Color color:0;

};

struct Board {

};
*/
struct ChessMove {
	std::string from;
	std::string to;

	std::string promotion = "";
	bool castle = false;

	bool operator==(const ChessMove& other) const {
		return (from == other.from && to == other.to && promotion == other.promotion);
	}
};

// credit to this stackoverflow post for showing me how to do this: https://stackoverflow.com/questions/50888127/how-can-i-use-an-unordered-set-with-a-custom-struct
namespace std
{
	template<> struct hash<ChessMove>
	{
		std::size_t operator()(const ChessMove& move) const noexcept
		{
			std::size_t from = std::hash<std::string>()(move.from);
			std::size_t to = std::hash<std::string>()(move.to);
			std::size_t promotion = std::hash<std::string>()(move.promotion);

			return from ^ (to << 1) ^ (promotion << 2);
		}
	};
}


struct Threats {
	bool white = false;
	bool black = false;
};

class BoardState {
	public:
	~BoardState();
	BoardState();
	BoardState(std::string fen);
	std::string board;
	std::vector<char> boardArray;

	std::vector<Threats> boardThreats;

	std::string getColumnLetter(int column);

	ChessMove getUCINotation(int startRow, int startColumn, int endRow, int endColumn, char promotion = 0, bool castle = false);
	char getPieceAtSquare(int row, int column);

	int halfTurnCount = 0;
	int fullTurnCount = 0;

	std::string enPassantColumn = "-";

	struct castle {
		bool whiteQueenside = false;
		bool whiteKingside = false;
		bool blackQueenside = false;
		bool blackKingside = false;
	};




	castle castleEligibility;
};