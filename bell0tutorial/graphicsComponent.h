#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		27/06/2018 - Lenningen - Luxembourg
*
* Desc:		The graphics component handles all graphical issues. Things that are common to both 2D and 3D are handled by this component.
*			2D and 3D specific tasks are delegated to the 2D and 3D component respectively.
*			
*			graphics components:
*				- 2D component - currently powered by Direct2D and DirectWrite
*				- 3D component - currently powered by Direct3D
*				- Sprites
* Hist:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// C++ includes
#include <string>

// bell0bytes graphics
#include "sprites.h"

// DEFINITIONS //////////////////////////////////////////////////////////////////////////

namespace util
{
	template<typename T>
	class Expected;
}

namespace core
{
	class DirectXApp;
	class Window;
	class Timer;
}

namespace graphics
{
	class GraphicsComponent2D;
	class GraphicsComponent3D;
	class GraphicsComponentWrite;

	class GraphicsComponent
	{
	private:
		GraphicsComponent2D* graphics2D;									// all things related to 2D graphics
		GraphicsComponent3D* graphics3D;									// all things related to 3D graphics
		GraphicsComponentWrite* graphicsWrite;								// all things related to writing text

		// virtual screen resolution (resolution the game is designed and optimized for)
		const unsigned int virtualWidth = 1920;
		const unsigned int virtualHeight = 1080;

		// resize functions
		virtual util::Expected<void> onResize(core::DirectXApp& dxApp);		// resize game graphics

		// screen modes
		unsigned int getCurrentModeIndex()  const;
		unsigned int getNumberOfSupportedModes() const;

	public:
		// constructs and destructors (dxApp can't be const because of the observer pattern)
		GraphicsComponent(core::DirectXApp& dxApp, const core::Window& appWindow);
		~GraphicsComponent();

		// retrieve the components
		GraphicsComponentWrite& getWriteComponent() const;

		// screen resolution
		unsigned int getCurrentWidth() const;
		unsigned int getCurrentHeight() const;
		unsigned int getCurrentRefreshRateNum() const;
		unsigned int getCurrentRefreshRateDen() const;
		unsigned int getVirtualWidth() const;
		unsigned int getVirtualHeight() const;


		// fullscreen state
		const bool getFullscreenState() const;
		util::Expected<void> checkFullscreen(core::DirectXApp& dxApp, core::Timer& timer, const bool applicationStarted, bool& isPaused);
		util::Expected<void> toggleFullscreen() const;
		util::Expected<void> changeResolution(const unsigned int index) const;

		// resolution independent transformation
		void setResolutionIndependentTransformation() const;
		void resetTransformation() const;
		D2D1::Matrix3x2F getResolutionIndependentTransformationMatrix() const;
		D2D1::Matrix3x2F getInverseResolutionIndependentTransformationMatrix() const;
		float getBiggestScalingFactor() const;
		void translate(const float x, const float y) const;

		// rendering
		void clearBuffers() const;							// clear the back and depth/stencil buffers - default: white
		void clearBuffers(float[4]) const;					// as above, but specifies the background colour
		void setScene() const;								// sets the scene: 1. clears the buffers 2. sets resolution independent rendering
		void setScene(float[4]) const;						// see clearBuffers
		void beginDraw() const;								// starts Direct2D drawing
		util::Expected<void> endDraw() const;				// ends Direct2D drawing
		util::Expected<int> presentScene() const;			// presents the scene

		// get components
		const GraphicsComponent2D& get2DComponent() const { return *this->graphics2D; };
		const GraphicsComponent3D& get3DComponent() const { return *this->graphics3D; };

		// sprites and animations
		util::Expected<void> createNewAnimationData(const std::vector<AnimationCycleData>&, const std::wstring&, AnimationData**) const;		// given the data of the animation cycle and a string to the image to load, this function creates the animation data for an animated sprite
		AnimatedSprite* createNewAnimatedSprite(AnimationData* const animData, const unsigned int activeAnimation = 0, const float animationFPS = 24, const float x = 0.0f, const float y = 0.0f, const Layers layer = Layers::Characters, const unsigned int drawOrder = 0) const;
		
		// friends
		friend class core::DirectXApp;
		friend class Direct3D;
		friend class GraphicsComponent2D;
		friend class GraphicsComponent3D;
	};
}