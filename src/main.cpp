#include <iostream>
#include <vector>

class Board {
	public:
		Board();
		void setStartingPosition();
		bool attacks(uint8_t pieceRank, uint8_t pieceFile, uint8_t targetRank, uint8_t targetFile);
		std::vector<uint8_t> getLegalMoves(uint8_t pieceRank, uint8_t pieceFile);
		bool getSide(uint8_t pieceRank, uint8_t pieceFile);

		static constexpr bool WHITE = false;
		static constexpr bool BLACK = true;

		static constexpr uint8_t EMPTY = 0;
		static constexpr uint8_t WPAWN = 1;
		static constexpr uint8_t WKNIGHT = 2;
		static constexpr uint8_t WBISHOP = 3;
		static constexpr uint8_t WROOK = 4;
		static constexpr uint8_t WQUEEN = 5;
		static constexpr uint8_t WKING = 6;
		static constexpr uint8_t BPAWN = 7;
		static constexpr uint8_t BKNIGHT = 8;
		static constexpr uint8_t BBISHOP = 9;
		static constexpr uint8_t BROOK = 10;
		static constexpr uint8_t BQUEEN = 11;
		static constexpr uint8_t BKING = 12;
	private:
		void clearPosition();
		uint8_t board[8][8];
}; // class Board

Board::Board() {
	this->clearPosition();
}

void Board::setStartingPosition() {
	this->clearPosition();
	this->board[0][0] = WROOK;
	this->board[1][0] = WKNIGHT;
	this->board[2][0] = WBISHOP;
	this->board[3][0] = WQUEEN;
	this->board[4][0] = WKING;
	this->board[5][0] = WBISHOP;
	this->board[6][0] = WKNIGHT;
	this->board[7][0] = WROOK;

	this->board[0][7] = BROOK;
	this->board[1][7] = BKNIGHT;
	this->board[2][7] = BBISHOP;
	this->board[3][7] = BQUEEN;
	this->board[4][7] = BKING;
	this->board[5][7] = BBISHOP;
	this->board[6][7] = BKNIGHT;
	this->board[7][7] = BROOK;

	for (int i = 0; i < 8; i++) {
		this->board[1][i] = WPAWN;
		this->board[6][i] = BPAWN;
	}
}

bool Board::attacks(uint8_t pieceRank, uint8_t pieceFile, uint8_t targetRank, uint8_t targetFile) {
	
}

std::vector<uint8_t> Board::getLegalMoves(uint8_t pieceRank, uint8_t pieceFile) {
	std::vector<uint8_t> legalMoves;
	switch (this->board[pieceRank][pieceFile]) {
		case EMPTY:
			break;
		case WPAWN:
			legalMoves.push_back((pieceRank + 1) * 8 + pieceFile);
			if (pieceRank == 1) {
				legalMoves.push_back((pieceRank + 2) * 8 + pieceFile);
			}
			if (this->board[pieceRank + 1][pieceFile - 1] != EMPTY) {
				legalMoves.push_back((pieceRank + 1) * 8 + pieceFile - 1);
			}
			if (this->board[pieceRank + 1][pieceFile + 1] != EMPTY) {
				legalMoves.push_back((pieceRank + 1) * 8 + pieceFile + 1);
			}
			// TODO implement en passant
		case BPAWN:
			break;
		case WKNIGHT:
		case BKNIGHT:
			break;
		case WBISHOP:
		case BBISHOP:
			break;
		case WROOK:
		case BROOK:
			break;
		case WQUEEN:
		case BQUEEN:
			break;
		case WKING:
		case BKING:
			break;
	}
}

bool Board::getSide(uint8_t pieceRank, uint8_t pieceFile) {
	return (this->board[pieceRank][pieceFile] > WKING);
}

void Board::clearPosition() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			this->board[i][j] = EMPTY;
		}
	}
}

int main() {
	
	return 0;
}