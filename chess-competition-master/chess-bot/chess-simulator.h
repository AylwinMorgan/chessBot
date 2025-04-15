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

 // 




// 
 struct minmaxNode {
	~minmaxNode();

	ChessMove move;
	BoardState board;
	int heuristic;
	std::unordered_set<minmaxNode*> children = std::unordered_set<minmaxNode*>();
 };


int getValueOfMove(ChessMove move, BoardState board);
bool kingIsInCheck(BoardState board, ChessMove move, Color color);
std::unordered_set<ChessMove> getLegalMoves(BoardState board, int row, int column, bool whiteToMove, bool considerCheck);
std::string Move(std::string fen);
std::unordered_set<ChessMove> getAllLegalMoves(BoardState board, bool whiteToMove, bool considerCheck);

// needs implmentation
ChessMove getBestMove(std::unordered_set<ChessMove> moves, bool whiteToMove);

// needs implementation
std::unordered_set<minmaxNode*> generateMinmaxChildren(BoardState board);
int minmax(minmaxNode* node, int depth, bool maximize);


BoardState getNewBoardStateFromMove(BoardState board, ChessMove move);
//std::string getFenFromMove(Move move, std::string fen);
} // namespace ChessSimulator