#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
#include "chess.hpp"
#include <random>
using namespace ChessSimulator;

std::unordered_set<std::string> ChessSimulator::getLegalMoves(BoardState board, int row, int column) {
	char piece = board.getPieceAtSquare(row, column);
	if (!isalpha(piece)) {
		return std::unordered_set<std::string>;
	}
	uint8_t color = Color.white;
	char p;
	Piece pieceStruct;
	if (islower(piece)){
		color = Color.black;
	}
	p = toupper(piece);
	switch (piece)
	{
		case 'P':
			Pawn pawn;
			pieceStruct = pawn;
			break;
		case 'R':
			Rook rook;
			pieceStruct = rook;
			break;
		case 'N':
			Knight knight;
			pieceStruct = knight;
			break;
		case 'B':
			Bishop bishop;
			pieceStruct = bishop;
			break;
		case 'Q':
			Queen queen;
			pieceStruct = queen;
			break;
		case 'K':
			King king;
			pieceStruct = king;
			break;
	}
	pieceStruct.color = color;
	return pieceStruct.getLegalMoves(row,column);
}

std::string ChessSimulator::Move(std::string fen) {
	std::srand(std::time(nullptr));
	// get all pieces,
    // for each piece:
    // get all legal moves
    // pick a random move from this set
	std::unordered_set<std::string> allMoves;
	BoardState board(fen);
	Piece::board = board;
	for (int i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++){
			allMoves.merge(getLegalMoves(board, i, j));
		}
	}

	auto iterator = allMoves.begin();
	std::advance(iterator, rand() % allMoves.size());

	return *iterator;

}

  // create your board based on the board string following the FEN notation
  // search for the best move using minimax / monte carlo tree search /
  // alpha-beta pruning / ... try to use nice heuristics to speed up the search
  // and have better results return the best move in UCI notation you will gain
  // extra points if you create your own board/move representation instead of
  // using the one provided by the library

  // 
  
  // here goes a random movement
  //chess::Board board(fen);
  //chess::Movelist moves;
  //chess::movegen::legalmoves(moves, board);
  //if(moves.size() == 0)
  //  return "";



  //// get random move
  //std::random_device rd;
  //std::mt19937 gen(rd());
  //std::uniform_int_distribution<> dist(0, moves.size() - 1);
  //auto move = moves[dist(gen)];
  //return chess::uci::moveToUci(move);
}