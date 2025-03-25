enum class Color std::uint8_t {
	white = 0,
	black = 1
};

struct Move {
	std::string from;
	std::string to;

	std::string promotion;
};

struct Piece {
	uint8_t color;
	virtual std::vector<Move> getLegalMoves() = 0;
};

struct Pawn : Piece {
	std::vector<Move> getLegalMoves(std::string* board, int row, int column);
};

struct Rook : Piece {
	std::vector<Move> getLegalMoves();
};

struct Knight : Piece {
	std::vector<Move> getLegalMoves();
};

struct Bishop : Piece {
	std::vector<Move> getLegalMoves();
};

struct Queen : Piece {
	std::vector<Move> getLegalMoves();
};

struct King : Piece {
	std::vector<Move> getLegalMoves();
};