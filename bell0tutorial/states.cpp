// INCLUDES /////////////////////////////////////////////////////////////////////////////

// bell0bytes core
#include "app.h"
#include "states.h"

// bell0byte graphics
#include "graphicsComponent.h"
#include "graphicsComponent2D.h"

// CLASS METHODS ////////////////////////////////////////////////////////////////////////
namespace core
{
	/////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Constructor and Destructor ////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	GameState::GameState(DirectXApp& app, const std::wstring& name) : dxApp(app), name(name), gc(dxApp.getGraphicsComponent()), isPaused(false), firstCreation(true)
	{ }

	GameState::~GameState()
	{ }
}