#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		20/08/2019 - Lenningen - Luxembourg
*
* Desc:		class to define the Arkanoid game world
*
* Hist:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// C++ includes
#include <set>
#include <array>

// windows includes
#include <wrl.h>

// bell0bytes util
#include "expected.h"
#include "depesche.h"

// DEFINITIONS //////////////////////////////////////////////////////////////////////////
namespace boost
{
	namespace serialization
	{
		class access;
	}
}

namespace core
{
	class DepescheSender;
	class DirectXApp;
}

namespace audio
{
	struct SoundEvent;
	struct StreamEvent;
}

namespace game
{
	struct Score;
	class HighscoreTable;

	// different block types
	// - normal: just a normal block that disappears when hit once
	// - study: sturdy blocks have to be hit multiple times
	// - benevolent: benevolent blocks leave power ups when destroyed
	// - malevolent: malevolent blocks leave power downs when destroyed
	// - anomaly: these blocks leave a physical anomaly when destroyed - changing the physical laws of the environment
	enum BlockTypes {Normal, Sturdy, Benevolent, Malevolent, Anomaly};
	
	// block colours
	enum BlockColours {Gray, Brown, LightBrown, Green, Red, Yellow};

	// power ups, power downs and physical anomalies
	enum PowerUps {NoPU, BiggerShip, FasterShip, SlowerBall};
	enum PowerDowns {NoPD, SmallerShop, SlowerShip, FasterBall};
	enum PhysicalAnomalies {NoPA, AngleOfIncidence, InvertedAxis};

	class Block
	{
	public:
		BlockTypes type;			// the type of the block
		BlockColours colour;		// the colour of the block
		PowerDowns pd;				// power down
		PowerUps pu;				// power up
		PhysicalAnomalies pa;		// physical anomaly

		Block();
	};

	class GameBoard : public core::DepescheSender
	{
	private:
		core::DirectXApp& dxApp;

		// brushes for different coloured blocks

		// constant definitions to specify the tetris bucket
		static const unsigned int N = 10;					// the number of blocks per row
		static const unsigned int M = 16;					// the number of blocks per column
		const unsigned int lineWidth = 2;					// the width of the border line
		const unsigned int lineHeight = 2;					// the height of the border line
		const unsigned int bucketWidth = 600;				// the width of the game world
		const unsigned int bucketHeight = 960;				// the height of the game world
		const unsigned int blockWidth = 60;					// the width of each block
		const unsigned int blockHeight = 60;				// the height of each block
		const unsigned int hudWidth = 600;					// the width of each hud
		const unsigned int hudHeight = 1080;				// the height of each hud
		const unsigned int heightOffset = 60;				// height offset

		// highscore table
		//HighscoreTable* highscoreTable;					// stores the high scores
		//Score* currentScore;								// keeps track of the score of the current player
		
		// game features

		// enum to define whether a position is free or already filled
		enum PositionStatus { PositionFree, PositionFilled };	// PositionFree: no tetronimo block here ; PositionFilled: a block of a tetronimo is occupying this space
		PositionStatus board[N][M];								// the array to define the bucket

		bool gameOver = false;								// true iff the game is over
		bool levelChanged = false;							// true iff the level just changed

		// game audio
		audio::SoundEvent* rowClearedSound;
		audio::SoundEvent* tetrisClearedSound;
		audio::SoundEvent* newLevelSound;
		audio::SoundEvent* gameOverSound;
		audio::StreamEvent* gameMusic;
		bool musicIsPlaying = false;
						
		// initialize
		util::Expected<void> initializeBrushes();
		util::Expected<void> createTextFormatsAndLayouts();
		util::Expected<void> loadSoundEffectsAndMusic();

		// game status
	
		// initialize the game board
		void initializeGame();						// initialize the game board

		// load level from file
		void loadLevel(const unsigned int level = 0);	// read level file from hard drive
	
		// render
		void drawBoard(float opacity) const;

		// update score
		void updateScore(const unsigned int nRows);
	
	public:
		GameBoard(core::DirectXApp& dxApp);
		~GameBoard();

		bool gamePaused = false;							// true iff the game is paused

		// game status
		const bool isGameOver() const;			// if there are any blocks in the top row, the player lost the game
	
		// update the game
		void update(const double deltaTime);

		// handle input
		void moveLeft();
		void moveRight();

		// render
		const unsigned int getPixelX(const unsigned int position) const;
		const unsigned int getPixelY(const unsigned int position) const;
		void draw() const;		// draws the game board

		// get score
		//Score* const getCurrentScore() const { return currentScore; };
		//const Score& getCurrentHighscore() const;
		//HighscoreTable& getHighscoreTable() const;

		friend class boost::serialization::access;
	};
}
