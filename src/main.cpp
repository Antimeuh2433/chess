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
		std::vector<Coordinates> getLegalMoves(Coordinates piece);
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
		this->board[i][1] = WPAWN;
		this->board[i][6] = BPAWN;
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
	for (int i = 0; i < attacking.size(); i++) {
		if (attacking[i].file < 0 || attacking[i].file > 7 || attacking[i].rank < 0 || attacking[i].rank > 7) {
			attacking.erase(attacking.begin() + i);
			i--;
		}
	}
	return attacking;
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
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->board[i][j] == WKING || this->board[i][j] == BKING) {
				if (this->getSide({i, j}) == side) {
					for (int k = 0; k < 8; k++) {
						for (int l = 0; l < 8; l++) {
							if (this->board[k][l] != EMPTY && this->getSide({k, l}) != side) {
								if (this->attacks({k, l}, {i, j})) {
									return true;
								}
							}
						}
					}
					return false;
				}
			}
		}
	}
	return false;
}

std::vector<Coordinates> Board::getLegalMoves(Coordinates piece) {
	// TODO implement en passant
	std::vector<Coordinates> legalMoves = this->getAttacks(piece);
	uint8_t currentStatus = this->board[piece.file][piece.rank], destinationStatus;
	if (currentStatus == WPAWN || currentStatus == BPAWN) {
		for (int i = 0; i < legalMoves.size(); i++) {
			if (this->board[legalMoves[i].file][legalMoves[i].rank] == EMPTY || this->getSide(legalMoves[i]) == this->getSide(piece)) {
				legalMoves.erase(legalMoves.begin() + i);
				i--;
			}
		}
		if (getSide(piece) == WHITE) {
			if (this->board[piece.file][piece.rank + 1] == EMPTY) {
				legalMoves.push_back({piece.file, piece.rank + 1});
				if (piece.rank < 6) {
					if (this->board[piece.file][piece.rank + 2] == EMPTY) {
						legalMoves.push_back({piece.file, piece.rank + 2});
					}
				}
			}
		} else {
			if (this->board[piece.file][piece.rank - 1] == EMPTY) {
				legalMoves.push_back({piece.file, piece.rank + 1});
				if (piece.rank > 1) {
					if (this->board[piece.file][piece.rank - 2] == EMPTY) {
						legalMoves.push_back({piece.file, piece.rank + 2});
					}
				}
			}
		}
	}
	this->board[piece.file][piece.rank] = EMPTY;
	for (int i = 0; i < legalMoves.size(); i++) {
		destinationStatus = this->board[legalMoves[i].file][legalMoves[i].rank];
		if (this->board[legalMoves[i].file][legalMoves[i].rank] != EMPTY && this->getSide(legalMoves[i]) == this->getSide(piece)) {
			legalMoves.erase(legalMoves.begin() + i);
			i--;
			continue;
		}
		this->board[legalMoves[i].file][legalMoves[i].rank] = currentStatus;
		if (this->isInCheck(this->getSide(piece))) {
			this->board[legalMoves[i].file][legalMoves[i].rank] = destinationStatus;
			legalMoves.erase(legalMoves.begin() + i);
			i--;
			continue;
		}
		this->board[legalMoves[i].file][legalMoves[i].rank] = destinationStatus;
	}
	return legalMoves;
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
	Board board;
	board.setStartingPosition();
	std::cout << board.getLegalMoves({0, 1}).size() << std::endl;
	return 0;
}