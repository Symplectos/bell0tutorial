#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		27/06/2018 - Lenningen - Luxembourg
*
* Desc:		input component
* Hist:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// bell0bytes input
#include "inputHandler.h"
#include "gameCommands.h"

// DEFINITIONS //////////////////////////////////////////////////////////////////////////
namespace core
{
	class DirectXApp;
}

namespace input
{
	class InputComponent
	{
	private:
		InputHandler& ih;			// the input handler class

	public:
		InputComponent(InputHandler& ih);
		~InputComponent();

		InputHandler& getInputHandler() const;
				
		friend class core::DirectXApp;
	};
}