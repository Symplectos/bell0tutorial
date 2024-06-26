// Lua and Sol
//#pragma warning( push )
//#pragma warning( disable : 4127)	// disable constant if expr warning
//#pragma warning( disable : 4702)	// disable unreachable code warning
#include <sol.hpp>
//#pragma warning( pop ) 
#pragma comment(lib, "liblua53.a")

// the header
#include "d3d.h"

// bell0bytes core
#include "app.h"
#include "window.h"

// bell0bytes file system
#include "fileSystemComponent.h"

// bell0bytes util
#include "serviceLocator.h"
#include "stringConverter.h"

// bell0bytes graphics
#include "graphicsComponent.h"
#include "d2d.h"

// bell0bytes input
#include "gameCommands.h"

namespace graphics
{
	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Constructor //////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	Direct3D::Direct3D(core::DirectXApp& dxApp, const core::Window& window, const unsigned int virtualWidth, const unsigned int virtualHeight) : dxApp(dxApp), appWindow(window.getMainWindowHandle()), desiredColourFormat(DXGI_FORMAT_B8G8R8A8_UNORM), startInFullscreen(false), currentModeIndex(-1), currentlyInFullscreen(false), changeMode(false), virtualWidth(virtualWidth), virtualHeight(virtualHeight)
	{
		// handle errors
		HRESULT hr;
		util::Expected<void> result;

		// read configuration file
		if (!readConfigurationFile().wasSuccessful())
			throw std::runtime_error("Unable to read configuration file!");

		// define device creation flags,  D3D11_CREATE_DEVICE_BGRA_SUPPORT needed to get Direct2D interoperability with Direct3D resources
		unsigned int createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

		// if in debug mode, create device with debug layer
#ifndef NDEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// create the device
		D3D_FEATURE_LEVEL featureLevel;
		hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, NULL, 0, D3D11_SDK_VERSION, &dev, &featureLevel, &devCon);

		if (FAILED(hr))
		{
			util::ServiceLocator::getFileLogger()->print<util::SeverityType::error>("The creation of the Direct3D device and its context failed!");
			throw std::runtime_error("Unable to create the Direct3D device and its context!");
		}
		else if (featureLevel < D3D_FEATURE_LEVEL_11_0)
		{
			util::ServiceLocator::getFileLogger()->print<util::SeverityType::error>("Critical error: DirectX 11 is not supported by your GPU!");
			throw std::runtime_error("Unable to create the Direct3D device and its context!");
		}

		// now that the device and its context are available, create further resouces
		if (!createResources(nullptr, window).wasSuccessful())
		{
			util::ServiceLocator::getFileLogger()->print<util::SeverityType::error>("Critical error: Creation of Direct3D resources failed!");
			throw std::runtime_error("Creation of Direct3D resources failed!");
		}

		// add core DirectXApp as observer
		addObserver(&dxApp);

		//  log success
		util::ServiceLocator::getFileLogger()->print<util::SeverityType::info>("Direct3D was initialized successfully.");
	}
	Direct3D::~Direct3D()
	{
		// switch to windowed mode before exiting the application
		swapChain->SetFullscreenState(false, nullptr);

		// delete supported modes array
		delete[] supportedModes;

		util::ServiceLocator::getFileLogger()->print<util::SeverityType::info>("Direct3D was shut down successfully.");
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Resource Creation ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	util::Expected<void> Direct3D::createResources(Direct2D* const d2d, const core::Window& window)
	{
		// create the swap chain

		// fill in the swap chain description
		DXGI_SWAP_CHAIN_DESC scd;
		scd.BufferDesc.Width = 0;													// width of the back buffer
		scd.BufferDesc.Height = 0;													// height
		scd.BufferDesc.RefreshRate.Numerator = 0;									// refresh rate: 0 -> do not care
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = desiredColourFormat;								// the color palette to use								
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// unspecified scan line ordering
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;						// unspecified scaling
		scd.SampleDesc.Count = 1;													// disable msaa
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;							// use back buffer as render target
		scd.BufferCount = 3;														// the number of buffers in the swap chain (including the front buffer)
		scd.OutputWindow = window.getMainWindowHandle();							// set the main window as output target
		scd.Windowed = true;														// windowed, not fullscreen
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;								// flip mode and discarded buffer after presentation
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;							// allow mode switching

		// get the DXGI factory
		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;

		// first, retrieve the underlying DXGI device from the Direct3D device
		HRESULT hr = dev.As(&dxgiDevice);
		if (FAILED(hr))
			return std::runtime_error("The Direct3D device was unable to retrieve the underlying DXGI device!");

		// now identify the physical GPU this device is running on
		hr = dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
		if (FAILED(hr))
			return std::runtime_error("The DXGI Device was unable to get the GPU adapter!");

		// finally retrieve the factory
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory);
		if (FAILED(hr))
			return std::runtime_error("The DXGI Adapter was unable to get the factory!");

		// create the actual swap chain
		hr = dxgiFactory->CreateSwapChain(dev.Get(), &scd, swapChain.GetAddressOf());
		if (FAILED(hr))
			return std::runtime_error("The creation of the swap chain failed!");

		// enumerate all available display modes

		// get representation of the output adapter
		IDXGIOutput *output = nullptr;
		if (FAILED(swapChain->GetContainingOutput(&output)))
			return std::runtime_error("Unable to retrieve the output adapter!");

		// get the amount of supported display modes for the desired format
		if (FAILED(output->GetDisplayModeList(desiredColourFormat, 0, &numberOfSupportedModes, NULL)))
			return std::runtime_error("Unable to list all supported display modes!");

		// set up array for the supported modes
		supportedModes = new DXGI_MODE_DESC[numberOfSupportedModes];
		ZeroMemory(supportedModes, sizeof(DXGI_MODE_DESC) * numberOfSupportedModes);

		// fill the array with the available display modes
		if (FAILED(output->GetDisplayModeList(desiredColourFormat, 0, &numberOfSupportedModes, supportedModes)))
			return std::runtime_error("Unable to retrieve all supported display modes!");

		// release the output adapter
		output->Release();

		// if the index of the desired supported mode is known, activate it
		bool supportedMode = false;
		if (currentModeIndex != -1 && currentModeIndex >= 0 && currentModeIndex < (int)numberOfSupportedModes)
		{
			currentModeDescription = supportedModes[currentModeIndex];
			supportedMode = true;
		}
		else
		{
			// search for supported more
			for (unsigned int i = numberOfSupportedModes-1; i > 0; i--)
				if ((unsigned int)window.getClientWidth() == supportedModes[i].Width && (unsigned int)window.getClientHeight() == supportedModes[i].Height)
				{
					supportedMode = true;
					currentModeDescription = supportedModes[i];
					currentModeIndex = i;
					break;
				}
		}

		if (!supportedMode)
		{
			// print a warning 
			util::ServiceLocator::getFileLogger()->print<util::SeverityType::warning>("The desired screen resolution is not supported! Resizing...");

			// set the mode to the highest supported resolution
			currentModeDescription = supportedModes[numberOfSupportedModes-1];
			currentModeIndex = numberOfSupportedModes-1;
			if (FAILED(swapChain->ResizeTarget(&currentModeDescription)))
				return std::runtime_error("Unable to resize target to a supported display mode!");

			// write the current mode to the configuration file
			if (!writeCurrentModeDescriptionToConfigurationFile().wasSuccessful())
				return std::runtime_error("Unable to write to the configuration file!");
		}

		// set fullscreen mode?
		if (startInFullscreen)
		{
			// switch to fullscreen mode
			if (FAILED(swapChain->SetFullscreenState(true, nullptr)))
				return std::runtime_error("Unable to switch to fullscreen mode!");
			currentlyInFullscreen = true;
		}
		else
			currentlyInFullscreen = false;

		// the remaining steps need to be done each time the window is resized
		if (!onResize(d2d).wasSuccessful())
			return std::runtime_error("Direct3D was unable to resize its resources!");

		// return success
		return {};
	}
	util::Expected<void> Direct3D::onResize(Direct2D* const d2d)
	{
		// Microsoft recommends zeroing out the refresh rate of the description before resizing the targets
		DXGI_MODE_DESC zeroRefreshRate = currentModeDescription;
		zeroRefreshRate.RefreshRate.Numerator = 0;
		zeroRefreshRate.RefreshRate.Denominator = 0;

		// check for fullscreen switch
		BOOL inFullscreen = false;
		swapChain->GetFullscreenState(&inFullscreen, NULL);

		if (currentlyInFullscreen != inFullscreen)
		{
			// fullscreen switch
			if (inFullscreen)
			{
				// switched to fullscreen -> Microsoft recommends resizing the target before going into fullscreen
				if (FAILED(swapChain->ResizeTarget(&zeroRefreshRate)))
					return std::runtime_error("Unable to resize target!");

				// set fullscreen state
				if (FAILED(swapChain->SetFullscreenState(true, nullptr)))
					return std::runtime_error("Unable to switch to fullscreen mode!");
			}
			else
			{
				// switched to windowed -> simply set fullscreen mode to false
				if (FAILED(swapChain->SetFullscreenState(false, nullptr)))
					return std::runtime_error("Unable to switch to windowed mode mode!");

				// recompute client area and set new window size
				RECT rect = { 0, 0, (long)currentModeDescription.Width,  (long)currentModeDescription.Height };
				if (FAILED(AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW)))
					return std::runtime_error("Failed to adjust window rectangle!");
				SetWindowPos(appWindow, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);
			}

			// change fullscreen mode
			currentlyInFullscreen = !currentlyInFullscreen;
		}
				
		// resize target to the desired resolution
		if (FAILED(swapChain->ResizeTarget(&zeroRefreshRate)))
			return std::runtime_error("Unable to resize target!");

		// release and reset all resources
		if (d2d)
			d2d->devCon->SetTarget(nullptr);

		devCon->ClearState();
		renderTargetView = nullptr;
		depthStencilView = nullptr;
		
		// resize the swap chain
		if(FAILED(swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
			return std::runtime_error("Direct3D was unable to resize the swap chain!");

		// (re)-create the render target view
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()))))
			return std::runtime_error("Direct3D was unable to acquire the back buffer!");
		if (FAILED(dev->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView)))
			return std::runtime_error("Direct3D was unable to create the render target view!");

		// create the depth and stencil buffer
		D3D11_TEXTURE2D_DESC dsd;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> dsBuffer;
		backBuffer->GetDesc(&dsd);
		dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsd.Usage = D3D11_USAGE_DEFAULT;
		dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		if (FAILED(dev->CreateTexture2D(&dsd, NULL, dsBuffer.ReleaseAndGetAddressOf())))
			return std::runtime_error("Direct3D was unable to create a 2D-texture!");
		if (FAILED(dev->CreateDepthStencilView(dsBuffer.Get(), NULL, depthStencilView.GetAddressOf())))
			return std::runtime_error("Direct3D was unable to create the depth and stencil buffer!");

		// activate the depth and stencil buffer
		devCon->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

		// set the viewport depending on the screen resolution
		unsigned int currentHeight = currentModeDescription.Height;
		unsigned int currentWidth = currentModeDescription.Width;

		float targetAspectRatio = (float)virtualWidth / (float)virtualHeight;

		// figure out the largest area that fits in this resolution at the desired aspect ratio
		unsigned int width = currentWidth;
		unsigned int height = (unsigned int)(width / targetAspectRatio + 0.5f);

		if (height > currentHeight)
		{
			// it doesn't fit our height, we must switch to pillarbox then
			height = currentHeight;
			width = (unsigned int)((float)height * targetAspectRatio + 0.5f);
		}

		// set up the new viewport centered in the backbuffer
		float vpX = 0.5f * (currentWidth - width);
		float vpY = 0.5f * (currentHeight - height);

		// define the viewport
		D3D11_VIEWPORT vp;
		vp.TopLeftX = vpX;
		vp.TopLeftY = vpY;
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		devCon->RSSetViewports(1, &vp);

		// recompute the transformation matrix
		computeResolutionIndependentTransformationMatrix();

		// (re)-create the Direct2D target bitmap associated with the swap chain back buffer and set it as the current target
		if(d2d)
			if(!d2d->createBitmapRenderTarget(*this).wasSuccessful())
				return std::runtime_error("Direct3D was unable to resize the Direct2D bitmap render target!");

		// re-create Direct2D device dependent resources
		if (d2d)
			if (!d2d->createDeviceDependentResources().wasSuccessful())
				return std::runtime_error("Direct3D was unable to resize the Direct2D device dependent resources!");

		// re-initialize GPU pipeline
		initPipeline();

		// log and return success
#ifndef NDEBUG
		if (dxApp.gameHasStarted())
			util::ServiceLocator::getFileLogger()->print<util::SeverityType::info>("The Direct3D and Direct2D resources were resized successfully.");
#endif
		return {};
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Pipeline ////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	util::Expected<void> Direct3D::initPipeline()
	{
		// load Compiled Shader Object files
#ifndef NDEBUG
		util::Expected<ShaderBuffer> vertexShaderBuffer = loadShader(L"../x64/Debug/vertexShader.cso");
		util::Expected<ShaderBuffer> pixelShaderBuffer = loadShader(L"../x64/Debug/pixelShader.cso");
#else
		util::Expected<ShaderBuffer> vertexShaderBuffer = loadShader(L"../x64/Release/vertexShader.cso");
		util::Expected<ShaderBuffer> pixelShaderBuffer = loadShader(L"../x64/Release/pixelShader.cso");
#endif
		if (!vertexShaderBuffer.wasSuccessful() || !pixelShaderBuffer.wasSuccessful())
			return "Critical error: Unable to read Compiled Shader Object files!";

		// create the shaders
		if (FAILED(dev->CreateVertexShader(vertexShaderBuffer.get().buffer, vertexShaderBuffer.get().size, nullptr, &standardVertexShader)))
			return "Critical error: Unable to create the vertex shader!";
		if(FAILED(dev->CreatePixelShader(pixelShaderBuffer.get().buffer, pixelShaderBuffer.get().size, nullptr, &standardPixelShader)))
			return "Critical error: Unable to create the pixel shader!";

		// set the shader objects as the active shaders
		devCon->VSSetShader(standardVertexShader.Get(), nullptr, 0);
		devCon->PSSetShader(standardPixelShader.Get(),  nullptr, 0);

		// specify the input layout
		D3D11_INPUT_ELEMENT_DESC ied[] = {	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
											{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 } };

		// create the input layout
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		if (FAILED(dev->CreateInputLayout(ied, ARRAYSIZE(ied), vertexShaderBuffer.get().buffer, vertexShaderBuffer.get().size, &inputLayout)))
			return "Critical error: Unable to create the input layout!";

		// set active input layout
		devCon->IASetInputLayout(inputLayout.Get());

		// define and set the constant buffers
		
		// constant colour buffer
		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = 32;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (FAILED(dev->CreateBuffer(&bd, nullptr, &constantColourPositionBuffer)))
			return "Critical error: Unable to create the constant colour and position buffer!";
		devCon->VSSetConstantBuffers(0, 1, constantColourPositionBuffer.GetAddressOf());

		// delete shader buffer pointers
		delete vertexShaderBuffer.get().buffer;
		delete pixelShaderBuffer.get().buffer;

		// log and return return success
#ifndef NDEBUG
		util::ServiceLocator::getFileLogger()->print<util::SeverityType::info>("The rendering pipeline was successfully initialized.");
#endif
		return { };
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// Shaders ////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	const util::Expected<ShaderBuffer> Direct3D::loadShader(const std::wstring& filename) const
	{
		// load precompiled shaders from .cso objects
		ShaderBuffer sb;
		byte* fileData = nullptr;

		// open the file
		std::ifstream csoFile(filename, std::ios::in | std::ios::binary | std::ios::ate);

		if (csoFile.is_open())
		{
			// get shader size
			sb.size = (unsigned int)csoFile.tellg();

			// collect shader data
			fileData = new byte[sb.size];
			csoFile.seekg(0, std::ios::beg);
			csoFile.read(reinterpret_cast<char*>(fileData), sb.size);
			csoFile.close();
			sb.buffer = fileData;
		}
		else
			return "Critical error: Unable to open the compiled shader object!";

		// return the shader buffer
		return sb;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Scene Presentation ///////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void Direct3D::clearBuffers()
	{
		// clear the back buffer and depth / stencil buffer
		float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		this->clearBuffers(white);
	}
	void Direct3D::clearBuffers(float colour[4])
	{
		// clear the back buffer and depth / stencil buffer
		devCon->ClearRenderTargetView(renderTargetView.Get(), colour);
		devCon->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	util::Expected<int> Direct3D::present()
	{
		HRESULT hr = swapChain->Present(0, DXGI_PRESENT_DO_NOT_WAIT);
		if (FAILED(hr) && hr != DXGI_ERROR_WAS_STILL_DRAWING)
		{	
			util::ServiceLocator::getFileLogger()->print<util::SeverityType::error>("The presentation of the scene failed!");
			return std::runtime_error("Direct3D failed to present the scene!");
		}

		// rebind the depth and stencil buffer - necessary since the flip model releases the view targets after a call to present
		devCon->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

		// return success
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Screen Resolution ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void Direct3D::changeResolution(bool increase)
	{
		if (increase)
		{
			// if increase is true, choose a higher resolution, if possible
			if (currentModeIndex < (int)numberOfSupportedModes - 1)
			{
				currentModeIndex++;
				changeMode = true;
			}
			else
				changeMode = false;
		}
		else
		{
			// else choose a smaller resolution, but only if possible
			if (currentModeIndex > 0)
			{
				currentModeIndex--;
				changeMode = true;
			}
			else
				changeMode = false;
		}

		if (changeMode)
		{
			// change mode
			currentModeDescription = supportedModes[currentModeIndex];

			// resize everything
			notify(input::Events::ChangeResolution);
		}
	}
	util::Expected<void> Direct3D::changeResolution(const unsigned int index)
	{
		// change mode
		currentModeIndex = index;
		currentModeDescription = supportedModes[currentModeIndex];

		// resize everything
		return notify(input::Events::ChangeResolution);
	}
	util::Expected<void> Direct3D::toggleFullscreen()
	{
		if (!currentlyInFullscreen)
		{
			// switch to fullscreen mode
			if (FAILED(swapChain->SetFullscreenState(true, nullptr)))
				return std::runtime_error("Unable to switch to fullscreen mode!");
			currentlyInFullscreen = true;
		}
		else
		{
			// switch to windowed mode
			if (FAILED(swapChain->SetFullscreenState(false, nullptr)))
				return std::runtime_error("Unable to switch to fullscreen mode!");
			currentlyInFullscreen = false;
		}

		return dxApp.getGraphicsComponent().onResize(dxApp);
	}
	util::Expected<bool> Direct3D::switchFullscreen() const
	{
		BOOL fullscreen;
		if (FAILED(swapChain->GetFullscreenState(&fullscreen, nullptr)))
			return std::runtime_error("Error: Unable to query fullscreen state!");

		return(fullscreen != currentlyInFullscreen ? true : false);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////// Resolution Independence //////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void Direct3D::computeResolutionIndependentTransformationMatrix()
	{
		// get dimensions
		float currentWidth = (float)currentModeDescription.Width;
		float currentHeight = (float)currentModeDescription.Height;
	
		// translation

		// translate to the middle of the screen
		float translateX = 0.5f * (currentWidth - virtualWidth);
		float translateY = 0.5f * (currentHeight - virtualHeight);
		D2D1::Matrix3x2F translationMatrix = D2D1::Matrix3x2F::Translation(translateX, translateY);

		// scaling
		float scaleX = currentWidth / virtualWidth;
		float scaleY = currentHeight / virtualHeight;

		// get middle of the screen
		float x = currentWidth * 0.5f;
		float y = currentHeight * 0.5f;
		D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(scaleX, scaleY, D2D1::Point2F(x, y));

		// multiply the matrices
		D2D1::Matrix3x2F transformationMatrix = translationMatrix * scaleMatrix;

		// store the matrix
		this->resolutionIndependentTransformationMatrix = transformationMatrix;

		// compute and store the inverse
		transformationMatrix.Invert();
		this->inverseResolutionIndependentTransformationMatrix = transformationMatrix;

		// store largest scaling factor (for mouse scaling)
		if (scaleX > scaleY)
			largestScalingFactor = scaleX;
		else
			largestScalingFactor = scaleY;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Helper Functions /////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	util::Expected<void> Direct3D::writeCurrentModeDescriptionToConfigurationFile() const
	{
		// create the string to print
		std::string resolution = "\tresolution = { width = " + std::to_string(currentModeDescription.Width) + ", height = " + std::to_string(currentModeDescription.Height) + " },";
		// append name of the log file to the path
		std::wstring pathToPrefFile = dxApp.getFileSystemComponent().getPathToConfigurationFiles() + L"\\" + dxApp.getFileSystemComponent().getPrefsFile();

		// read the file
		std::vector<std::string> data;
		std::string input;

		std::ifstream prefFileStreamIn(pathToPrefFile.c_str());
		if (prefFileStreamIn.good())
		{
			while (std::getline(prefFileStreamIn, input))
				data.push_back(input);
			prefFileStreamIn.close();

			// modify the stream
			for (auto& line : data)
			{
				if (line.find("resolution") != std::string::npos)
				{
					line.replace(0, line.size(), resolution);
					break;
				}
			}
		}
		else
			return std::runtime_error("Unable to read the configuration file!");

		// write the modified stream back to the file
		std::ofstream prefFileStreamOut(pathToPrefFile.c_str());
		if (prefFileStreamOut.good())
		{
			for (auto const& line : data)
				prefFileStreamOut << line << std::endl;
		}
		else
			return std::runtime_error("Unable to write to the configuration file!");

		return {};
	}
	util::Expected<void> Direct3D::readConfigurationFile()
	{
		if (dxApp.getFileSystemComponent().hasValidConfigurationFile())
		{
			// configuration file exists, try to read from it
			std::wstring pathToPrefFile = dxApp.getFileSystemComponent().getPathToConfigurationFiles() + L"\\" + dxApp.getFileSystemComponent().getPrefsFile();

			try
			{
				sol::state lua;
				lua.script_file(util::StringConverter::ws2s(pathToPrefFile));

				// read fullscreen
				startInFullscreen = lua["config"]["fullscreen"].get_or(false);

				// read index
				currentModeIndex = lua["config"]["resolution"]["index"].get_or(-1);
#ifndef NDEBUG
				std::stringstream res;
				res << "The fullscreen mode was read from the LUA configuration file: " << std::boolalpha << startInFullscreen << ".";
				util::ServiceLocator::getFileLogger()->print<util::SeverityType::info>(res.str());
#endif
			}
			catch (std::exception)
			{
				util::ServiceLocator::getFileLogger()->print<util::SeverityType::warning>("Unable to read configuration file. Starting in windowed mode!");
			}
		}

		return { };
	}
}