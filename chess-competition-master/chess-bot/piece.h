#pragma once
#include <unordered_set>
#include "boardState.h"


enum class Color: std::uint8_t {
	white = 0,
	black = 1
};

struct Piece {
	Color color;
	virtual std::unordered_set<ChessMove> getLegalMoves(int row, int column) = 0;
	bool squareIsValid(int row, int column);
	static BoardState board;
};

struct Pawn : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column);
};

struct Rook : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column);
};

struct Knight : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column);
};

struct Bishop : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column);
};

struct Queen : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column);
};

struct King : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column);
};