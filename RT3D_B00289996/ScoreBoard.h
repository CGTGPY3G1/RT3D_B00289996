#pragma once
#ifndef B00289996_SCOREBOARD_H
#define B00289996_SCOREBOARD_H
namespace B00289996 {
	/// <summary>
	/// Counts collected coins
	/// </summary>
	class ScoreBoard {
	public:
		/// <summary>
		/// Gets the singleton instance.
		/// </summary>
		/// <returns>the singleton instance</returns>
		static ScoreBoard & GetInstance();
		/// <summary>
		/// Finalizes an instance of the <see cref="ScoreBoard"/> class.
		/// </summary>
		~ScoreBoard();
		/// <summary>
		/// Adds 1 to the coin count.
		/// </summary>
		void AddCoin();
		/// <summary>
		/// Resets the coin count.
		/// </summary>
		void Reset();
		/// <summary>
		/// Gets the number of collected coins.
		/// </summary>
		/// <returns></returns>
		const unsigned int GetCoins() const;
	private:
		/// <summary>
		/// Prevents a default instance of the <see cref="ScoreBoard"/> class from being created.
		/// </summary>
		ScoreBoard();
		unsigned int coinsCollected = 0;
	};
}
#endif // !B00289996_SCOREBOARD_H
