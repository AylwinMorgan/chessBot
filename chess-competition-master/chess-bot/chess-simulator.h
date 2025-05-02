#pragma once
#include <string>
#include "piece.h"
#include "unordered_set"
#include "boardState.h"
#include <ctime>

namespace ChessSimulator {
/**
 * @brief Move a piece on the board
 *
 * @param fen The board as FEN
 * @return std::string The move as UCI
 */

 struct minmaxNode {
	~minmaxNode();

	ChessMove move;
	BoardState board;
	int heuristic;
	std::unordered_set<minmaxNode*> children = std::unordered_set<minmaxNode*>();
 };

 enum class PieceType : std::uint8_t {
	 EMPTY = 0b000,
	 PAWN = 0b001,
	 KNIGHT = 0b010,
	 BISHOP = 0b011,
	 ROOK = 0b100,
	 QUEEN = 0b101,
	 KING = 0b110
 };

 enum class PieceColor : std::uint8_t {
	 BLACK = 0b0,
	 WHITE = 0b1
 };

 enum class SquareContent : std::uint8_t {
	 EMPTY = 0b0000,
	 BLACK_PAWN = 0b0001,
	 BLACK_KNIGHT = 0b0010,
	 BLACK_BISHOP = 0b0011,
	 BLACK_ROOK = 0b0100,
	 BLACK_QUEEN = 0b0101,
	 BLACK_KING = 0b0110,
	 WHITE_PAWN = 0b1001,
	 WHITE_KNIGHT = 0b1010,
	 WHITE_BISHOP = 0b1011,
	 WHITE_ROOK = 0b1100,
	 WHITE_QUEEN = 0b1101,
	 WHITE_KING = 0b1110
 };

 struct Square {
	 PieceColor color : 1;
	 PieceType  type : 3;
 };


int getValueOfBoard(BoardState& board, bool whiteToMove, bool originallyWhite);
int getColorScore(BoardState& board, bool checkWhite, bool originallyWhite);
bool squareIsThreatened(BoardState& board, int row, int column, bool checkWhite);
bool kingIsInCheck(BoardState& board, ChessMove move, Color color); 
bool kingIsInCheck(BoardState& board, bool checkWhite);
std::unordered_set<ChessMove> getLegalMoves(BoardState& board, int row, int column, bool whiteToMove, bool considerCheck, bool threatenOwnPieces = false);
std::string Move(std::string fen);
std::unordered_set<ChessMove> getAllLegalMoves(BoardState& board, bool whiteToMove, bool considerCheck);

// needs implmentation
ChessMove getBestMove(std::unordered_set<ChessMove> moves, BoardState& board, bool whiteToMove);

// needs implementation
std::unordered_set<minmaxNode*> generateMinmaxChildren(BoardState& board);
int minmax(minmaxNode* node, int depth, int alpha, int beta, bool maximize, bool whiteToMove, bool originallyWhite);


BoardState getNewBoardStateFromMove(BoardState& board, ChessMove move);
//std::string getFenFromMove(Move move, std::string fen);
} // namespace ChessSimulator