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
std::unordered_set<std::string> getLegalMoves(BoardState board, int row, int column);
std::string Move(std::string fen);
} // namespace ChessSimulator