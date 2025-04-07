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
bool kingIsInCheck(BoardState board, ChessMove move, Color color);
std::unordered_set<ChessMove> getLegalMoves(BoardState board, int row, int column, bool whiteToMove, bool considerCheck);
std::string Move(std::string fen);
} // namespace ChessSimulator