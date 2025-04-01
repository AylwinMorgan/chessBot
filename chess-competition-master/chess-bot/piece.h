#pragma once
#include <unordered_set>
#include "boardState.h"

struct Move {
	std::string from;
	std::string to;

	std::string promotion;
};

struct Piece {
	uint8_t color;
	virtual std::unordered_set<Move> getLegalMoves(std::string* board, int row, int column) = 0;
};

struct Pawn : Piece {
	std::unordered_set<Move> getLegalMoves(std::string* board, int row, int column);
};

struct Rook : Piece {
	std::unordered_set<Move> getLegalMoves(std::string* board, int row, int column);
};

struct Knight : Piece {
	std::unordered_set<Move> getLegalMoves(std::string* board, int row, int column);
};

struct Bishop : Piece {
	std::unordered_set<Move> getLegalMoves(std::string* board, int row, int column);
};

struct Queen : Piece {
	std::unordered_set<Move> getLegalMoves(std::string* board, int row, int column);
};

struct King : Piece {
	std::unordered_set<Move> getLegalMoves(std::string* board, int row, int column);
};