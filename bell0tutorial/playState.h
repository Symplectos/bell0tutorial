#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		14/06/2018 - Lenningen - Luxembourg
*
* Desc:		main state of the running game
*
* Hist:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// c++ includes
#include <iostream>
#include <array>

// Microsoft
#include <wrl.h>

// direct write
#include <dwrite_3.h>

// bell0bytes core
#include "states.h"

// DEFINITIONS //////////////////////////////////////////////////////////////////////////
namespace core
{
	class DirectXApp;
	class DepescheSender;
	class DepescheDestination;
}

namespace core
{
	class DirectXApp;
}

namespace game
{
	class GameBoard;

	class PlayState : public core::GameState
	{
	private:
		// the constructor is private to ensure that this will be a singleton
		PlayState(core::DirectXApp& app, const std::wstring& name);

		GameBoard* gb;	// the actual board of the game

		
	public:
		virtual ~PlayState();

		// singleton: get instance
		static PlayState& createInstance(core::DirectXApp& app, const std::wstring& name);

		// initialization
		virtual util::Expected<void> initialize() override;
		virtual util::Expected<void> shutdown() override;

		// pause and resume
		virtual util::Expected<void> pause() override;
		virtual util::Expected<void> resume() override;

		// user input
		virtual util::Expected<void> handleInput(std::unordered_map<input::GameCommands, input::GameCommand&>& activeKeyMap) override;
		virtual util::Expected<void> update(const double deltaTime) override;

		// render the scene
		virtual util::Expected<void> render(const double farSeer) override;

		// handle message
		virtual util::Expected<void> onMessage(const core::Depesche&) override;
	};
}