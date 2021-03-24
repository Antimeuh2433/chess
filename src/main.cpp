#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <iterator>
#include <fstream>

//  So I'm just writing everything wrong with this code here
// 1. The semantics are horrible
// 2. During checkmate/stalemate, Board::toPlay seems to be wrong.
// 3. Make sure the 75 (or 50) move rule checks for checkmates, stalemates, captures or pawn moves at the last move
// 4. Threefold repetition isn't implemented
// 5. Draw by insufficient material isn't implemented
// 6. Board::play assumes the move is legal, which may or may not be a good thing
// 7. The semantics are horrible
// 8. The semantics are horrible
// 9. The semantics are horrible

struct Coordinates {
	uint8_t file, rank;
	uint8_t promotion = 0;
	Coordinates(uint8_t nFile = 0, uint8_t nRank = 0, uint8_t nPromotion = 0) {
		this->file = nFile;
		this->rank = nRank;
		this->promotion = nPromotion;
	}
	bool operator==(Coordinates other) {
		return (this->file == other.file && this->rank == other.rank);
	}
}; // struct Coordinates

class Board {
	public:
		Board(bool recordMoves = false);
		void setStartingPosition();
		std::vector<Coordinates> getAttacks(Coordinates piece);
		bool attacks(Coordinates piece, Coordinates target);
		bool isInCheck(bool side);
		std::vector<Coordinates> getLegalMoves(Coordinates piece);
		bool getSide(Coordinates coords);
		void play(Coordinates piece, Coordinates target);
		bool playRandom(bool side);
		void print();
		void clearPosition();
		void setPiece(Coordinates coords, uint8_t piece);
		bool loadFromFEN(std::string fen);
		std::string exportFEN();
		void exportPGN(std::string outputFile);

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
		static constexpr uint8_t KINGSIDECASTLE = 13;
		static constexpr uint8_t QUEENSIDECASTLE = 14;
		static constexpr char *pieces = " PNBRQKpnbrqk";
	private:
		Coordinates whiteKingPosition;
		Coordinates blackKingPosition;
		uint8_t board[8][8];
		int8_t enPassantFlag;
		bool whiteKingSideCastle, whiteQueenSideCastle, blackKingSideCastle, blackQueenSideCastle;
		int moveCount;
		uint8_t pliesForDraw;
		bool toPlay;
		bool recordEnable;
		std::string pgn;
}; // class Board

Board::Board(bool recordMoves) {
	this->clearPosition();
	this->enPassantFlag = -1;
	this->whiteKingSideCastle = true;
	this->whiteQueenSideCastle = true;
	this->blackKingSideCastle = true;
	this->blackQueenSideCastle = true;
	this->moveCount = 0;
	this->pliesForDraw = 0;
	this->toPlay = WHITE;
	this->recordEnable = recordMoves;
	if (recordMoves) this->pgn = "[Event \"Casual Game\"]\n[Site \"Earth\"]\n[Date \"????.??.??\"]\n[Round \"1\"]\n[White \"Bax's Horrible Engine\"]\n[Black \"Bax's Horrible Engine\"]\n[Result \"*\"]\n";
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
			for (int i = 1; i <= std::min(piece.file, piece.rank); i++) {
				attacking.push_back({piece.file - i, piece.rank - i});
				if (this->board[piece.file - i][piece.rank - i] != EMPTY) break;
			}
			for (int i = 1; i <= std::min(uint8_t(7 - piece.file), piece.rank); i++) {
				attacking.push_back({piece.file + i, piece.rank - i});
				if (this->board[piece.file + i][piece.rank - i] != EMPTY) break;
			}
			for (int i = 1; i <= std::min(piece.file, uint8_t(7 - piece.rank)); i++) {
				attacking.push_back({piece.file - i, piece.rank + i});
				if (this->board[piece.file - i][piece.rank + i] != EMPTY) break;
			}
			for (int i = 1; i <= std::min(uint8_t(7 - piece.file), uint8_t(7 - piece.rank)); i++) {
				attacking.push_back({piece.file + i, piece.rank + i});
				if (this->board[piece.file + i][piece.rank + i] != EMPTY) break;
			}
			break;
		case WROOK:
		case BROOK:
			for (int i = 1; i <= piece.file; i++) {
				attacking.push_back({piece.file - i, piece.rank});
				if (this->board[piece.file - i][piece.rank] != EMPTY) break;
			}
			for (int i = 1; i <= 7 - piece.file; i++) {
				attacking.push_back({piece.file + i, piece.rank});
				if (this->board[piece.file + i][piece.rank] != EMPTY) break;
			}
			for (int i = 1; i <= piece.rank; i++) {
				attacking.push_back({piece.file, piece.rank - i});
				if (this->board[piece.file][piece.rank - i] != EMPTY) break;
			}
			for (int i = 1; i <= 7 - piece.rank; i++) {
				attacking.push_back({piece.file, piece.rank + i});
				if (this->board[piece.file][piece.rank + i] != EMPTY) break;
			}
			break;
		case WQUEEN:
		case BQUEEN:
			for (int i = 1; i <= std::min(piece.file, piece.rank); i++) {
				attacking.push_back({piece.file - i, piece.rank - i});
				if (this->board[piece.file - i][piece.rank - i] != EMPTY) break;
			}
			for (int i = 1; i <= std::min(uint8_t(7 - piece.file), piece.rank); i++) {
				attacking.push_back({piece.file + i, piece.rank - i});
				if (this->board[piece.file + i][piece.rank - i] != EMPTY) break;
			}
			for (int i = 1; i <= std::min(piece.file, uint8_t(7 - piece.rank)); i++) {
				attacking.push_back({piece.file - i, piece.rank + i});
				if (this->board[piece.file - i][piece.rank + i] != EMPTY) break;
			}
			for (int i = 1; i <= std::min(uint8_t(7 - piece.file), uint8_t(7 - piece.rank)); i++) {
				attacking.push_back({piece.file + i, piece.rank + i});
				if (this->board[piece.file + i][piece.rank + i] != EMPTY) break;
			}

			for (int i = 1; i <= piece.file; i++) {
				attacking.push_back({piece.file - i, piece.rank});
				if (this->board[piece.file - i][piece.rank] != EMPTY) break;
			}
			for (int i = 1; i <= 7 - piece.file; i++) {
				attacking.push_back({piece.file + i, piece.rank});
				if (this->board[piece.file + i][piece.rank] != EMPTY) break;
			}
			for (int i = 1; i <= piece.rank; i++) {
				attacking.push_back({piece.file, piece.rank - i});
				if (this->board[piece.file][piece.rank - i] != EMPTY) break;
			}
			for (int i = 1; i <= 7 - piece.rank; i++) {
				attacking.push_back({piece.file, piece.rank + i});
				if (this->board[piece.file][piece.rank + i] != EMPTY) break;
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
		if (attacking[i].file > 7 || attacking[i].rank > 7) {
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
			if (this->board[i][j] != EMPTY && this->getSide({i, j}) != side) {
				if (side == WHITE) {
					if (this->attacks({i, j}, this->whiteKingPosition)) {
						return true;
					}
				} else {
					if (this->attacks({i, j}, this->blackKingPosition)) {
						return true;
					}
				}
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
	bool side = this->getSide(piece);
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
			if (piece.rank == 1) {
				if (this->board[piece.file][piece.rank + 2] == EMPTY) {
					legalMoves.push_back({piece.file, piece.rank + 2});
				}
			}
		}
		for (int i = 0; i < legalMoves.size(); i++) {
			if (legalMoves[i].rank == 7 && legalMoves[i].promotion == 0) {
				legalMoves.push_back({legalMoves[i].file, legalMoves[i].rank, WKNIGHT});
				legalMoves.push_back({legalMoves[i].file, legalMoves[i].rank, WBISHOP});
				legalMoves.push_back({legalMoves[i].file, legalMoves[i].rank, WROOK});
				legalMoves.push_back({legalMoves[i].file, legalMoves[i].rank, WQUEEN});
				legalMoves.erase(legalMoves.begin() + i);
				i--;
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
			legalMoves.push_back({piece.file, piece.rank - 1});
			if (piece.rank == 6) {
				if (this->board[piece.file][piece.rank - 2] == EMPTY) {
					legalMoves.push_back({piece.file, piece.rank - 2});
				}
			}
		}
		for (int i = 0; i < legalMoves.size(); i++) {
			if (legalMoves[i].rank == 0 && legalMoves[i].promotion == 0) {
				legalMoves.push_back({legalMoves[i].file, legalMoves[i].rank, BKNIGHT});
				legalMoves.push_back({legalMoves[i].file, legalMoves[i].rank, BBISHOP});
				legalMoves.push_back({legalMoves[i].file, legalMoves[i].rank, BROOK});
				legalMoves.push_back({legalMoves[i].file, legalMoves[i].rank, BQUEEN});
				legalMoves.erase(legalMoves.begin() + i);
				i--;
			}
		}
	} else if (currentStatus == WKING || currentStatus == BKING) {
		if (side == WHITE ? this->whiteKingSideCastle : this->blackKingSideCastle) {
			if (!this->isInCheck(side) && this->board[piece.file + 1][piece.rank] == EMPTY) {
				(side == WHITE ? this->whiteKingPosition : this->blackKingPosition) = {piece.file + 1, piece.rank};
				if (!this->isInCheck(side) && this->board[piece.file + 2][piece.rank] == EMPTY) {
					(side == WHITE ? this->whiteKingPosition : this->blackKingPosition) = {piece.file + 2, piece.rank};
					if (!this->isInCheck(side)) {
						legalMoves.push_back({piece.file + 2, piece.rank, KINGSIDECASTLE});
					}
				}
			}
		}
		if (side == WHITE ? this->whiteQueenSideCastle : this->blackQueenSideCastle) {
			if (!this->isInCheck(side) && this->board[piece.file - 1][piece.rank] == EMPTY) {
				(side == WHITE ? this->whiteKingPosition : this->blackKingPosition) = {piece.file - 1, piece.rank};
				if (!this->isInCheck(side) && this->board[piece.file - 2][piece.rank] == EMPTY) {
					(side == WHITE ? this->whiteKingPosition : this->blackKingPosition) = {piece.file - 2, piece.rank};
					if (!this->isInCheck(side) && this->board[piece.file - 3][piece.rank] == EMPTY) {
						legalMoves.push_back({piece.file - 2, piece.rank, QUEENSIDECASTLE});
					}
				}
			}
		}
		(side == WHITE ? this->whiteKingPosition : this->blackKingPosition) = piece;
	}
	this->board[piece.file][piece.rank] = EMPTY;
	for (int i = 0; i < legalMoves.size(); i++) {
		destinationStatus = this->board[legalMoves[i].file][legalMoves[i].rank];
		if (destinationStatus != EMPTY && this->getSide(legalMoves[i]) == side) {
			legalMoves.erase(legalMoves.begin() + i);
			i--;
			continue;
		}
		this->board[legalMoves[i].file][legalMoves[i].rank] = currentStatus;
		if (legalMoves[i].promotion > 0 && legalMoves[i].promotion < KINGSIDECASTLE) {
			this->board[legalMoves[i].file][legalMoves[i].rank] = legalMoves[i].promotion;
		}
		if (i == enPassant) {
			if (side == WHITE) {
				this->board[legalMoves[i].file][legalMoves[i].rank - 1] = EMPTY;
			} else {
				this->board[legalMoves[i].file][legalMoves[i].rank + 1] = EMPTY;
			}
		}
		if (currentStatus == WKING) {
			this->whiteKingPosition = legalMoves[i];
		} else if (currentStatus == BKING) {
			this->blackKingPosition = legalMoves[i];
		}
		if (this->isInCheck(side)) {
			if (i == enPassant) {
				if (side == WHITE) {
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
		if (i == enPassant) {
			if (side == WHITE) {
				this->board[legalMoves[i].file][legalMoves[i].rank - 1] = BPAWN;
			} else {
				this->board[legalMoves[i].file][legalMoves[i].rank + 1] = WPAWN;
			}
		}
		this->board[legalMoves[i].file][legalMoves[i].rank] = destinationStatus;
	}
	this->board[piece.file][piece.rank] = currentStatus;
	if (currentStatus == WKING) {
		this->whiteKingPosition = piece;
	} else if (currentStatus == BKING) {
		this->blackKingPosition = piece;
	}
	return legalMoves;
}

bool Board::getSide(Coordinates coords) {
	return (this->board[coords.file][coords.rank] > WKING);
}

void Board::play(Coordinates piece, Coordinates target) {
	if (this->recordEnable && this->moveCount % 2 == 0) {
		this->pgn += std::to_string(this->moveCount / 2 + 1);
		this->pgn += ". ";
	}
	bool isEnPassant = false;
	if (target.file == this->enPassantFlag) {
		if (this->getSide(piece) == WHITE && target.rank == 5) {
			this->board[target.file][target.rank - 1] = EMPTY;
			isEnPassant = true;
		} else if (this->getSide(piece) == BLACK && target.rank == 2) {
			this->board[target.file][target.rank + 1] = EMPTY;
			isEnPassant = true;
		}
	}
	if (this->board[piece.file][piece.rank] == WKING) {
		this->whiteKingPosition = target;
		this->whiteKingSideCastle = false;
		this->whiteQueenSideCastle = false;
	} else if (this->board[piece.file][piece.rank] == BKING) {
		this->blackKingPosition = target;
		this->blackKingSideCastle = false;
		this->blackQueenSideCastle = false;
	} else if (this->board[piece.file][piece.rank] == WROOK) {
		if (piece.file == 0) this->whiteQueenSideCastle = false;
		else if (piece.file == 7) this->whiteKingSideCastle = false;
	} else if (this->board[piece.file][piece.rank] == BROOK) {
		if (piece.file == 0) this->blackQueenSideCastle = false;
		else if (piece.file == 7) this->blackKingSideCastle = false;
	}
	if (target.promotion == KINGSIDECASTLE) {
		this->board[target.file][target.rank] = this->board[piece.file][piece.rank];
		this->board[target.file - 1][target.rank] = this->board[7][target.rank];
		this->board[7][target.rank] = EMPTY;
		this->pliesForDraw++;
		this->pgn += "O-O ";
	} else if (target.promotion == QUEENSIDECASTLE) {
		this->board[target.file][target.rank] = this->board[piece.file][piece.rank];
		this->board[target.file + 1][target.rank] = this->board[0][target.rank];
		this->board[0][target.rank] = EMPTY;
		this->pliesForDraw++;
		this->pgn += "O-O-O ";
	} else if (target.promotion != 0) {
		if (this->recordEnable) {
			this->pgn += (char)(piece.file + 'a');
			if (piece.file != target.file) {
				this->pgn += 'x';
				this->pgn += (char)(target.file + 'a');
			}
			this->pgn += std::to_string(target.rank + 1);
			this->pgn += '=';
			this->pgn += this->pieces[target.promotion % 6];
		}
		this->board[target.file][target.rank] = target.promotion;
		this->pliesForDraw = 0;
	} else {
		if (this->recordEnable) {
			if (this->board[piece.file][piece.rank] == WPAWN || this->board[piece.file][piece.rank] == BPAWN) {
				this->pgn += (char)(piece.file + 'a');
				if (piece.file != target.file) {
					this->pgn += 'x';
					this->pgn += (char)(target.file + 'a');
				}
			} else {
				this->pgn += this->pieces[this->board[piece.file][piece.rank] % 6];
				if (this->board[target.file][target.rank] != EMPTY) this->pgn += 'x';
				this->pgn += (char)(target.file + 'a');
				// check if any other piece with the same name attacks the same square
			}
			this->pgn += std::to_string(target.rank + 1);
		}
		if (this->board[piece.file][piece.rank] == WPAWN || this->board[piece.file][piece.rank] == BPAWN || this->board[target.file][target.rank] != EMPTY) {
			this->pliesForDraw = 0;
		} else {
			this->pliesForDraw++;
		}
		this->board[target.file][target.rank] = this->board[piece.file][piece.rank];
	}
	if (this->recordEnable) this->pgn += ' ';

	this->board[piece.file][piece.rank] = EMPTY;
	this->enPassantFlag = -1;
	if (this->board[target.file][target.rank] == WPAWN || this->board[target.file][target.rank] == BPAWN) {
		if (abs(target.rank - piece.rank) == 2) {
			this->enPassantFlag = target.file;
		}
	}
	this->moveCount++;
	this->toPlay = !this->toPlay;
}

bool Board::playRandom(bool side) {
	std::vector<std::pair<Coordinates, Coordinates>> allMoves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->board[i][j] != EMPTY && this->getSide({i, j}) == side) {
				std::vector<Coordinates> legalMoves = this->getLegalMoves({i, j});
				for (int k = 0; k < legalMoves.size(); k++) {
					allMoves.push_back(std::make_pair(Coordinates(i, j), legalMoves[k]));
				}
			}
		}
	}
	if (allMoves.size() == 0) {
		if (this->isInCheck(side)) {
			std::cout << "Checkmate. " << (side == WHITE ? "White " : "Black ") << "wins." << std::endl;
			return false;
		} else {
			std::cout << "Stalemate. Draw." << std::endl;
			return false;
		}
	}
	if (this->pliesForDraw == 150) {
		std::cout << "75 moves since last pawn move or capture. Draw." << std::endl;
		return false;
	}
	int move = rand() % allMoves.size();
	this->play(allMoves[move].first, allMoves[move].second);
	return true;
}

void Board::clearPosition() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			this->board[i][j] = EMPTY;
		}
	}
}

void Board::print() {
	std::cout << "Turn " << (this->moveCount / 2) + 1 << " (" << (this->toPlay == WHITE ? "white" : "black") << " to move); " << (int)this->pliesForDraw << " plies since last capture or pawn move." << std::endl;
	std::cout << "a b c d e f g h" << std::endl << std::endl;
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			std::cout << this->pieces[this->board[j][i]] << " ";
		}
		std::cout << " " << i + 1 << std::endl;
	}
	std::cout << "------------------" << std::endl;
}

void Board::setPiece(Coordinates coords, uint8_t piece) {
	this->board[coords.file][coords.rank] = piece;
}

bool Board::loadFromFEN(std::string fen) {
	std::istringstream iss(fen);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (results.size() != 6) return false;
	uint8_t rank = 7;
	uint8_t file = 0;
	for (int i = 0; i < results[0].size(); i++) {
		switch (results[0][i]) {
			case 'P':
				this->board[file][rank] = WPAWN;
				file++;
				break;
			case 'N':
				this->board[file][rank] = WKNIGHT;
				file++;
				break;
			case 'B':
				this->board[file][rank] = WBISHOP;
				file++;
				break;
			case 'R':
				this->board[file][rank] = WROOK;
				file++;
				break;
			case 'Q':
				this->board[file][rank] = WQUEEN;
				file++;
				break;
			case 'K':
				this->board[file][rank] = WKING;
				this->whiteKingPosition = {file, rank};
				file++;
				break;
			case 'p':
				this->board[file][rank] = BPAWN;
				file++;
				break;
			case 'n':
				this->board[file][rank] = BKNIGHT;
				file++;
				break;
			case 'b':
				this->board[file][rank] = BBISHOP;
				file++;
				break;
			case 'r':
				this->board[file][rank] = BROOK;
				file++;
				break;
			case 'q':
				this->board[file][rank] = BQUEEN;
				file++;
				break;
			case 'k':
				this->board[file][rank] = BKING;
				this->blackKingPosition = {file, rank};
				file++;
				break;
			case '/':
				file = 0;
				rank--;
				break;
			default:
				uint8_t spaces = results[0][i] - '0';
				if (file + spaces > 8) {
					return false;
				}
				file += (int)spaces;
				break;
		}
	}
	if (results[1][0] == 'b') {
		this->toPlay = BLACK;
	} else {
		this->toPlay = WHITE;
	}
	this->whiteKingSideCastle = false;
	this->whiteQueenSideCastle = false;
	this->blackKingSideCastle = false;
	this->blackQueenSideCastle = false;
	for (int i = 0; i < results[2].size(); i++) {
		switch (results[2][i]) {
			case '-':
				break;
			case 'K':
				this->whiteKingSideCastle = true;
				break;
			case 'Q':
				this->whiteQueenSideCastle = true;
				break;
			case 'k':
				this->blackKingSideCastle = true;
				break;
			case 'q':
				this->blackQueenSideCastle = true;
			default:
				return false;
		}
	}
	if (results[3][0] != '-') {
		this->enPassantFlag = results[3][0] - 'a';
	}
	std::istringstream(results[4]) >> this->pliesForDraw;
	std::istringstream(results[5]) >> this->moveCount;
	this->moveCount *= 2;
	return true;
}

std::string Board::exportFEN() {
	std::string fen = "";
	for (int i = 7; i >= 0; i--) {
		int emptySquareCount = 0;
		for (int j = 0; j < 8; j++) {
			if (this->board[j][i] == EMPTY) {
				emptySquareCount++;
			} else {
				if (emptySquareCount > 0) {
					fen += std::to_string(emptySquareCount);
					emptySquareCount = 0;
				}
				fen += this->pieces[this->board[j][i]];
			}
		}
		if (emptySquareCount > 0) {
			fen += std::to_string(emptySquareCount);
		}
		fen += '/';
	}
	fen.pop_back();
	fen += (this->toPlay == WHITE ? " w " : " b ");
	if (this->whiteKingSideCastle) fen += 'K';
	if (this->whiteQueenSideCastle) fen += 'Q';
	if (this->blackKingSideCastle) fen += 'k';
	if (this->blackQueenSideCastle) fen += 'q';
	if (fen.back() == ' ') fen += '-';
	
	fen += ' ';
	if (this->enPassantFlag != -1) {
		fen += (this->enPassantFlag + 'a');
		fen += (this->toPlay == WHITE ? "6 " : "3 ");
	} else {
		fen += "- ";
	}
	fen += std::to_string((int)pliesForDraw);
	fen += ' ';
	fen += std::to_string(moveCount / 2 + 1);
	return fen;
}

void Board::exportPGN(std::string outputFile) {
	if (!this->recordEnable) return;
	std::ofstream out(outputFile + ".pgn");
	out << this->pgn << std::endl;
	out.close();
}

int main() {
	srand(time(NULL));
	Board board(true);
	board.setStartingPosition();
	bool toPlay = Board::WHITE;
	while (true) {
		if (!board.playRandom(toPlay)) break;
		toPlay = !toPlay;
	}
	board.print();
	board.exportPGN("game");
	std::cout << board.exportFEN() << std::endl;
	return 0;
}