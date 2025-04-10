#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
//#include "chess.hpp"
#include <random>
#include <iostream>
using namespace ChessSimulator;

bool ChessSimulator::kingIsInCheck(BoardState board, ChessMove move, Color color) {
	// get a potential board state based on the move and see if the king of 'color' is in check
	// do this by looking at all enemy pieces and checking if any of their legal moves end at the 'color' king's position
	bool kingIsReached = false;
	BoardState* newBoard = new BoardState;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			newBoard->boardArray.push_back(board.boardArray.at(i * 8 + j));
		}
	}

	int fromColumn = (int)move.from.at(0) - (int)'a';
	int fromRow = (int)move.from.at(1) - (int)'1';

	int toColumn = (int)move.to.at(0) - (int)'a';
	int toRow = (int)move.to.at(1) - (int)'1';

	// set ChessMove.to in the new board to be the piece at ChessMove.from
	newBoard->boardArray.at((7 - toRow) * 8 + toColumn) = newBoard->boardArray.at((7 - fromRow) * 8 + fromColumn);
	// set ChessMove.from in the new board to empty
	newBoard->boardArray.at((7 - fromRow) * 8 + fromColumn) = '-';

	// get all 
	bool wtm = color == Color::black;

	int kingRow;
	int kingColumn;
	std::string kingColumnString;
	std::string kingRowString;

	// find the king
	for (kingRow = 0; kingRow < 8; kingRow++) {
		for (kingColumn = 0; kingColumn < 8; kingColumn++) {
			char square = newBoard->boardArray.at((7 - kingRow) * 8 + kingColumn);
			if (((color == Color::white && isupper(square)) || (color == Color::black && islower(square))) && tolower(square) == 'k') {
				kingColumnString = board.getColumnLetter(kingColumn);
				kingRowString = std::to_string(kingRow + 1);
				//std::cout << "king found at: " << kingColumnString << kingRowString << std::endl;
				goto end_loop;
			}
		}
	}
	end_loop:

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// get all moves that the enemy piece can do on the new board
			std::unordered_set<ChessMove> enemyMoves = getLegalMoves(*newBoard, i, j, wtm, false);
			// if any of the moves end on the king's square, return true
			for (ChessMove move : enemyMoves) {
				if (move.to == kingColumnString + kingRowString) {
					newBoard = nullptr;
					delete newBoard;
					return true;
				}
			}
		}
	}
	newBoard = nullptr;
	delete newBoard;
	return false;
}


std::unordered_set<ChessMove> ChessSimulator::getLegalMoves(BoardState board, int row, int column, bool whiteToMove, bool considerCheck) {

	char piece = board.getPieceAtSquare(row, column);
	if (!isalpha(piece)) {
		return std::unordered_set<ChessMove>();
	}
	Pawn* pawn = new Pawn();
	Rook* rook = new Rook();
	Knight* knight = new Knight();
	Bishop* bishop = new Bishop();
	Queen* queen = new Queen();
	King* king = new King();

	bool pieceIsCorrectColor = whiteToMove;
	Color color = Color::white;
	char p;
	std::unordered_set<ChessMove> moves = std::unordered_set<ChessMove>();

	Piece* pieceStruct;
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
			    pieceStruct = pawn;
				break;
			}
			case 'R':
			{
				pieceStruct = rook;
				break;
			}
			case 'N':
			{
				pieceStruct = knight;
				break;
			}
			case 'B':
			{
				pieceStruct = bishop;
				break;
			}
			case 'Q':
			{
				pieceStruct = queen;
				break;
			}
			case 'K':
			{
				pieceStruct = king;
				break;
			}
			default:
			{
				pieceStruct = pawn;
			}
		}
		pieceStruct->color = color;
		moves = pieceStruct->getLegalMoves(row,column,board);
		std::unordered_set<ChessMove> toDelete;
		if (considerCheck) {
			for (ChessMove move : moves) {
				if (kingIsInCheck(board, move, color)) {
					toDelete.insert(move);
					//moves.erase(move);
				}
			}
			for (ChessMove move : toDelete) {
				moves.erase(move);
			}
		}
	}
	delete pawn;
	delete rook;
	delete bishop;
	delete knight;
	delete queen;
	delete king;
	return moves;
}

std::string ChessSimulator::Move(std::string fen) {
	std::srand(std::time(nullptr));
	// get all pieces,
    // for each piece:
    // get all legal moves
    // pick a random move from this set
	std::unordered_set<ChessMove> allMoves;
	BoardState board(fen);
	//std::string tempFen = "r1b1nb1r / 2p4q / 3n2p1 / pp1pP2p / PPPP1PkP / 6pR / 3K4 / R1BQNB2 b - e3 0 15";
	//BoardState board(tempFen);
	bool whiteToMove = fen.find('w') != std::string::npos;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){

			std::unordered_set<ChessMove> newMoves = getLegalMoves(board,i,j, whiteToMove, true);
			allMoves.insert(newMoves.begin(), newMoves.end());
		}
	}

	auto iterator = allMoves.begin();
	if (allMoves.size() > 0){
    	std::advance(iterator, rand() % allMoves.size());

    	ChessMove move = *iterator;

    	std::string finalMove = move.from + move.to + move.promotion;
	    return finalMove;
	}
	return "";
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