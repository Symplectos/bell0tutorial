#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		24/06/2018 - Lenningen - Luxembourg
*
* Desc:		mathematical vectors
*
* History:
*
* ToDo:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// CLASSES //////////////////////////////////////////////////////////////////////////////

namespace mathematics
{
	namespace linearAlgebra
	{
		class Vector2F
		{
		public:
			float x, y;
			float squareLength;
			float length;

			// constructor and destructor
			Vector2F() : x(0.0f), y(0.0f), length(0.0f), squareLength(0.0f) {};
			Vector2F(float x, float y) : x(x), y(y) { squareLength = getSquareLength(); };
			~Vector2F() {};

			// get square length
			float getSquareLength();
			float getSquareLength() const;

			// get length
			float getLength();
			float getLength() const;

			// normalize vector
			void normalize(float l = -1.0f);

			// overload operators
			Vector2F& operator*=(const float);
			Vector2F operator*(const float);

			Vector2F& operator*=(const double);
			Vector2F operator*(const double);
			Vector2F operator*(const double) const;

			Vector2F operator+(const Vector2F&);
			Vector2F operator+(const Vector2F&) const;
			Vector2F& operator+=(const Vector2F&);
			Vector2F& operator+=(const float);

			Vector2F operator-(const Vector2F&);
			Vector2F operator-(const Vector2F&) const;
			Vector2F& operator-=(const Vector2F&);
			Vector2F& operator-=(const float);
		};

		// scalar products
		float scalarProduct2F(const Vector2F x, const Vector2F y);			// computes the standard euclidean scalar product of the vector x and y

		// cross products
		float crossProduct2F(const Vector2F x, const Vector2F y);			// computes the cross product of two vectors

		// reflections
		Vector2F reflectionVector(const Vector2F v, const Vector2F n);		// given a vector v and a normal vector n, this function computes the reflection of v to n
		void reflectionVector(Vector2F* v, Vector2F n);						// given a vector v and a normal vector n, this function changes v to be its own reflection to n
	}
}