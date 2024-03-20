// C++
#include <vector>
#include <string>

// include the header
#include "graphicsComponent2D.h"

// bell0bytes core
#include "app.h"

// bell0bytes graphics
#include "graphicsComponent.h"
#include "d2d.h"
#include "d3d.h"

namespace graphics
{
	/////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Constructor and Destructors ///////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	GraphicsComponent2D::GraphicsComponent2D(const core::DirectXApp& dxApp, const Direct3D& d3d) : d2d(NULL), matrixRotation90CW(D2D1::Matrix3x2F::Rotation(90)),
		matrixRotation180CW(D2D1::Matrix3x2F::Rotation(180)),
		matrixRotation270CW(D2D1::Matrix3x2F::Rotation(270)),
		matrixRotation90CCW(D2D1::Matrix3x2F::Rotation(-90)),
		matrixRotation180CCW(D2D1::Matrix3x2F::Rotation(-180)),
		matrixRotation270CCW(D2D1::Matrix3x2F::Rotation(-270))
	{
		// initialize Direct2D
		try { d2d = new graphics::Direct2D(dxApp, d3d); }
		catch (std::runtime_error& e)
		{ throw e; }
	}
	GraphicsComponent2D::~GraphicsComponent2D()
	{
		if (d2d)
			delete d2d;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// Drawing ////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void GraphicsComponent2D::beginDraw() const
	{
		d2d->beginDraw();
	}
	util::Expected<void> GraphicsComponent2D::endDraw() const
	{
		return d2d->endDraw();
	}

	// draw line
	void GraphicsComponent2D::drawLine(const D2D1_POINT_2F& startPoint, const D2D1_POINT_2F& endPoint, const float width, const float opacity, ID2D1Brush* const brush) const
	{
		if (brush)
		{
			brush->SetOpacity(opacity);
			d2d->devCon->DrawLine(startPoint, endPoint, brush, width);
		}
		else
		{
			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(opacity);
			d2d->devCon->DrawLine(startPoint, endPoint, d2d->blackBrush.Get(), width);
			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(1.0f);
		}
	}

	// draw and fill rectangles
	void GraphicsComponent2D::fillRectangle(const float ulX, const float ulY, const float lrX, const float lrY, const float opacity, ID2D1Brush* const brush) const
	{
		D2D1_RECT_F rect = { ulX, ulY, lrX, lrY };
		if (brush)
		{
			brush->SetOpacity(opacity);
			d2d->devCon->FillRectangle(&rect, brush);
		}
		else
		{
			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(opacity);

			d2d->devCon->FillRectangle(&rect, d2d->blackBrush.Get());

			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(1.0f);
		}
	}
	void GraphicsComponent2D::fillRectangle(const D2D1_POINT_2F& upperLeft, const D2D1_POINT_2F& lowerRight, const float opacity, ID2D1Brush* const brush) const
	{
		D2D1_RECT_F rect = { upperLeft.x , upperLeft.y , lowerRight.x , lowerRight.y };
		if (brush)
		{
			brush->SetOpacity(opacity);
			d2d->devCon->FillRectangle(&rect, brush);
		}
		else
		{
			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(opacity);

			d2d->devCon->FillRectangle(&rect, d2d->blackBrush.Get());

			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(1.0f);
		}
	}
	void GraphicsComponent2D::drawRectangle(const float ulX, const float ulY, const float lrX, const float lrY, ID2D1Brush* const brush, const float width, ID2D1StrokeStyle1* const strokeStyle) const
	{
		D2D1_RECT_F rect = { ulX, ulY, lrX, lrY };
		if (brush)
			d2d->devCon->DrawRectangle(&rect, brush, width, strokeStyle);
		else
			d2d->devCon->DrawRectangle(&rect, d2d->blackBrush.Get(), width, strokeStyle);
	}
	void GraphicsComponent2D::drawRectangle(const D2D1_POINT_2F& upperLeft, const D2D1_POINT_2F& lowerRight, ID2D1Brush* const brush, const float width, ID2D1StrokeStyle1* const strokeStyle) const
	{
		D2D1_RECT_F rect = { upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y };
		if (brush)
			d2d->devCon->DrawRectangle(&rect, brush, width, strokeStyle);
		else
			d2d->devCon->DrawRectangle(&rect, d2d->blackBrush.Get(), width, strokeStyle);
	}

	// fill and draw rounded rectangles
	void GraphicsComponent2D::fillRoundedRectangle(const float ulX, const float ulY, const float lrX, const float lrY, const float radiusX, const float radiusY, const float opacity, ID2D1Brush* const brush) const
	{
		D2D1_RECT_F rect = { ulX, ulY, lrX, lrY };
		if (brush)
		{
			brush->SetOpacity(opacity);
			d2d->devCon->FillRoundedRectangle(D2D1::RoundedRect(rect, radiusX, radiusY), brush);
		}
		else
		{
			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(opacity);

			d2d->devCon->FillRoundedRectangle(D2D1::RoundedRect(rect, radiusX, radiusY), d2d->blackBrush.Get());

			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(1.0f);
		}
	}
	void GraphicsComponent2D::fillRoundedRectangle(const D2D1_POINT_2F& upperLeft, const D2D1_POINT_2F& lowerRight, const float radiusX, const float radiusY, const float opacity, ID2D1Brush* const brush) const
	{
		D2D1_RECT_F rect = { upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y };

		if (brush)
		{
			brush->SetOpacity(opacity);
			d2d->devCon->FillRoundedRectangle(D2D1::RoundedRect(rect, radiusX, radiusY), brush);
		}
		else
		{
			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(opacity);

			d2d->devCon->FillRoundedRectangle(D2D1::RoundedRect(rect, radiusX, radiusY), d2d->blackBrush.Get());

			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(opacity);
		}
	}
	void GraphicsComponent2D::drawRoundedRectangle(const float ulX, const float ulY, const float lrX, const float lrY, const float radiusX, const float radiusY, ID2D1Brush* const brush, const float width, ID2D1StrokeStyle1* const strokeStyle) const
	{
		D2D1_RECT_F rect = { ulX, ulY, lrX, lrY };
		if (brush)
			d2d->devCon->DrawRoundedRectangle(D2D1::RoundedRect(rect, radiusX, radiusY), brush, width, strokeStyle);
		else
			d2d->devCon->DrawRoundedRectangle(D2D1::RoundedRect(rect, radiusX, radiusY), d2d->blackBrush.Get(), width, strokeStyle);
	}
	void GraphicsComponent2D::drawRoundedRectangle(const D2D1_POINT_2F& upperLeft, const D2D1_POINT_2F& lowerRight, const float radiusX, const float radiusY, ID2D1Brush* const brush, const float width, ID2D1StrokeStyle1* const strokeStyle) const
	{
		D2D1_RECT_F rect = { upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y };
		if (brush)
			d2d->devCon->DrawRoundedRectangle(D2D1::RoundedRect(rect, radiusX, radiusY), brush, width, strokeStyle);
		else
			d2d->devCon->DrawRoundedRectangle(D2D1::RoundedRect(rect, radiusX, radiusY), d2d->blackBrush.Get(), width, strokeStyle);
	}

	// fill and draw ellipses
	void GraphicsComponent2D::fillEllipse(const float centreX, const float centreY, const float radiusX, const float radiusY, const float opacity, ID2D1Brush* const brush) const
	{
		if (brush)
		{
			brush->SetOpacity(opacity);
			d2d->devCon->FillEllipse(D2D1::Ellipse(D2D1::Point2F(centreX, centreY), radiusX, radiusY), brush);
		}
		else
		{
			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(opacity);
			d2d->devCon->FillEllipse(D2D1::Ellipse(D2D1::Point2F(centreX, centreY), radiusX, radiusY), d2d->blackBrush.Get());

			if (opacity != 1.0f)
				d2d->blackBrush->SetOpacity(1.0f);
		}
	}
	void GraphicsComponent2D::drawEllipse(const float centreX, const float centreY, const float radiusX, const float radiusY, ID2D1Brush* const brush, const float width, ID2D1StrokeStyle1* const strokeStyle) const
	{
		if (brush)
			d2d->devCon->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(centreX, centreY), radiusX, radiusY), brush, width, strokeStyle);
		else
			d2d->devCon->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(centreX, centreY), radiusX, radiusY), d2d->blackBrush.Get(), width, strokeStyle);
	}

	util::Expected<void> GraphicsComponent2D::createPolygon(D2D1_POINT_2F* startingPoint, D2D1_POINT_2F vertices[], unsigned int size, Microsoft::WRL::ComPtr<ID2D1PathGeometry>* polygon) const
	{
		// create the path geometry
		if (FAILED(d2d->factory->CreatePathGeometry(polygon->ReleaseAndGetAddressOf())))
			return std::runtime_error("Critical error: Unable to create the path geometry for the polygon!");
		
		// open a sink
		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
		if (FAILED(polygon->Get()->Open(sink.GetAddressOf())))
			return std::runtime_error("Critical error: Unable to open the sink of the polygon!");
		
		// set the fill mode
		sink->SetFillMode(D2D1_FILL_MODE_WINDING);

		// fill the sink
		sink->BeginFigure(*startingPoint, D2D1_FIGURE_BEGIN_FILLED);

		// combine the points
		for (unsigned int i = 0; i < size; i++)
			vertices[i];

		sink->AddLines(vertices, size);
		
		// end figure
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);

		// close the sink
		if (FAILED(sink->Close()))
			return std::runtime_error("Critical error: Failed to close the sink!");

		// return success
		return { };
	}

	void GraphicsComponent2D::drawPolygon(ID2D1PathGeometry* polygon) const
	{
		if (polygon != nullptr)
			d2d->devCon->DrawGeometry(polygon, d2d->blackBrush.Get(), 5);
	}

	void GraphicsComponent2D::fillPolygon(Microsoft::WRL::ComPtr<ID2D1PathGeometry>* polygon) const
	{
		d2d->devCon->FillGeometry(polygon->Get(), d2d->blackBrush.Get());
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Brushes and Strokes //////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	util::Expected<void> GraphicsComponent2D::createSolidColourBrush(const D2D1::ColorF& colour, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& brush) const
	{
		if (FAILED(d2d->devCon->CreateSolidColorBrush(colour, brush.ReleaseAndGetAddressOf())))
			return std::runtime_error("Critical error: Unable to create brush!");

		// return success
		return {};
	}
	util::Expected<void> GraphicsComponent2D::createLinearGradientBrush(const float startX, const float startY, const float endX, const float endY, const D2D1_GRADIENT_STOP* const stops, const unsigned int stopsSize, Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush>& linearGradientBrush) const
	{
		// create the stop collection
		Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> stopCollection;
		D2D1_GRADIENT_STOP* gradientStops = new D2D1_GRADIENT_STOP[stopsSize];
		for (unsigned int i = 0; i < stopsSize; i++)
			gradientStops[i] = stops[i];
		//memcpy(gradientStops, stops, sizeof(D2D1_GRADIENT_STOP)*stopsSize);

		if (FAILED(d2d->devCon->CreateGradientStopCollection(gradientStops, stopsSize, stopCollection.GetAddressOf())))
			return std::runtime_error("Critical error: Unable to create the gradient stop collection!");

		// create the brush
		D2D1_POINT_2F startPoint = D2D1::Point2F(startX, startY);
		D2D1_POINT_2F endPoint = D2D1::Point2F(endX, endY);
		if (FAILED(d2d->devCon->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(startPoint, endPoint), stopCollection.Get(), linearGradientBrush.ReleaseAndGetAddressOf())))
			return std::runtime_error("Critical error: Unable to create the linear gradient brush!");

		// return success
		return {};
	}
	util::Expected<void> GraphicsComponent2D::createRadialGradientBrush(const float centreX, const float centreY, const float offsetX, const float offsetY, const float radiusX, const float radiusY, ID2D1GradientStopCollection& stopCollection, Microsoft::WRL::ComPtr<ID2D1RadialGradientBrush>& radialGradientBrush) const
	{
		D2D1_POINT_2F centre = D2D1::Point2F(centreX, centreY);
		D2D1_POINT_2F offset = D2D1::Point2F(offsetX, offsetY);
		if (FAILED(d2d->devCon->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(centre, offset, radiusX, radiusY), &stopCollection, radialGradientBrush.ReleaseAndGetAddressOf())))
			return std::runtime_error("Critical error: Unable to create the radial gradient brush!");

		// return success
		return {};
	}
	util::Expected<void> GraphicsComponent2D::createStrokeStyle(D2D1_STROKE_STYLE_PROPERTIES1& strokeProperties, Microsoft::WRL::ComPtr<ID2D1StrokeStyle1>& stroke) const
	{
		if (FAILED(d2d->factory->CreateStrokeStyle(strokeProperties, nullptr, 0, stroke.GetAddressOf())))
			return std::runtime_error("Critical error: Unable to create stroke style!");

		// return success
		return {};
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// Transformations ////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void GraphicsComponent2D::reflectX(float x, float y) const
	{
		D2D1::Matrix3x2F trans1 = D2D1::Matrix3x2F::Translation(-x, -y);
		D2D1::Matrix3x2F trans2 = D2D1::Matrix3x2F::Translation(x, y);

		// rotate
		D2D1::Matrix3x2F m;
		m._11 = 1; m._12 = 0;
		m._21 = 0; m._22 = -1;
		m._31 = 0; m._32 = 1;

		D2D1::Matrix3x2F transformation = trans1 * m * trans2;
		setTransformation(transformation);
	}
	void GraphicsComponent2D::reflectY(float x, float y) const
	{
		D2D1::Matrix3x2F trans1 = D2D1::Matrix3x2F::Translation(-x, -y);
		D2D1::Matrix3x2F trans2 = D2D1::Matrix3x2F::Translation(x, y);

		// rotate
		D2D1::Matrix3x2F m;
		m._11 = -1; m._12 = 0;
		m._21 = 0; m._22 = 1;
		m._31 = 0; m._32 = 1;

		D2D1::Matrix3x2F transformation = trans1 * m * trans2;
		setTransformation(transformation);
	}

	void GraphicsComponent2D::setTransformationCentralScale(const float scaleFactorX, const float scaleFactorY) const
	{
		// get middle of the screen
		float x = d2d->getCurrentWidth() * 0.5f;
		float y = d2d->getCurrentHeight() * 0.5f;
		D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(scaleFactorX, scaleFactorY, D2D1::Point2F(x,y));

		d2d->devCon->SetTransform(scaleMatrix);
	}

	void GraphicsComponent2D::setRotationMatrix(const float angle, const float rotX, const float rotY) const
	{
		d2d->devCon->SetTransform(D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(rotX, rotY)));
	}

	void GraphicsComponent2D::setTransformation(const D2D_MATRIX_3X2_F& transMatrix) const
	{
		d2d->devCon->SetTransform(transMatrix);
	}
	void GraphicsComponent2D::resetTransformation() const
	{
		d2d->devCon->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	void GraphicsComponent2D::setTransformation90CW() const
	{
		d2d->devCon->SetTransform(matrixRotation90CW);
	}
	void GraphicsComponent2D::setTransformation180CW() const
	{
		d2d->devCon->SetTransform(matrixRotation180CW);
	}
	void GraphicsComponent2D::setTransformation270CW() const
	{
		d2d->devCon->SetTransform(matrixRotation270CW);
	}
	void GraphicsComponent2D::setTransformation90CCW() const
	{
		d2d->devCon->SetTransform(matrixRotation90CCW);
	}
	void GraphicsComponent2D::setTransformation180CCW() const
	{
		d2d->devCon->SetTransform(matrixRotation180CCW);
	}
	void GraphicsComponent2D::setTransformation270CCW() const
	{
		d2d->devCon->SetTransform(matrixRotation270CCW);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// Getters /////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	ID2D1SolidColorBrush& GraphicsComponent2D::getBrush(const std::wstring& color) const
	{
		 std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> >::const_iterator it = d2d->brushes.find (color);

		 if (it == d2d->brushes.end())
			 // a brush with this colour does not exist, return the black brush
			 return d2d->getBlackBrush();
		else
			return *d2d->brushes[color].Get();
	}
}