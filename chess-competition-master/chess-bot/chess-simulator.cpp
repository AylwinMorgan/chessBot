#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
//#include "chess.hpp"
#include <random>
using namespace ChessSimulator;

bool ChessSimulator::kingIsInCheck(BoardState board, ChessMove move, Color color) {
	// get a potential board state based on the move and see if the king of 'color' is in check
	// do this by looking at all enemy pieces and checking if any of their legal moves end at the 'color' king's position
	bool kingIsReached = false;
	BoardState* newBoard = new BoardState;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			newBoard->boardArray.push_back(board.boardArray[(7 - i) * 8 + j]);
		}
	}

	int fromColumn = (int)move.from[0] - (int)'a';
	int fromRow = (int)move.from[1] - (int)'1';

	int toColumn = (int)move.to[0] - (int)'a';
	int toRow = (int)move.to[1] - (int)'1';

	// set ChessMove.to in the new board to be the piece at ChessMove.from
	newBoard->boardArray[(7 - toRow) * 8 + toColumn] = newBoard->boardArray[fromRow * 8 + fromColumn];
	// set ChessMove.from in the new board to empty
	newBoard->boardArray[(7 - fromRow) * 8 + fromColumn] = '-';

	// get all 
	bool wtm = color == Color::black;

	int kingRow;
	int kingColumn;
	std::string kingColumnString;

	// find the king
	for (kingRow = 0; kingRow < 8; kingRow++) {
		for (kingColumn = 0; kingColumn < 8; kingColumn++) {
			char square = newBoard->boardArray[(7 - kingRow) * 8 + kingColumn];
			if (((color == Color::white && isupper(square)) || (color == Color::black && islower(square))) && tolower(square) == 'k') {
				kingColumnString = board.getColumnLetter(kingColumn);
				break;
			}
		}
	}


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// get all moves that the enemy piece can do on the new board
			std::unordered_set<ChessMove> enemyMoves = getLegalMoves(*newBoard, i, j, wtm, false);
			// if any of the moves end on the king's square, return false
			for (ChessMove move : enemyMoves) {
				if (move.to == kingColumnString + std::to_string(kingRow)) {
					delete newBoard;
					return false;
				}
			}
		}
	}
	delete newBoard;
	return false;
}


std::unordered_set<ChessMove> ChessSimulator::getLegalMoves(BoardState board, int row, int column, bool whiteToMove, bool considerCheck) {
	char piece = board.getPieceAtSquare(row, column);
	if (!isalpha(piece)) {
		return std::unordered_set<ChessMove>();
	}
	bool pieceIsCorrectColor = whiteToMove;
	Color color = Color::white;
	char p;
	Piece* pieceStruct = new Pawn();
	if (islower(piece)){
		color = Color::black;
		if (whiteToMove) {
			pieceIsCorrectColor = false;
		}
		else {
			pieceIsCorrectColor = true;
		}
	}
	p = toupper(piece);
	if (pieceIsCorrectColor){
		switch (p)
		{
			case 'P':
			{
				break;
			}
			case 'R':
			{
				Rook* rook = new Rook();
				pieceStruct = rook;
				break;
			}
			case 'N':
			{
				Knight* knight = new Knight();
				pieceStruct = knight;
				break;
			}
			case 'B':
			{
				Bishop* bishop = new Bishop();
				pieceStruct = bishop;
				break;
			}
			case 'Q':
			{
				Queen* queen = new Queen();
				pieceStruct = queen;
				break;
			}
			case 'K':
			{
				King* king = new King();
				pieceStruct = king;
				break;
			}
		}
	pieceStruct->color = color;
	std::unordered_set<ChessMove> moves = pieceStruct->getLegalMoves(row,column);
	if (considerCheck) {
		for (ChessMove move : moves) {
			if (kingIsInCheck(board, move, color)) {
				moves.erase(move);
			}
		}
	}
	delete pieceStruct;
	return moves;
	}
	delete pieceStruct;
	return std::unordered_set<ChessMove>();
}

std::string ChessSimulator::Move(std::string fen) {
	std::srand(std::time(nullptr));
	// get all pieces,
    // for each piece:
    // get all legal moves
    // pick a random move from this set
	std::unordered_set<ChessMove> allMoves;
	BoardState board(fen);
	bool whiteToMove = fen.find('w') != std::string::npos;
	Piece::board = board;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){

			std::unordered_set<ChessMove> newMoves = getLegalMoves(board,i,j, whiteToMove, true);
			allMoves.insert(newMoves.begin(), newMoves.end());
		}
	}

	auto iterator = allMoves.begin();
	std::advance(iterator, rand() % allMoves.size());

	ChessMove move = *iterator;

	std::string finalMove = move.from + move.to + move.promotion;
	return finalMove;

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