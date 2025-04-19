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


int getValueOfBoard(BoardState board, bool whiteToMove);
int getColorScore(BoardState board, bool checkWhite);
bool kingIsInCheck(BoardState board, ChessMove move, Color color); 
bool kingIsInCheck(BoardState board, bool checkWhite);
std::unordered_set<ChessMove> getLegalMoves(BoardState board, int row, int column, bool whiteToMove, bool considerCheck);
std::string Move(std::string fen);
std::unordered_set<ChessMove> getAllLegalMoves(BoardState board, bool whiteToMove, bool considerCheck);

// needs implmentation
ChessMove getBestMove(std::unordered_set<ChessMove> moves, BoardState board, bool whiteToMove);

// needs implementation
std::unordered_set<minmaxNode*> generateMinmaxChildren(BoardState board);
int minmax(minmaxNode* node, int depth, int alpha, int beta, bool maximize, bool whiteToMove);


BoardState getNewBoardStateFromMove(BoardState board, ChessMove move);
//std::string getFenFromMove(Move move, std::string fen);
} // namespace ChessSimulator