#include "vectors.h"
#include "log.h"
#include "serviceLocator.h"

#include <math.h>

namespace mathematics
{
	namespace linearAlgebra
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////// Reflections ////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Vector2F reflectionVector(const Vector2F incidence, const Vector2F normal)
		{
			// compute projection: b(-I,n)n
			float coef = -2 * scalarProduct2F(incidence, normal);

			// return the vector of reflection: r = -2b(I,n)n+I
			return normal * coef + incidence;
		}

		void reflectionVector(Vector2F* incidence, Vector2F normal)
		{
			// compute projection: b(-I,n)n
			float coef = -2 * scalarProduct2F(*incidence, normal);

			// return the vector of reflection: r = -2b(I,n)n+I
			*incidence += normal * coef;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////// Scalar Product ///////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		float scalarProduct2F(const Vector2F x, const Vector2F y)
		{
			// standard scalar product
			return x.x * y.x + x.y * y.y;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Cross Product ///////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		float crossProduct2F(const Vector2F x, const Vector2F y)
		{
			// standard cross product
			return x.x * y.y - x.y * y.x;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////// Lengths //////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////

		// square length
		float Vector2F::getSquareLength()
		{
			// if the length was already compute, return it
			squareLength = x * x + y * y;
			return squareLength;
		}
		float Vector2F::getSquareLength() const
		{
			// if the length was already compute, return it
			return x * x + y * y;
		}

		// length
		float Vector2F::getLength()
		{
			length = sqrtf(x * x + y * y);
			return length;
		}
		float Vector2F::getLength() const
		{
			return sqrtf(x * x + y * y);
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////// Normalize ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		void Vector2F::normalize(float l)
		{
			if (l == -1)
			{
				// no length provided -> compute it
				l = getLength();
			}

			// normalize vector
			x = x / l;
			y = y / l;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////// Operators ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Vector2F& Vector2F::operator*=(const float a)
		{
			x *= a;
			y *= a;

			return *this;
		}

		Vector2F Vector2F::operator*(const float a)
		{
			Vector2F temp;
			temp.x = x * a;
			temp.y = y * a;

			return temp;
		}

		Vector2F& Vector2F::operator*=(const double a)
		{
			x *= (float)a;
			y *= (float)a;

			return *this;
		}

		Vector2F Vector2F::operator*(const double a)
		{
			Vector2F temp;
			temp.x = (float)(x * a);
			temp.y = (float)(y * a);

			return temp;
		}

		Vector2F Vector2F::operator*(const double a) const
		{
			Vector2F temp;
			temp.x = (float)(x * a);
			temp.y = (float)(y * a);

			return temp;
		}

		Vector2F Vector2F::operator+(const Vector2F& v)
		{
			Vector2F temp;
			temp.x = x + v.x;
			temp.y = y + v.y;

			return temp;
		}

		Vector2F Vector2F::operator+(const Vector2F& v) const
		{
			Vector2F temp;
			temp.x = x + v.x;
			temp.y = y + v.y;

			return temp;
		}

		Vector2F& Vector2F::operator+=(const Vector2F& v)
		{
			x += v.x;
			y += v.y;

			return *this;
		}

		Vector2F& Vector2F::operator+=(const float f)
		{
			x += f;
			y += f;

			return *this;
		}

		Vector2F Vector2F::operator-(const Vector2F& v)
		{
			Vector2F temp;
			temp.x = x - v.x;
			temp.y = y - v.y;

			return temp;
		}

		Vector2F Vector2F::operator-(const Vector2F& v) const
		{
			Vector2F temp;
			temp.x = x - v.x;
			temp.y = y - v.y;

			return temp;
		}

		Vector2F& Vector2F::operator-=(const Vector2F& v)
		{
			x -= v.x;
			y -= v.y;

			return *this;
		}

		Vector2F& Vector2F::operator-=(const float f)
		{
			x -= f;
			y -= f;

			return *this;
		}
	}
}