#pragma once
#include <unordered_set>
#include "boardState.h"


enum class Color: std::uint8_t {
	white = 0,
	black = 1
};

struct Piece {
	Color color;
	virtual std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board, bool threatenOwnColor = false) = 0;
	bool squareIsValid(int row, int column, BoardState& board, bool considerColor);
};

struct Pawn : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board, bool threatenOwnColor = false);
};

struct Rook : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board, bool threatenOwnColor = false);
};

struct Knight : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board, bool threatenOwnColor = false);
};

struct Bishop : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board, bool threatenOwnColor = false);
};

struct Queen : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board, bool threatenOwnColor = false);
};

struct King : Piece {
	std::unordered_set<ChessMove> getLegalMoves(int row, int column, BoardState board, bool threatenOwnColor = false);
};