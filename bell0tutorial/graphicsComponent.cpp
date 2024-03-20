// include the header
#include "graphicsComponent.h"

// bell0bytes core
#include "timer.h"

// bell0bytes 3D graphics
#include "graphicsComponent3D.h"
#include "d3d.h"

// bell0bytes 2D graphics
#include "graphicsComponent2D.h"
#include "d2d.h"

// bell0bytes write
#include "graphicsComponentWrite.h"

// bell0bytes util
#include "serviceLocator.h"

namespace graphics
{
	/////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Constructor and Destructors ///////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	GraphicsComponent::GraphicsComponent(core::DirectXApp& dxApp, const core::Window& appWindow)
	{
		// create Direct3D components
		try { graphics3D = new GraphicsComponent3D(dxApp, appWindow, virtualWidth, virtualHeight); }
		catch (std::runtime_error& e) { throw e; }

		// create Direct2D components
		try { graphics2D = new GraphicsComponent2D(dxApp, *graphics3D->d3d); }
		catch (std::runtime_error& e) { throw e; }

		// create DirectWrite component
		try { graphicsWrite = new GraphicsComponentWrite(dxApp, graphics2D->d2d->getDeviceContext(), graphics2D->d2d->getWriteFactory(), graphics2D->d2d->getBlackBrush()); }
		catch (std::runtime_error& e) { throw e; }
	}
	GraphicsComponent::~GraphicsComponent()
	{
		if (graphicsWrite)
			delete graphicsWrite;

		if (graphics2D)
			delete graphics2D;

		if (graphics3D)
			delete graphics3D;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Fullscreen State //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	util::Expected<void> GraphicsComponent::checkFullscreen(core::DirectXApp& dxApp, core::Timer& timer, const bool applicationStarted, bool& isPaused)
	{
		util::Expected<void> result;

		if (applicationStarted)
		{
			util::Expected<bool> switchFullscreenState = graphics3D->d3d->switchFullscreen();
			if (switchFullscreenState.isValid())
			{
				if (switchFullscreenState.get())
				{	// fullscreen mode changed, pause the application, resize everything and unpause the application again
					isPaused = true;
					result = timer.stop();
					if (!result.isValid())
						return result;
					result = onResize(dxApp);
					if (!result.isValid())
						return result;
					result = timer.start();
					if (!result.isValid())
						return result;
					isPaused = false;
				}
			}
			else
				return switchFullscreenState;
		}

		return {};
	}
	util::Expected<void> GraphicsComponent::toggleFullscreen() const
	{
		return graphics3D->d3d->toggleFullscreen();
	}
	const bool GraphicsComponent::getFullscreenState() const
	{
		return graphics3D->d3d->getFullscreenState();
	};

	/////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Screen Resolution /////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	unsigned int GraphicsComponent::getCurrentWidth() const
	{
		return graphics3D->d3d->getCurrentWidth();
	}
	unsigned int GraphicsComponent::getVirtualWidth() const
	{
		return virtualWidth;
	}
	unsigned int GraphicsComponent::getCurrentHeight() const
	{
		return graphics3D->d3d->getCurrentHeight();
	}
	unsigned int GraphicsComponent::getVirtualHeight() const
	{
		return virtualHeight;
	}
	unsigned int GraphicsComponent::getCurrentRefreshRateNum() const
	{
		return graphics3D->d3d->getCurrentRefreshRateNum();
	}
	unsigned int GraphicsComponent::getCurrentRefreshRateDen() const
	{
		return graphics3D->d3d->getCurrentRefreshRateDen();
	}
	util::Expected<void> GraphicsComponent::changeResolution(const unsigned int index) const
	{
		return graphics3D->d3d->changeResolution(index);
	};

	unsigned int GraphicsComponent::getCurrentModeIndex() const
	{
		return graphics3D->getCurrentModeIndex();
	}

	unsigned int GraphicsComponent::getNumberOfSupportedModes() const
	{
		return graphics3D->getNumberOfSupportedModes();
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////// Get Components ////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	GraphicsComponentWrite& GraphicsComponent::getWriteComponent() const
	{
		return *graphicsWrite;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////// Resize //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	util::Expected<void> GraphicsComponent::onResize(core::DirectXApp& dxApp)
	{
#ifndef NDEBUG
		util::ServiceLocator::getFileLogger()->print<util::SeverityType::warning>("The window was resized. The game graphics must be updated!");
#endif
		// handle errors
		util::Expected<void> result;
		
		// resize graphics
		result = graphics3D->d3d->onResize(graphics2D->d2d);
		if (!result.isValid())
			return result;

		// recreate the DirectWrite component
		delete graphicsWrite;
		try { graphicsWrite = new GraphicsComponentWrite(dxApp, graphics2D->d2d->getDeviceContext(), graphics2D->d2d->getWriteFactory(), graphics2D->d2d->getBlackBrush()); }
		catch (std::runtime_error& e) { throw e; }

		// return success
		return {};
	}

	void GraphicsComponent::setResolutionIndependentTransformation() const
	{
		graphics2D->d2d->getDeviceContext().SetTransform(graphics3D->d3d->getResolutionIndependentTransformationMatrix());
	}

	void GraphicsComponent::translate(const float x, const float y) const
	{
		D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(x, y);
		graphics2D->d2d->getDeviceContext().SetTransform(translationMatrix);
	}

	void GraphicsComponent::resetTransformation() const
	{
		graphics2D->d2d->getDeviceContext().SetTransform(D2D1::Matrix3x2F::Identity());
	}

	D2D1::Matrix3x2F GraphicsComponent::getResolutionIndependentTransformationMatrix() const
	{
		return graphics3D->d3d->getResolutionIndependentTransformationMatrix();
	}

	D2D1::Matrix3x2F GraphicsComponent::getInverseResolutionIndependentTransformationMatrix() const
	{
		return graphics3D->d3d->getInverseResolutionIndependentTransformationMatrix();
	}

	float GraphicsComponent::getBiggestScalingFactor() const
	{
		return graphics3D->d3d->getLargestScalingFactor();
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////// Rendering ///////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void GraphicsComponent::clearBuffers() const
	{
		graphics3D->clearBuffers();
	}

	void GraphicsComponent::clearBuffers(float colour[4]) const
	{
		graphics3D->clearBuffers(colour);
	}

	void GraphicsComponent::beginDraw() const
	{
		graphics2D->beginDraw();
	}

	void GraphicsComponent::setScene() const
	{
		graphics3D->clearBuffers();
		this->setResolutionIndependentTransformation();
	}

	void GraphicsComponent::setScene(float colour[4]) const
	{
		graphics3D->clearBuffers(colour);
		this->setResolutionIndependentTransformation();
	}

	util::Expected<void> GraphicsComponent::endDraw() const
	{
		return graphics2D->endDraw();
	}

	util::Expected<int> GraphicsComponent::presentScene() const
	{
		return graphics3D->present();
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////Animation and Sprites /////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	util::Expected<void> GraphicsComponent::createNewAnimationData(const std::vector<AnimationCycleData>& animationCycleData, const std::wstring& fileName, AnimationData** animationData) const
	{
		try { *animationData = new AnimationData(*this->graphics2D->d2d, fileName.c_str(), animationCycleData); }
		catch (std::exception& e) { return e; }

		return { };
	}

	AnimatedSprite* GraphicsComponent::createNewAnimatedSprite(AnimationData* const animData, const unsigned int activeAnimation, const float animationFPS, const float x, const float y, const Layers layer, const unsigned int drawOrder) const
	{
		AnimatedSprite* animSprite = new AnimatedSprite(*this->graphics2D->d2d, animData, activeAnimation, animationFPS, x, y, layer, drawOrder);
		if (animSprite)
			return animSprite;
		else
			throw std::runtime_error("Unable to create cursor sprite!");
	}
}
