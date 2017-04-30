#include "ScoreBoard.h"
namespace B00289996 {
	ScoreBoard & ScoreBoard::GetInstance() {
		static ScoreBoard instance;
		return instance;
	}

	ScoreBoard::ScoreBoard() {
	}

	ScoreBoard::~ScoreBoard() {
	}

	void ScoreBoard::AddCoin() {
		coinsCollected++;
	}

	void ScoreBoard::Reset() {
		coinsCollected = 0;
	}

	const unsigned int ScoreBoard::GetCoins() const {
		return coinsCollected;
	}
}