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
		void play(Coordinates piece, Coordinates target);
		void playRandom(bool side);
		bool capture(Coordinates pieceCapturing, Coordinates pieceCaptured, bool side, bool enPassant = false);
		void print();
		void clearPosition();

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
		Coordinates whiteKingPosition;
		Coordinates blackKingPosition;
		std::vector<Coordinates> blackPieces;
		std::vector<Coordinates> whitePieces;
		uint8_t board[8][8];
		int8_t enPassantFlag;
}; // class Board

Board::Board() {
	this->clearPosition();
	this->enPassantFlag = -1;
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
	this->whiteKingPosition = {4, 0};

	this->board[0][7] = BROOK;
	this->board[1][7] = BKNIGHT;
	this->board[2][7] = BBISHOP;
	this->board[3][7] = BQUEEN;
	this->board[4][7] = BKING;
	this->board[5][7] = BBISHOP;
	this->board[6][7] = BKNIGHT;
	this->board[7][7] = BROOK;
	this->blackKingPosition = {4, 7};

	for (int i = 0; i < 8; i++) {
		this->board[i][1] = WPAWN;
		this->board[i][6] = BPAWN;
		this->whitePieces.push_back({i, 0});
		this->whitePieces.push_back({i, 1});
		this->blackPieces.push_back({i, 7});
		this->blackPieces.push_back({i, 6});
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
			// TODO rework bishop, rook and queen movement
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
	if (side == WHITE) {
		for (int i = 0; i < this->blackPieces.size(); i++) {
			if (this->attacks(this->blackPieces[i], this->whiteKingPosition)) {
				return true;
			}
		}
	} else {
		for (int i = 0; i < this->whitePieces.size(); i++) {
			if (this->attacks(this->whitePieces[i], this->blackKingPosition)) {
				return true;
			}
		}
	}
	return false;
}

std::vector<Coordinates> Board::getLegalMoves(Coordinates piece) {
	std::vector<Coordinates> legalMoves = this->getAttacks(piece);
	uint8_t currentStatus = this->board[piece.file][piece.rank];
	uint8_t destinationStatus;
	int8_t enPassant = -1;
	if (currentStatus == WPAWN) {
		for (int i = 0; i < legalMoves.size(); i++) {
			if (this->board[legalMoves[i].file][legalMoves[i].rank] == EMPTY) {
				if (legalMoves[i].file != this->enPassantFlag || legalMoves[i].rank != 5) {
					legalMoves.erase(legalMoves.begin() + i);
					i--;
				} else {
					enPassant = i;
				}
			} else {
				if (this->getSide(legalMoves[i]) == WHITE) {
					legalMoves.erase(legalMoves.begin() + i);
					i--;
				}
			}
		}
		if (this->board[piece.file][piece.rank + 1] == EMPTY) {
			legalMoves.push_back({piece.file, piece.rank + 1});
			if (piece.rank < 6) {
				if (this->board[piece.file][piece.rank + 2] == EMPTY) {
					legalMoves.push_back({piece.file, piece.rank + 2});
				}
			}
		}
	} else if (currentStatus == BPAWN) {
		for (int i = 0; i < legalMoves.size(); i++) {
			if (this->board[legalMoves[i].file][legalMoves[i].rank] == EMPTY) {
				if (legalMoves[i].file != this->enPassantFlag || legalMoves[i].rank != 2) {
					legalMoves.erase(legalMoves.begin() + i);
					i--;
				} else {
					enPassant = i;
				}
			} else {
				if (this->getSide(legalMoves[i]) == BLACK) {
					legalMoves.erase(legalMoves.begin() + i);
					i--;
				}
			}
		}
		if (this->board[piece.file][piece.rank - 1] == EMPTY) {
			legalMoves.push_back({piece.file, piece.rank + 1});
			if (piece.rank > 1) {
				if (this->board[piece.file][piece.rank - 2] == EMPTY) {
					legalMoves.push_back({piece.file, piece.rank + 2});
				}
			}
		}
	}
	this->board[piece.file][piece.rank] = EMPTY;
	// FIX code is generally really bad pretty much everywhere
	// FIX if piece is king, update this->*KingPosition
	if (this->getSide(piece) == WHITE) {
		for (int i = 0; i < this->whitePieces.size(); i++) {
			if (this->whitePieces[i] == piece) {
				this->whitePieces.erase(this->whitePieces.begin() + i);
				break;
			}
		}
		if (currentStatus == WKING) {
			this->whiteKingPosition = piece;
		}
	} else {
		for (int i = 0; i < this->blackPieces.size(); i++) {
			if (this->blackPieces[i] == piece) {
				this->blackPieces.erase(this->blackPieces.begin() + i);
				break;
			}
		}
		if (currentStatus == BKING) {
			this->blackKingPosition = piece;
		}
	}
	for (int i = 0; i < legalMoves.size(); i++) {
		// FIX if destinationStatus is king, what happens
		destinationStatus = this->board[legalMoves[i].file][legalMoves[i].rank];
		if (destinationStatus != EMPTY && this->getSide(legalMoves[i]) == this->getSide(piece)) {
			legalMoves.erase(legalMoves.begin() + i);
			i--;
			continue;
		}
		if (this->board[legalMoves[i].file][legalMoves[i].rank] != EMPTY) {
			if (this->getSide(piece) == WHITE) {
				for (int i = 0; i < this->blackPieces.size(); i++) {
					if (this->blackPieces[i] == legalMoves[i]) {
						this->blackPieces.erase(this->blackPieces.begin() + i);
						break;
					}
				}
			} else {
				for (int i = 0; i < this->whitePieces.size(); i++) {
					if (this->whitePieces[i] == legalMoves[i]) {
						this->whitePieces.erase(this->whitePieces.begin() + i);
						break;
					}
				}
			}
		}
		this->board[legalMoves[i].file][legalMoves[i].rank] = currentStatus;
		if (i == enPassant) {
			if (this->getSide(piece) == WHITE) {
				this->board[legalMoves[i].file][legalMoves[i].rank - 1] = EMPTY;
			} else {
				this->board[legalMoves[i].file][legalMoves[i].rank + 1] = EMPTY;
			}
		}
		// FIX update this->*Pieces and this->*KingPosition before this->isInCheck call
		if (this->isInCheck(this->getSide(piece))) {
			if (enPassant != -1) {
				if (this->getSide(piece) == WHITE) {
					this->board[legalMoves[i].file][legalMoves[i].rank - 1] = BPAWN;
				} else {
					this->board[legalMoves[i].file][legalMoves[i].rank + 1] = WPAWN;
				}
			}
			this->board[legalMoves[i].file][legalMoves[i].rank] = destinationStatus;
			legalMoves.erase(legalMoves.begin() + i);
			i--;
			continue;
		}
		if (enPassant != -1) {
			if (this->getSide(piece) == WHITE) {
				this->board[legalMoves[i].file][legalMoves[i].rank - 1] = BPAWN;
			} else {
				this->board[legalMoves[i].file][legalMoves[i].rank + 1] = WPAWN;
			}
		}
		this->board[legalMoves[i].file][legalMoves[i].rank] = destinationStatus;
	}
	this->board[piece.file][piece.rank] = currentStatus;
	return legalMoves;
}

bool Board::getSide(Coordinates coords) {
	return (this->board[coords.file][coords.rank] > WKING);
}

void Board::play(Coordinates piece, Coordinates target) {
	if (target.file == enPassantFlag) {
		if (this->getSide(piece) == WHITE && target.rank == 5) {
			this->board[target.file][target.rank - 1] == EMPTY;
		} else if (this->getSide(piece) == BLACK && target.rank == 2) {
			this->board[target.file][target.rank + 1] == EMPTY;
		}
	}
	this->board[target.file][target.rank] = this->board[piece.file][piece.rank];
	this->board[piece.file][piece.rank] = EMPTY;
	this->enPassantFlag = -1;
	if (this->board[target.file][target.rank] == WPAWN || this->board[target.file][target.rank] == BPAWN) {
		if (abs(target.rank - piece.rank) == 2) {
			this->enPassantFlag = target.file;
		}
	}
}

void Board::playRandom(bool side) {

}

bool Board::capture(Coordinates pieceCapturing, Coordinates pieceCaptured, bool sideCapturing, bool enPassant) {
	this->board[pieceCaptured.file][pieceCaptured.rank] = this->board[pieceCapturing.file][pieceCapturing.rank];
	this->board[pieceCapturing.file][pieceCapturing.rank] = EMPTY;
	if (sideCapturing == WHITE) {
		for (int i = 0; i < this->whitePieces.size(); i++) {
			if (this->whitePieces[i] == pieceCapturing) {
				this->whitePieces[i] = pieceCaptured;
				break;
			}
		}
		if (enPassant) pieceCaptured.rank--;
		for (int i = 0; i < this->blackPieces.size(); i++) {
			if (this->blackPieces[i] == pieceCaptured) {
				this->blackPieces.erase(this->blackPieces.begin() + i);
				break;
			}
		}
		this->board[pieceCaptured.file][pieceCaptured.rank] = EMPTY;
	} else {
		for (int i = 0; i < this->blackPieces.size(); i++) {
			if (this->blackPieces[i] == pieceCapturing) {
				this->blackPieces[i] = pieceCaptured;
				break;
			}
		}
		if (enPassant) pieceCaptured.rank++;
		for (int i = 0; i < this->whitePieces.size(); i++) {
			if (this->whitePieces[i] == pieceCaptured) {
				this->whitePieces.erase(this->whitePieces.begin() + i);
				break;
			}
		}
		this->board[pieceCaptured.file][pieceCaptured.rank] = EMPTY;
	}
}

void Board::clearPosition() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			this->board[i][j] = EMPTY;
		}
	}
}

void Board::print() {
	char pieces[] = " PNBRQKpnbrqk";
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			std::cout << pieces[this->board[j][i]] << " ";
		}
		std::cout << std::endl;
	}
}

int main() {
	Board board;
	board.setStartingPosition();
	board.print();
	board.getLegalMoves({1, 1});
	std::cout << std::endl;
	board.play({1, 1}, {1, 3});
	board.print();
	return 0;
}