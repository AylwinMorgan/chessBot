#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
//#include "chess.hpp"
#include <random>
#include <iostream>
#include <cmath>
using namespace ChessSimulator;

BoardState ChessSimulator::getNewBoardStateFromMove(BoardState board, ChessMove move) {
	BoardState newBoard = BoardState();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			newBoard.boardArray.push_back(board.boardArray.at(i * 8 + j));
		}
	}

	int fromColumn = (int)move.from.at(0) - (int)'a';
	int fromRow = (int)move.from.at(1) - (int)'1';

	int toColumn = (int)move.to.at(0) - (int)'a';
	int toRow = (int)move.to.at(1) - (int)'1';

	// set ChessMove.to in the new board to be the piece at ChessMove.from
	newBoard.boardArray.at((7 - toRow) * 8 + toColumn) = newBoard.boardArray.at((7 - fromRow) * 8 + fromColumn);
	// set ChessMove.from in the new board to empty
	newBoard.boardArray.at((7 - fromRow) * 8 + fromColumn) = '-';
	return newBoard;
}

bool ChessSimulator::kingIsInCheck(BoardState board, ChessMove move, Color color) {
	// get a potential board state based on the move and see if the king of 'color' is in check
	// do this by looking at all enemy pieces and checking if any of their legal moves end at the 'color' king's position
	bool kingIsReached = false;

	BoardState newBoard = getNewBoardStateFromMove(board,move);

	// get all 
	bool wtm = color == Color::black;

	int kingRow;
	int kingColumn;
	std::string kingColumnString;
	std::string kingRowString;

	// find the king
	for (kingRow = 0; kingRow < 8; kingRow++) {
		for (kingColumn = 0; kingColumn < 8; kingColumn++) {
			char square = newBoard.boardArray.at((7 - kingRow) * 8 + kingColumn);
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
			std::unordered_set<ChessMove> enemyMoves = getLegalMoves(newBoard, i, j, wtm, false);
			// if any of the moves end on the king's square, return true
			for (ChessMove move : enemyMoves) {
				if (move.to == kingColumnString + kingRowString) {
					return true;
				}
			}
		}
	}
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

std::unordered_set<ChessMove> ChessSimulator::getAllLegalMoves(BoardState board, bool whiteToMove, bool considerCheck) {
	std::unordered_set<ChessMove> allMoves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {

			std::unordered_set<ChessMove> newMoves = getLegalMoves(board, i, j, whiteToMove, true);
			allMoves.insert(newMoves.begin(), newMoves.end());
		}
	}
	return allMoves;
}

std::string ChessSimulator::Move(std::string fen) {
	std::srand(std::time(nullptr));
	// get all pieces,
    // for each piece:
    // get all legal moves
    // pick a random move from this set
	BoardState board(fen);
	//std::string tempFen = "r1b1nb1r / 2p4q / 3n2p1 / pp1pP2p / PPPP1PkP / 6pR / 3K4 / R1BQNB2 b - e3 0 15";
	//BoardState board(tempFen);
	bool whiteToMove = fen.find('w') != std::string::npos;
	std::unordered_set<ChessMove> allMoves = getAllLegalMoves(board,whiteToMove,true);

	auto iterator = allMoves.begin();
	if (allMoves.size() > 0){
    	std::advance(iterator, rand() % allMoves.size());

    	ChessMove move = *iterator;

    	std::string finalMove = move.from + move.to + move.promotion;
	    return finalMove;
	}
	return "";
}



// uses minimax algorithm to get most favorable move based on heuristic
ChessMove ChessSimulator::getBestMove(std::unordered_set<ChessMove> moves, bool whiteToMove) {
	// look at all moves
	// for each move:
	// get all subsequent moves 
	
	// fill node children set with a node for all moves in moves
	// for each child, fill its child set with a node for each new possible set of moves
	ChessSimulator::minmaxNode* root = new minmaxNode;
	int depth = 3;
	int bestValue = ChessSimulator::minmax(root, depth, true, whiteToMove);
	ChessMove bestMove;

	for (minmaxNode* child : root->children) {
		if (child->heuristic == root->heuristic) {
			bestMove = child->move;
		}
	}

	delete root;
	return bestMove;
}

/*   minimax pseudocode for reference (from wikipedia)
function minimax(node, depth, maximizingPlayer) is
   if depth = 0 or node is a terminal node then
	   return the heuristic value of node
   if maximizingPlayer then
	   value := −∞
	   for each child of node do
		   value := max(value, minimax(child, depth − 1, FALSE))
	   return value
   else (* minimizing player *)
	   value := +∞
	   for each child of node do
		   value := min(value, minimax(child, depth − 1, TRUE))
	   return value
*/

// get value of chess move and apply it to node
// to do: add alpha-beta pruning to make this more efficient
int ChessSimulator::minmax(minmaxNode* node, int depth, bool maximize, bool whiteToMove) {
	
	
	// if deepest depth reached, return h-value of node based on board state
	if (depth == 0) {
		node->heuristic = ChessSimulator::getValueOfMove(node->board);
		return node;
	}

	// otherwise get all moves
	std::unordered_set<ChessMove> moves = getAllLegalMoves(node->board, whiteToMove, true);

	// if there are no moves, get current board value 
	// (this means checkmate or draw, examine this state carefully)
	if (moves.empty()) {
		node->heuristic = ChessSimulator::getValueOfMove(node->board);
		return node;
	}

	// otherwise, look at all possible moves and pick the one with the highest or lowest minmax value
	// for each possible move, create a child node and run minmax again from there (with 1 less depth and maximize switched)
	int value = maximize ? -INFINITY : INFINITY;

	for (ChessMove move : moves) {
		BoardState nextState = ChessSimulator::getNewBoardStateFromMove(node->board, move);
		ChessSimulator::minmaxNode* child = new ChessSimulator::minmaxNode;
		child->board = nextState;
		child->move = move;

		int childScore = ChessSimulator::minmax(child, depth - 1, !maximize, !whiteToMove);
		node->children.insert(child);

		if (maximize) {
			value = std::max(value, childScore);
		}
		else {
			value = std::min(value, childScore);
		}
	}

	node->heuristic = value;
	return value;
}

// to do: make this analyze the resulting board state
int ChessSimulator::getValueOfMove(BoardState board) {
	return 0;
}

// delete all child nodes when node is deleted
ChessSimulator::minmaxNode::~minmaxNode() {
	for (minmaxNode* child : children) {
		children.erase(child);
		delete child;
	}
}

// fen string manipulation (unneeded)
/*
std::string getFenFromMove(Move move, std::string fen) {
	/// SUMMARY OF OPERATIONS ///
	// update piece placements based on move
	// swap active color
	// ignore castling availability for now, assume neither side can castle
	// ignore en passant for now, assume no en passant is possible
	// add to half move clock or reset it if a capture or pawn advance occurred
	// update fullmove clock if black moved
	std::string newFen = fen;

	int fromColumn = (int)move.from.at(0) - (int)'a';
	int fromRow = (int)move.from.at(1) - (int)'1';

	int toColumn = (int)move.to.at(0) - (int)'a';
	int toRow = (int)move.to.at(1) - (int)'1';


	int currentRow, currentColumn;
	currentRow = 7;
	currentColumn = 0;
	char movingPiece;
	int index = 0;

	// sets the previously occupied position to empty
	for (char c : fen) {
		if (isalpha(c)) {
			if (currentColumn == fromColumn && currentRow == fromRow) {
				movingPiece = c;
				// if the value to the left or right of c are a number
				// erase c from the string and add to the adjacent value
				// if both left and right are numbers
				
				char leftValue = fen.at(index-1);
				char rightValue = fen.at(index+1);
				bool leftIsNumber = isdigit(leftValue);
				bool rightIsNumber = isdigit(rightValue);

				fen.at(index) = '1';
				// if number to left, erase value at index and add it to left
				if (leftIsNumber) {

					fen.erase(index,1);
					index--;
					fen.at(index) = leftValue + ()
				}
				// if number to right, erase value at index and add it to right
				if (rightIsNumber) {

				}
			}
			currentColumn++;
		}
		else if (isdigit(c)) {
			currentColumn += int(c - '0');
		}

		if (currentColumn > 7) {
			currentColumn = 0;
			currentRow++;
		}
		index++;
	}

	currentColumn = 0;
	currentRow = 7;
	for (char c : fen) {
		if (isalpha(c)) {
			if (currentColumn == toColumn && currentRow == toRow) {
				movingPiece = c;
				fen.at(index) = movingPiece;
			}
			currentColumn++;
		}
		else if (isdigit(c)) {
			currentColumn += int(c - '0');
		}

		if (currentColumn > 7) {
			currentColumn = 0;
			currentRow++;
		}
		index++;
	}

}
*/


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