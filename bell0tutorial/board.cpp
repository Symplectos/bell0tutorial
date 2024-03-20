#include "board.h"
#include "app.h"
#include "graphicsComponent.h"
#include "graphicsComponent2D.h"
#include "d2d.h"
#include "expected.h"
#include "graphicsComponentWrite.h"
#include <sstream>
#include "audioComponent.h"
#include "fileSystemComponent.h"
#include <fstream>
#include <array>
#include "inputComponent.h"
#include "inputHandler.h"



namespace game
{
	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// Block ////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	Block::Block() : type(BlockTypes::Normal), pu(PowerUps::NoPU), pd(PowerDowns::NoPD), pa(PhysicalAnomalies::NoPA) {};

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// Constructor //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	GameBoard::GameBoard(core::DirectXApp& dxApp) : dxApp(dxApp)
	{
		// initialize the bucket
		
		util::Expected<void> result;

		// create text formats and layouts
		result = createTextFormatsAndLayouts();

		// initialize sound effect and music
		result = loadSoundEffectsAndMusic();

		// initialize the board
		initializeGame();
	}

	GameBoard::~GameBoard()
	{

		// stop sounds

		// stop menu music

		// delete highscore table
	}

	void GameBoard::initializeGame()
	{
		// initialize the random number generator

		// initialize score structure
	
		// initialize highscore table
	}

	void GameBoard::loadLevel(const unsigned int level)
	{

	}

	util::Expected<void> GameBoard::initializeBrushes()
	{
		// handle errors
		util::Expected<void> result;

		// return success
		return { };
	}

	util::Expected<void> GameBoard::createTextFormatsAndLayouts()
	{
		util::Expected<void> result;

		// return success
		return { };
	}

	util::Expected<void> GameBoard::loadSoundEffectsAndMusic()
	{
		util::Expected<void> result;

		// return success
		return { };
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// Status ///////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	const bool GameBoard::isGameOver() const
	{
		// if there are any blocks in the top row, the game is over
		for (unsigned int i = 0; i < N; i++)
			if (board[i][0] == PositionStatus::PositionFilled)
				return true;

		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// Update ///////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void GameBoard::update(const double deltaTime)
	{
		// update animation status
		
		// check for new level
		
		// check whether to use white brush
	}

	void GameBoard::updateScore(const unsigned int nRows)
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// Input ////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void GameBoard::moveLeft()
	{

	}

	void GameBoard::moveRight()
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// Render ///////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void GameBoard::drawBoard(float opacity) const
	{

	}

	void GameBoard::draw() const
	{
		// draw borders

		// left border
		dxApp.getGraphicsComponent().get2DComponent().drawRectangle(hudWidth - 1, 0, hudWidth + 1, 1080);
	}
}
