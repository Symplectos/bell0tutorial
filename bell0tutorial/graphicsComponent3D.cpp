// include the header
#include "graphicsComponent3D.h"

// bell0bytes core
#include "app.h"

// bell0bytes graphics
#include "d3d.h"
#include "graphicsHelper.h"

namespace graphics
{
	/////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Constructor and Destructors ///////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	GraphicsComponent3D::GraphicsComponent3D(core::DirectXApp& dxApp, const core::Window& appWindow, const unsigned int virtualWidth, const unsigned int virtualHeight): d3d(NULL)
	{
		// initialize Direct3D
		try { d3d = new Direct3D(dxApp, appWindow, virtualWidth, virtualHeight); }
		catch (std::runtime_error& e)
		{ throw e; }
	}
	GraphicsComponent3D::~GraphicsComponent3D()
	{
		if (d3d)
			delete d3d;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Scene Presentation ///////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void GraphicsComponent3D::clearBuffers()
	{
		d3d->clearBuffers();
	}
	void GraphicsComponent3D::clearBuffers(float colour[4])
	{
		d3d->clearBuffers(colour);
	}
	util::Expected<int> GraphicsComponent3D::present()
	{
		return d3d->present();
	}
	unsigned int GraphicsComponent3D::getNumberOfSupportedModes() const
	{
		return d3d->getNumberOfSupportedModes();
	}
	unsigned int GraphicsComponent3D::getCurrentModeIndex() const
	{
		return d3d->getCurrentModeIndex();
	}
	const DXGI_MODE_DESC* const GraphicsComponent3D::getSupportedModes() const
	{ return d3d->getSupportedModes(); };

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Polygons /////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	util::Expected<void> GraphicsComponent3D::createPolygon(Microsoft::WRL::ComPtr<ID3D11Buffer>* vertexBuffer) const
	{
		// create triangle
		//graphics::VERTEX triangleVertices[] = { { 0.0f, 0.5f, 0.0f }, { 0.45f, -0.5f, 0.0f }, {-0.45f, -0.5f, 0.0f } };
		graphics::VERTEX triangleVertices[] = { { 0.0f, 0.5f, 0.0f, graphics::randomColour(), graphics::randomColour(), graphics::randomColour() },{ 0.45f, -0.5f, 0.0f, graphics::randomColour(), graphics::randomColour(), graphics::randomColour() },{ -0.45f, -0.5f, 0.0f, graphics::randomColour(), graphics::randomColour(), graphics::randomColour() } };
		//graphics::VERTEX triangleVertices[] = { { 0.0f, 0.5f, 0.0f, 1.0f, 0.5f, 0.25f }, { 0.45f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f }, {-0.45f, -0.5f, 0.0f, 1.0f, 0.0f, 0.25f } };

		// set up buffer description
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(graphics::VERTEX) * ARRAYSIZE(triangleVertices);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// define subresource data
		D3D11_SUBRESOURCE_DATA srd = { triangleVertices, 0,0 };

		// create the vertex buffer
		if (FAILED(d3d->dev->CreateBuffer(&bd, &srd, &*vertexBuffer)))
			return "Critical Error: Unable to create vertex buffer!";
		else
			return { };
	}

	void GraphicsComponent3D::drawPolygon(Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer) const
	{
		// set the vertex buffer
		unsigned int stride = sizeof(VERTEX);
		unsigned int offset = 0;
		d3d->devCon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

		// set primitive topology
		d3d->devCon->Draw(3, 0);
	}

}