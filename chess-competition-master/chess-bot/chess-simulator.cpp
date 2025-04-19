#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
//#include "chess.hpp"
#include <random>
#include <iostream>
#include <cmath>
#include <limits>
using namespace ChessSimulator;


// use to look up distance from center of board
const int arrCenterDistance[64] = {
  3, 3, 3, 3, 3, 3, 3, 3,
  3, 2, 2, 2, 2, 2, 2, 3,
  3, 2, 1, 1, 1, 1, 2, 3,
  3, 2, 1, 0, 0, 1, 2, 3,
  3, 2, 1, 0, 0, 1, 2, 3,
  3, 2, 1, 1, 1, 1, 2, 3,
  3, 2, 2, 2, 2, 2, 2, 3,
  3, 3, 3, 3, 3, 3, 3, 3
};

// use following tables to get piece value at board location
// credit: chessprogramming.org for the numerical values
const int whitePawnValue[64] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  50, 50, 50, 50, 50, 50, 50, 50,
  10, 10, 20, 30, 30, 20, 10, 10,
  5,  5, 10, 25, 25, 10,  5,  5,
  0,  0,  0, 20, 20,  0,  0,  0,
  5, -5,-10,  0,  0,-10, -5,  5,
  5, 10, 10,-20,-20, 10, 10,  5,
  0,  0,  0,  0,  0,  0,  0,  0
};

const int blackPawnValue[64] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10,-20,-20, 10, 10,  5,
  5, -5,-10,  0,  0,-10, -5,  5,
  0,  0,  0, 20, 20,  0,  0,  0,
  5,  5, 10, 25, 25, 10,  5,  5,
  10, 10, 20, 30, 30, 20, 10, 10,
  50, 50, 50, 50, 50, 50, 50, 50,
  0,  0,  0,  0,  0,  0,  0,  0
};

const int whiteKnightValue[64] = {
  -50,-40,-30,-30,-30,-30,-40,-50,
  -40,-20,  0,  0,  0,  0,-20,-40,
  -30,  0, 10, 15, 15, 10,  0,-30,
  -30,  5, 15, 20, 20, 15,  5,-30,
  -30,  0, 15, 20, 20, 15,  0,-30,
  -30,  5, 10, 15, 15, 10,  5,-30,
  -40,-20,  0,  5,  5,  0,-20,-40,
  -50,-40,-30,-30,-30,-30,-40,-50,
};

const int blackKnightValue[64] = {
  -50,-40,-30,-30,-30,-30,-40,-50,
  -40,-20,  0,  5,  5,  0,-20,-40,
  -30,  5, 10, 15, 15, 10,  5,-30,
  -30,  0, 15, 20, 20, 15,  0,-30,
  -30,  5, 15, 20, 20, 15,  5,-30,
  -30,  0, 10, 15, 15, 10,  0,-30,
  -40,-20,  0,  0,  0,  0,-20,-40,
  -50,-40,-30,-30,-30,-30,-40,-50,
};

const int whiteBishopValue[64] = {
  -20,-10,-10,-10,-10,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5, 10, 10,  5,  0,-10,
  -10,  5,  5, 10, 10,  5,  5,-10,
  -10,  0, 10, 10, 10, 10,  0,-10,
  -10, 10, 10, 10, 10, 10, 10,-10,
  -10,  5,  0,  0,  0,  0,  5,-10,
  -20,-10,-10,-10,-10,-10,-10,-20
};

const int blackBishopValue[64] = {
  -20,-10,-10,-10,-10,-10,-10,-20,
  -10,  5,  0,  0,  0,  0,  5,-10,
  -10, 10, 10, 10, 10, 10, 10,-10,
  -10,  0, 10, 10, 10, 10,  0,-10,
  -10,  5,  5, 10, 10,  5,  5,-10,
  -10,  0,  5, 10, 10,  5,  0,-10,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -20,-10,-10,-10,-10,-10,-10,-20
};

const int whiteRookValue[64] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0
};

const int blackRookValue[64] = {
  0,  0,  0,  5,  5,  0,  0,  0,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  5, 10, 10, 10, 10, 10, 10,  5,
  0,  0,  0,  0,  0,  0,  0,  0
};

const int whiteQueenValue[64] = {
  -20,-10,-10, -5, -5,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5,  5,  5,  5,  0,-10,
  -5,  0,  5,  5,  5,  5,  0, -5,
   0,  0,  5,  5,  5,  5,  0, -5,
  -10,  5,  5,  5,  5,  5,  0,-10,
  -10,  0,  5,  0,  0,  0,  0,-10,
  -20,-10,-10, -5, -5,-10,-10,-20
};

const int blackQueenValue[64] = {
  -20,-10,-10, -5, -5,-10,-10,-20,
  -10,  0,  5,  0,  0,  0,  0,-10,
  -10,  5,  5,  5,  5,  5,  0,-10,
   0,  0,  5,  5,  5,  5,  0, -5,
  -5,  0,  5,  5,  5,  5,  0, -5,
  -10,  0,  5,  5,  5,  5,  0,-10,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -20,-10,-10, -5, -5,-10,-10,-20
};

const int whiteKingValue[64] = {
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -20,-30,-30,-40,-40,-30,-30,-20,
  -10,-20,-20,-20,-20,-20,-20,-10,
   20, 20,  0,  0,  0,  0, 20, 20,
   20, 30, 10,  0,  0, 10, 30, 20
};

const int blackKingValue[64] = {
   20, 30, 10,  0,  0, 10, 30, 20,
   20, 20,  0,  0,  0,  0, 20, 20,
  -10,-20,-20,-20,-20,-20,-20,-10,
  -20,-30,-30,-40,-40,-30,-30,-20,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30
};

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

	char piece = newBoard.boardArray.at((7 - fromRow) * 8 + fromColumn);
	char targetSquareContents = newBoard.boardArray.at((7 - toRow) * 8 + toColumn);

	if (!move.promotion.empty()) {
		if (isupper(piece)){
			piece = toupper(move.promotion.at(0));
		}
		else {
			piece = move.promotion.at(0);
		}
	}
	// set ChessMove.to in the new board to be the piece at ChessMove.from
	newBoard.boardArray.at((7 - toRow) * 8 + toColumn) = piece;
	// set ChessMove.from in the new board to empty
	// if the move was a castle, move the rook as well
	if (move.castle) {
		char rook = 'r';
		if (isupper(piece)) {
			rook = 'R';
		}
		if (toColumn == 7) {
			newBoard.boardArray.at((7 - toRow) * 8 + toColumn - 1) = rook;
		}
		else {
			newBoard.boardArray.at((7 - toRow) * 8 + toColumn + 1) = rook;
		}
	}
	newBoard.boardArray.at((7 - fromRow) * 8 + fromColumn) = '-';
	return newBoard;
}

// uses board state and move to calculate new board state
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

// takes in already calculated new board state
bool ChessSimulator::kingIsInCheck(BoardState board, bool checkWhite) {
	int kingRow;
	int kingColumn;
	std::string kingColumnString;
	std::string kingRowString;

	// find the king
	for (kingRow = 0; kingRow < 8; kingRow++) {
		for (kingColumn = 0; kingColumn < 8; kingColumn++) {
			char square = board.boardArray.at((7 - kingRow) * 8 + kingColumn);
			if ((((checkWhite && isupper(square)) || !checkWhite && islower(square))) && tolower(square) == 'k') {
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
			std::unordered_set<ChessMove> enemyMoves = getLegalMoves(board, i, j, !checkWhite, false);
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

	ChessMove move = getBestMove(allMoves, board, whiteToMove);

	std::string finalMove = move.from + move.to + move.promotion;
	return finalMove;
	/*
	auto iterator = allMoves.begin();
	if (allMoves.size() > 0){
    	std::advance(iterator, rand() % allMoves.size());

    	ChessMove move = *iterator;

    	std::string finalMove = move.from + move.to + move.promotion;
	    return finalMove;
	}
	*/
	//return "";
}

// uses minimax algorithm to get most favorable move based on heuristic
ChessMove ChessSimulator::getBestMove(std::unordered_set<ChessMove> moves, BoardState board, bool whiteToMove) {
	// look at all moves
	// for each move:
	// get all subsequent moves 
	
	// fill node children set with a node for all moves in moves
	// for each child, fill its child set with a node for each new possible set of moves
	ChessSimulator::minmaxNode* root = new minmaxNode;
	root->board = board;
	int depth = 3;
	int bestValue = ChessSimulator::minmax(root, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), true, whiteToMove);
	ChessMove bestMove;

	for (minmaxNode* child : root->children) {
		if (child->heuristic == root->heuristic) {
			bestMove = child->move;
			break;
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
int ChessSimulator::minmax(minmaxNode* node, int depth, int alpha, int beta, bool maximize, bool whiteToMove) {
	if (depth == 0) {
		node->heuristic = ChessSimulator::getValueOfBoard(node->board, !whiteToMove);
		return node->heuristic;
	}

	
	// if deepest depth reached, return h-value of node based on board state
	std::unordered_set<ChessMove> moves = getAllLegalMoves(node->board, whiteToMove, true);

	// if there are no moves, get current board value 
	// (this means checkmate or draw, examine this state carefully)
	if (moves.empty() || depth == 0) {
		node->heuristic = ChessSimulator::getValueOfBoard(node->board, !whiteToMove);
		// if there are no valid moves and moving color piece is in check
		// return very low value
		// if non moving color 
		if (moves.empty() && kingIsInCheck(node->board, whiteToMove)) {
			// if depth is even, then minmax is currently looking at the non starting color
			bool originalColor = whiteToMove;
			if (depth % 2 == 0) {
				originalColor = !originalColor;
			}
			// if original color is in checkmate, avoid this outcome 
			if (kingIsInCheck(node->board, originalColor)) {
				node->heuristic = -10000000;
				return -10000000;
			}
			// if the other color is in checkmate, prioritize this state above all else
			else if (kingIsInCheck(node->board, !originalColor)) {
				node->heuristic = 10000000;
				return 10000000;
			}
		}
		return node->heuristic;
	}

	// otherwise, look at all possible moves and pick the one with the highest or lowest minmax value
	// for each possible move, create a child node and run minmax again from there (with 1 less depth and maximize switched)
	int value = maximize ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

	for (ChessMove move : moves) {
		BoardState nextState = ChessSimulator::getNewBoardStateFromMove(node->board, move);
		
		ChessSimulator::minmaxNode* child = new ChessSimulator::minmaxNode;
		child->board = nextState;
		child->move = move;

		int childScore = ChessSimulator::minmax(child, depth - 1, alpha, beta, !maximize, !whiteToMove);
		node->children.insert(child);

		if (maximize) {
			value = std::max(value, childScore);
			alpha = std::max(alpha, value);
		}
		else {
			value = std::min(value, childScore);
			beta = std::min(beta, value);
		}

		// alpha beta pruning
		if (beta <= alpha) {
			break;
		}
	}

	node->heuristic = value;
	return value;
}

// calculate value of board state based on which pieces both sides have
// also calculate value of piece value based on its location
int ChessSimulator::getColorScore(BoardState board, bool checkWhite) {
	int total = 0;
	int boardPosition = 0;

	int compositionValueFactor = 15;
	int positionValueFactor = 1;

	for (char c : board.boardArray) {
		int row = boardPosition / 8;
		int column = boardPosition % 8;
		int scoreFactor = 1;
		bool pieceIsWhite = isupper(c);
		if (isupper(c) != checkWhite){
			scoreFactor = -1;
		}
		char lower = tolower(c);
		int compositionValue = 0;
		int positionValue = 0;
		switch (lower)
		{
			case 'p':
				// pawns are worth 1 point overall and give value based on how far they have advanced
				compositionValue = 1 * compositionValueFactor;
				if (pieceIsWhite) {
					positionValue = whitePawnValue[boardPosition];
				}
				else {
					positionValue = blackPawnValue[boardPosition];
				}
				break;
			case 'n':
				compositionValue = 3;
				if (pieceIsWhite) {
					positionValue = whiteKnightValue[boardPosition];
				}
				else {
					positionValue = blackKnightValue[boardPosition];
				}
				break;
			case 'b':
				// knight and bishop are worth more when closer to the center 4 squares of the board
				compositionValue = 3;
				if (pieceIsWhite) {
					positionValue = whiteBishopValue[boardPosition];
				}
				else {
					positionValue = blackBishopValue[boardPosition];
				}
				break;
			case 'r':
				compositionValue = 5;
				if (pieceIsWhite) {
					positionValue = whiteRookValue[boardPosition];
				}
				else {
					positionValue = blackRookValue[boardPosition];
				}
				break;
			case 'q':
				compositionValue = 9;
				if (pieceIsWhite) {
					positionValue = whiteQueenValue[boardPosition];
				}
				else {
					positionValue = blackQueenValue[boardPosition];
				}
				break;
			case 'k':
				compositionValue = 1000;
				if (pieceIsWhite) {
					positionValue = whiteKingValue[boardPosition];
				}
				else {
					positionValue = blackKingValue[boardPosition];
				}
				break;
		}
		compositionValue *= compositionValueFactor;
		positionValue *= positionValueFactor;

		total += (compositionValue + positionValue) * scoreFactor;
		boardPosition++;
	}
	return total;
}

// to do: make this analyze the resulting board state
int ChessSimulator::getValueOfBoard(BoardState board, bool whiteToMove) {
	int value = 0;
	if (kingIsInCheck(board, !whiteToMove)) {
		value += 100;
	}
	return ChessSimulator::getColorScore(board, whiteToMove);
}

// delete all child nodes when node is deleted
ChessSimulator::minmaxNode::~minmaxNode() {
	for (minmaxNode* child : children) {
		delete child;
	}
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