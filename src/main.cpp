#include <iostream>
#include <vector>

struct Coordinates {
	int file, rank;
	bool operator==(Coordinates other) {
		return (this->file == other.file && this->rank == other.rank);
	}
}; // struct Coordinates

class Board {
	public:
		Board();
		void setStartingPosition();
		std::vector<Coordinates> getAttacks(Coordinates piece);
		bool attacks(Coordinates piece, Coordinates target);
		bool isInCheck(bool side);
		std::vector<Coordinates> getLegalMoves(uint8_t pieceRank, uint8_t pieceFile);
		bool getSide(Coordinates coords);

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

std::vector<Coordinates> Board::getAttacks(Coordinates piece) {
	std::vector<Coordinates> attacking;
	switch (this->board[piece.file][piece.rank]) {
		case EMPTY:
			break;
		case WPAWN:
			attacking.push_back({piece.file - 1, piece.rank + 1});
			attacking.push_back({piece.file + 1, piece.rank + 1});
			break;
		case BPAWN:
			attacking.push_back({piece.file - 1, piece.rank - 1});
			attacking.push_back({piece.file + 1, piece.rank - 1});
			break;
		case WKNIGHT:
		case BKNIGHT:
			attacking.push_back({piece.file - 1, piece.rank - 2});
			attacking.push_back({piece.file - 1, piece.rank + 2});
			attacking.push_back({piece.file + 1, piece.rank - 2});
			attacking.push_back({piece.file + 1, piece.rank + 2});
			attacking.push_back({piece.file - 2, piece.rank - 1});
			attacking.push_back({piece.file - 2, piece.rank + 1});
			attacking.push_back({piece.file + 2, piece.rank - 1});
			attacking.push_back({piece.file + 2, piece.rank + 1});
			break;
		case WBISHOP:
		case BBISHOP:
			{
				uint8_t i;
				for (i = 0; this->board[piece.file - i][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank - i});
				attacking.push_back({piece.file - i, piece.rank - i});
				for (i = 0; this->board[piece.file - i][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank - i});
				attacking.push_back({piece.file - i, piece.rank + i});
				for (i = 0; this->board[piece.file - i][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank - i});
				attacking.push_back({piece.file + i, piece.rank - i});
				for (i = 0; this->board[piece.file - i][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank - i});
				attacking.push_back({piece.file + i, piece.rank + i});
			}
			break;
		case WROOK:
		case BROOK:
			{
				uint8_t i;
				for (i = 0; this->board[piece.file - i][piece.rank] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank});
				attacking.push_back({piece.file - i, piece.rank});
				for (i = 0; this->board[piece.file + i][piece.rank] == EMPTY; i++)
					attacking.push_back({piece.file + i, piece.rank});
				attacking.push_back({piece.file + i, piece.rank});
				for (i = 0; this->board[piece.file][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file, piece.rank - i});
				attacking.push_back({piece.file, piece.rank - i});
				for (i = 0; this->board[piece.file][piece.rank + i] == EMPTY; i++)
					attacking.push_back({piece.file, piece.rank + i});
				attacking.push_back({piece.file, piece.rank + i});
			}
			break;
		case WQUEEN:
		case BQUEEN:
			{
				uint8_t i;
				for (i = 0; this->board[piece.file - i][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank - i});
				attacking.push_back({piece.file - i, piece.rank - i});
				for (i = 0; this->board[piece.file - i][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank - i});
				attacking.push_back({piece.file - i, piece.rank + i});
				for (i = 0; this->board[piece.file - i][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank - i});
				attacking.push_back({piece.file + i, piece.rank - i});
				for (i = 0; this->board[piece.file - i][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank - i});
				attacking.push_back({piece.file + i, piece.rank + i});

				for (i = 0; this->board[piece.file - i][piece.rank] == EMPTY; i++)
					attacking.push_back({piece.file - i, piece.rank});
				attacking.push_back({piece.file - i, piece.rank});
				for (i = 0; this->board[piece.file + i][piece.rank] == EMPTY; i++)
					attacking.push_back({piece.file + i, piece.rank});
				attacking.push_back({piece.file + i, piece.rank});
				for (i = 0; this->board[piece.file][piece.rank - i] == EMPTY; i++)
					attacking.push_back({piece.file, piece.rank - i});
				attacking.push_back({piece.file, piece.rank - i});
				for (i = 0; this->board[piece.file][piece.rank + i] == EMPTY; i++)
					attacking.push_back({piece.file, piece.rank + i});
				attacking.push_back({piece.file, piece.rank + i});
			}
			break;
		case WKING:
		case BKING:
			attacking.push_back({piece.file, piece.rank + 1});
			attacking.push_back({piece.file, piece.rank - 1});
			attacking.push_back({piece.file - 1, piece.rank + 1});
			attacking.push_back({piece.file - 1, piece.rank - 1});
			attacking.push_back({piece.file + 1, piece.rank + 1});
			attacking.push_back({piece.file + 1, piece.rank - 1});
			attacking.push_back({piece.file - 1, piece.rank});
			attacking.push_back({piece.file + 1, piece.rank});
	}
}

bool Board::attacks(Coordinates piece, Coordinates target) {
	std::vector<Coordinates> attacking = this->getAttacks(piece);
	for (int i = 0; i < attacking.size(); i++) {
		if (attacking[i] == target) {
			return true;
		}
	}
	return false;
}

bool Board::isInCheck(bool side) {

}

std::vector<Coordinates> Board::getLegalMoves(uint8_t pieceRank, uint8_t pieceFile) {
	std::vector<Coordinates> legalMoves = this->getAttacks({pieceRank, pieceFile});
	for (int i = 0; i < legalMoves.size(); i++) {
		if (this->getSide(legalMoves[i]) == this->getSide({pieceFile, pieceRank})) {
			legalMoves.erase(legalMoves.begin() + i);
			i--;
			continue;
		}
	}
}

bool Board::getSide(Coordinates coords) {
	return (this->board[coords.file][coords.rank] > WKING);
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