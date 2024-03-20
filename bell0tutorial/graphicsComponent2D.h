#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		27/06/2018 - Lenningen - Luxembourg
*
* Desc:		2D graphics components
*
* Hist:		- 28/06/2018: the "compute point on ellipse" method is now in a maths class
*			- 05/04/2019: added function to draw lines
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// DirectX includes
#include <d2d1_3.h>

// Windows includes
#include <wrl.h>

// DEFINITIONS //////////////////////////////////////////////////////////////////////////

namespace util
{
	template<typename T>
	class Expected;
}

namespace core
{
	class DirectXApp;
}

namespace graphics
{
	class Direct2D;
	class Direct3D;

	class GraphicsComponent2D
	{
	private:
		Direct2D* d2d;

		// useful fixed rotations
		const D2D1::Matrix3x2F matrixRotation90CW;				// 90 degrees clockwise rotation 
		const D2D1::Matrix3x2F matrixRotation180CW;				// 180 degrees clockwise rotation
		const D2D1::Matrix3x2F matrixRotation270CW;				// 270 degrees clockwise rotation
		const D2D1::Matrix3x2F matrixRotation90CCW;				// 90 degrees counterclockwise rotation
		const D2D1::Matrix3x2F matrixRotation180CCW;			// 180 degrees counterclockwise rotation
		const D2D1::Matrix3x2F matrixRotation270CCW;			// 270 degrees counterclockwise rotation

	public:
		// constructor and destructor
		GraphicsComponent2D(const core::DirectXApp& dxApp, const Direct3D& d3d);
		~GraphicsComponent2D();

		// begin and end drawing
		void beginDraw() const;
		util::Expected<void> endDraw() const;

		// draw line
		void drawLine(const D2D1_POINT_2F& startPoint, const D2D1_POINT_2F& endPoint, const float width = 1.0f, const float opacity = 1.0f, ID2D1Brush* const brush = NULL) const;

		// draw and fill rectangles
		void fillRectangle(const float ulX, const float ulY, const float lrX, const float lrY, const float opacity = 1.0f, ID2D1Brush* const brush = NULL) const;
		void fillRectangle(const D2D1_POINT_2F& upperLeft, const D2D1_POINT_2F& lowerRight, const float opacity = 1.0f, ID2D1Brush* const brush = NULL) const;
		void drawRectangle(const float ulX, const float ulY, const float lrX, const float lrY, ID2D1Brush* const brush = NULL, const float width = 1.0f, ID2D1StrokeStyle1* const strokeStyle = NULL) const;
		void drawRectangle(const D2D1_POINT_2F& upperLeft, const D2D1_POINT_2F& lowerRight, ID2D1Brush* const brush = NULL, const float width = 1.0f, ID2D1StrokeStyle1* const strokeStyle = NULL) const;

		// draw and fill rounded rectangles
		void fillRoundedRectangle(const float ulX, const float ulY, const float lrX, const float lrY, const float radiusX, const float radiusY, const float opacity = 1.0f, ID2D1Brush* const brush = NULL) const;
		void fillRoundedRectangle(const D2D1_POINT_2F& upperLeft, const D2D1_POINT_2F& lowerRight, const float radiusX, const float radiusY, const float opacity = 1.0f, ID2D1Brush* const brush = NULL) const;
		void drawRoundedRectangle(const float ulX, const float ulY, const float lrX, const float lrY, const float radiusX, const float radiusY, ID2D1Brush* const brush = NULL, const float width = 1.0f, ID2D1StrokeStyle1* const strokeStyle = NULL) const;
		void drawRoundedRectangle(const D2D1_POINT_2F& upperLeft, const D2D1_POINT_2F& lowerRight, const float radiusX, const float radiusY, ID2D1Brush* const brush = NULL, const float width = 1.0f, ID2D1StrokeStyle1* const strokeStyle = NULL) const;

		// draw and fill ellipses
		void fillEllipse(const float centreX, const float centreY, const float radiusX, const float radiusY, const float opacity = 1.0f, ID2D1Brush* const brush = NULL) const;
		void drawEllipse(const float centreX, const float centreY, const float radiusX, const float radiusY, ID2D1Brush* const brush = NULL, const float width = 1.0f, ID2D1StrokeStyle1* const strokeStyle = NULL) const;

		// draw and fill polygons
		util::Expected<void> createPolygon(D2D1_POINT_2F* startingPoint, D2D1_POINT_2F vertices[], unsigned int size, Microsoft::WRL::ComPtr<ID2D1PathGeometry>* polygon) const;
		void drawPolygon(ID2D1PathGeometry* polygon) const;
		void fillPolygon(Microsoft::WRL::ComPtr< ID2D1PathGeometry>* polygon) const;

		// brushes and strokes
		util::Expected<void> createSolidColourBrush(const D2D1::ColorF& colour, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>& brush) const;			// creates a solid colour brush
		util::Expected<void> createLinearGradientBrush(const float startX, const float startY, const float endX, const float endY, const D2D1_GRADIENT_STOP* const stops, unsigned int stopsSize, Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush>& linearGradientBrush) const;
		util::Expected<void> createRadialGradientBrush(const float centreX, const float centreY, const float offsetX, const float offsetY, const float radiusX, const float radiusY, ID2D1GradientStopCollection& stopCollection, Microsoft::WRL::ComPtr<ID2D1RadialGradientBrush>& radialGradientBrush) const;
		util::Expected<void> createStrokeStyle(D2D1_STROKE_STYLE_PROPERTIES1& strokeProperties, Microsoft::WRL::ComPtr<ID2D1StrokeStyle1>& stroke) const;// creates a stroke
		ID2D1SolidColorBrush& getBrush(const std::wstring&) const;	


		// transformations
		void setRotationMatrix(const float angle, const float rotX, const float rotY) const;
		void setTransformation90CW() const;
		void setTransformation180CW() const;
		void setTransformation270CW() const;
		void setTransformation90CCW() const;
		void setTransformation180CCW() const;
		void setTransformation270CCW() const;
		void setTransformationCentralScale(const float scaleFactorX, const float scaleFactorY) const;
		void setTransformation(const D2D_MATRIX_3X2_F& transMatrix) const;
		void resetTransformation() const;
		void reflectX(float x, float y) const;
		void reflectY(float x, float y) const;

		friend class core::DirectXApp;
		friend class GraphicsComponent;
		friend class GraphicsComponent3D;
	};
}