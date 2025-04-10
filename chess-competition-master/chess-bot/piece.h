#pragma once
#include <unordered_set>
#include "boardState.h"


enum class Color: std::uint8_t {
	white = 0,
	black = 1
};

struct Piece {
	Color color;
	virtual std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board) = 0;
	bool squareIsValid(int row, int column, BoardState board);
};

struct Pawn : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board);
};

struct Rook : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board);
};

struct Knight : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board);
};

struct Bishop : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board);
};

struct Queen : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board);
};

struct King : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board);
};