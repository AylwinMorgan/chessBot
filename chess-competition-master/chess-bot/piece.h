#pragma once
#include <unordered_set>
#include "boardState.h"

struct Move {
	std::string from;
	std::string to;

	std::string promotion;
};

enum class Color std::uint8_t {
	white = 0,
	black = 1
};

struct Piece {
	uint8_t color;
	virtual std::unordered_set<std::string> getLegalMoves(int row, int column) = 0;
	bool squareIsValid(int row, int column);
	void addPieceIfSquareIsValid(int row, int column, std::unordered_set<std::string>& set);
	static BoardState board;
};

struct Pawn : Piece {
	std::unordered_set<std::string> getLegalMoves(int row, int column);
};

struct Rook : Piece {
	std::unordered_set<std::string> getLegalMoves(int row, int column);
};

struct Knight : Piece {
	std::unordered_set<std::string> getLegalMoves(int row, int column);
};

struct Bishop : Piece {
	std::unordered_set<std::string> getLegalMoves(int row, int column);
};

struct Queen : Piece {
	std::unordered_set<std::string> getLegalMoves(int row, int column);
};

struct King : Piece {
	std::unordered_set<std::string> getLegalMoves(int row, int column);
};