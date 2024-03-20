#include "geometry.h"

// math includes
#define _USE_MATH_DEFINES
#include <math.h>


namespace mathematics
{
	namespace geometry
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Helper Functions ////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		void computeCoordinatesOnEllipse(const mathematics::linearAlgebra::Vector2F& ellipsePoint, const mathematics::linearAlgebra::Vector2F& ellipseRadius, const float angle, mathematics::linearAlgebra::Vector2F& point)
		{
			// the x and y-coordinates can be computed by circular functions
			point = { ellipsePoint.x + ellipseRadius.x * cosf(angle * (float)M_PI / 180) , ellipsePoint.y + ellipseRadius.y * sinf(angle * (float)M_PI / 180) };
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Angles //////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Distance ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		float distance2D(const mathematics::linearAlgebra::Vector2F& A, const mathematics::linearAlgebra::Vector2F& B)
		{
			float dx = B.x - A.x;
			float dy = B.y - A.y;

			return sqrtf(dx * dx - dy * dy);
		}

		float squareDistance2D(const mathematics::linearAlgebra::Vector2F& A, const mathematics::linearAlgebra::Vector2F& B)
		{
			float dx = B.x - A.x;
			float dy = B.y - A.y;

			return dx * dx - dy * dy;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Lines ///////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Line2D::Line2D() : normal(mathematics::linearAlgebra::Vector2F(0, 1)), d(0) {};
		Line2D::Line2D(const mathematics::linearAlgebra::Vector2F& n, const float dist) : normal(n), d(dist) { };

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Line Segments ///////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		LineSegment2D::LineSegment2D(mathematics::linearAlgebra::Vector2F startPoint, mathematics::linearAlgebra::Vector2F endPoint, mathematics::linearAlgebra::Vector2F normalVector) : startPoint(startPoint), endPoint(endPoint), normalVector(normalVector)
		{
			this->directionVector = endPoint - startPoint;

			// normal vector: (-dy,dx)
			this->normalVector = mathematics::linearAlgebra::Vector2F(-directionVector.y, directionVector.x);
			this->normalVector.normalize();
		}

		LineSegment2D::LineSegment2D(mathematics::linearAlgebra::Vector2F startPoint, mathematics::linearAlgebra::Vector2F endPoint) : startPoint(startPoint), endPoint(endPoint)
		{
			this->directionVector = endPoint - startPoint;

			// normal vector: (-dy,dx)
			this->normalVector = mathematics::linearAlgebra::Vector2F(-directionVector.y, directionVector.x);
			this->normalVector.normalize();
		}

		bool segmentIntersection2D(const LineSegment2D& segment1, const LineSegment2D& segment2, mathematics::linearAlgebra::Vector2F* t)
		{
			// compute the determinant of the 2x2 matrix
			float det = segment1.directionVector.x * segment2.directionVector.y - segment1.directionVector.y * segment2.directionVector.x;

			// if det is 0, the directional vectors are colinear
			if (det == 0)
				return false;

			// compute directional vector between the two starting points
			mathematics::linearAlgebra::Vector2F directionalVector = mathematics::linearAlgebra::Vector2F(segment2.startPoint.x - segment1.startPoint.x, segment2.startPoint.y - segment1.startPoint.y);

			// compute t1
			float t1 = segment2.directionVector.y * directionalVector.x - segment2.directionVector.x * directionalVector.y;
			t1 /= det;

			// if t1 is not between 0 and 1, the segments can't intersect
			if (0 > t1 || t1 > 1)
				return false;

			// compute t2
			float t2 = -segment1.directionVector.y * directionalVector.x + segment1.directionVector.x * directionalVector.y;
			t2 /= det;

			// if t2 is not between 0 and 1, the segments can't intersect
			if (0 > t2 || t2 > 1)
				return false;

			// else return true
			if (t != NULL)
			{
				t->x = t1;
				t->y = t2;
			}
			return true;
		}

		// collision detection between two lines - uses scalar product, works in higher dimensions
		bool intersection(const LineSegment2D& ls, const Line2D& l, mathematics::linearAlgebra::Vector2F* const intersectionPoint)
		{
			// check for colinearity of the normal vector of the line (ls2) and the direction vector of the line segment (ls1)
			float sp = mathematics::linearAlgebra::scalarProduct2F(ls.directionVector, l.normal);

			if (sp != 0)
			{
				// not colinear
				float t = -1 * mathematics::linearAlgebra::scalarProduct2F(ls.startPoint, l.normal) + l.d;
				t /= sp;

				// if t is between 0 and 1, there was a collision
				if (t >= 0 && t <= 1)
				{
					// there was an intersection - compute the intersection point if desires
					if (intersectionPoint != nullptr)
						* intersectionPoint = ls.startPoint + ls.directionVector * t;

					return true;
				}
				else
					return false;
			}
			else
			{
				// colinearity - check whether the start point of the vector lies on the line
				if (mathematics::linearAlgebra::scalarProduct2F(ls.startPoint, l.normal) + l.d == 0)
				{
					if (intersectionPoint != nullptr)
						* intersectionPoint = ls.startPoint;
					return true;
				}
				else
					return false;
			}
		}

		// collision detection between two line segments
		bool intersection(const LineSegment2D& ls1, const LineSegment2D& ls2, mathematics::linearAlgebra::Vector2F* const intersectionPoint)
		{
			// compute the determinant of the 2x2 matrix
			float det = ls1.directionVector.x * ls2.directionVector.y - ls1.directionVector.y * ls2.directionVector.x;

			// if det is 0, the directional vectors are colinear
			if (det == 0)
				return false;
				
			// compute directional vector between the two starting points
			mathematics::linearAlgebra::Vector2F directionalVector = mathematics::linearAlgebra::Vector2F(ls2.startPoint.x - ls1.startPoint.x, ls2.startPoint.y - ls1.startPoint.y);

			// compute t1
			float t1 = ls2.directionVector.y * directionalVector.x - ls2.directionVector.x * directionalVector.y;
			t1 /= det;

			// if t1 is not between 0 and 1, the segments can't intersect
			if (0 > t1 || t1 > 1)
				return false;

			//// compute t2
			//float t2 = -ls1.directionVector.y * directionalVector.x + ls1.directionVector.x * directionalVector.y;
			//t2 /= det;

			//// if t2 is not between 0 and 1, the segments can't intersect
			//if (0 > t2 || t2 > 1)
			//	return false;

			// else return true
			if (intersectionPoint != nullptr)
				*intersectionPoint = ls1.startPoint + ls1.directionVector * t1;
			return true;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Rays ////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Ray2D::Ray2D() : startPoint(mathematics::linearAlgebra::Vector2F(0, 0)), direction(mathematics::linearAlgebra::Vector2F(1, 0)) {};
		Ray2D::Ray2D(const mathematics::linearAlgebra::Vector2F& start, const mathematics::linearAlgebra::Vector2F& dir) : startPoint(start), direction(dir) {};
		mathematics::linearAlgebra::Vector2F Ray2D::ride(const float t) const
		{
			mathematics::linearAlgebra::Vector2F s, d;
			s = startPoint;
			d = direction;
			return s + d * t;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Spheres /////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Sphere2D::Sphere2D() : center(mathematics::linearAlgebra::Vector2F(0, 0)), radius(1.0f) {};
		Sphere2D::Sphere2D(const mathematics::linearAlgebra::Vector2F& c, const float r) : center(c), radius(r) {};

		mathematics::linearAlgebra::Vector2F Sphere2D::point(const float angle)
		{
			float x, y;
			x = center.x + radius * cosf(angle);
			y = center.y + radius * sinf(angle);

			return mathematics::linearAlgebra::Vector2F(x, y);
		}

		// collision detection - use square distance test
		bool intersection(const Sphere2D& s1, const Sphere2D& s2)
		{
			// get the vector joining the two center points
			mathematics::linearAlgebra::Vector2F centerVector = s2.center - s1.center;

			// compute the squared length of the vector joining the two center points
			float distanceSquared = mathematics::linearAlgebra::scalarProduct2F(centerVector, centerVector);

			// is the distance between the center points less than the square of the sum of the radii? If so, the spheres intersection
			if (distanceSquared < ((s1.radius + s2.radius) * (s1.radius + s2.radius)))
				return true;
			else
				return false;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Rectangles //////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Rectangle2D::Rectangle2D() : upperLeft(mathematics::linearAlgebra::Vector2F(0.0f, 0.0f)), lowerRight(mathematics::linearAlgebra::Vector2F(1.0f, 1.0f)) {};
		Rectangle2D::Rectangle2D(const mathematics::linearAlgebra::Vector2F& ul, const mathematics::linearAlgebra::Vector2F& lr) : upperLeft(ul), lowerRight(lr) {};

		// collision detection - checks for four cases where the rectangles definetely can't intersect
		bool intersection(const Rectangle2D& r1, const Rectangle2D& r2)
		{
			bool collision = (r1.lowerRight.x < r2.upperLeft.x || r2.lowerRight.x < r1.upperLeft.x || r2.lowerRight.y < r1.upperLeft.y || r1.lowerRight.y < r2.upperLeft.y);
			return !collision;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Capsules ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Capsule2D::Capsule2D() : lineSegment(LineSegment2D(mathematics::linearAlgebra::Vector2F(0, 0), mathematics::linearAlgebra::Vector2F(0, 1))), radius(0.5f) {};
		Capsule2D::Capsule2D(const LineSegment2D& ls, const float r) : lineSegment(ls), radius(r) {};
		Capsule2D::Capsule2D(const mathematics::linearAlgebra::Vector2F& sp, const mathematics::linearAlgebra::Vector2F& ep, const float r) : lineSegment(LineSegment2D(sp, ep)), radius(r) {};

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// Polygons ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Polygon2D::Polygon2D() : vertices(), centroid(mathematics::linearAlgebra::Vector2F(0, 0)), normal(0.0f) {};
		Polygon2D::Polygon2D(const std::vector<mathematics::linearAlgebra::Vector2F>& v, const mathematics::linearAlgebra::Vector2F& c) : vertices(v), centroid(c) 
		{
			this->normal = mathematics::linearAlgebra::crossProduct2F(v[1] - v[0], v[2] - v[1]);
		};

		Polygon2D::Polygon2D(const std::vector<mathematics::linearAlgebra::Vector2F>& v) : vertices(v)
		{
			computeCentroid(vertices, &centroid);
			this->normal = mathematics::linearAlgebra::crossProduct2F(v[1] - v[0], v[2] - v[1]);
		}

		void computeCentroid(const std::vector<mathematics::linearAlgebra::Vector2F>& vertices, mathematics::linearAlgebra::Vector2F* centroid)
		{
			// the vertices should be given in counter-clockwise order to avoid negative determinants
			float centroidX = 0, centroidY = 0;
			float det = 0, tempDet = 0;
			unsigned int j = 0;
			unsigned int nVertices = (unsigned int)vertices.size();

			for (unsigned int i = 0; i < nVertices; i++)
			{
				// closed polygon
				if (i + 1 == nVertices)
					j = 0;
				else
					j = i + 1;

				// compute the determinant
				tempDet = vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
				det += tempDet;

				centroidX += (vertices[i].x + vertices[j].x) * tempDet;
				centroidY += (vertices[i].y + vertices[j].y) * tempDet;
			}

			// divide by the total mass of the polygon
			centroidX /= 3 * det;
			centroidY /= 3 * det;

			centroid->x = centroidX;
			centroid->y = centroidY;
		}

		void computeBoundingBox(const std::vector<mathematics::linearAlgebra::Vector2F>& vertices, mathematics::linearAlgebra::Vector2F* leftTop, mathematics::linearAlgebra::Vector2F* rightBottom)
		{
			// this algorithm works with local coordinates
			float minX = 0, maxX = 0, minY = 0, maxY = 0;

			// loop through each vertex
			for (std::vector<mathematics::linearAlgebra::Vector2F>::const_iterator it = vertices.begin(); it != vertices.end(); it++)
			{
				if ((*it).x > maxX)
					maxX = (*it).x;

				if ((*it).y > maxY)
					maxY = (*it).y;

				if ((*it).x < minX)
					minX = (*it).x;

				if ((*it).y < minY)
					minY = (*it).y;
			}

			leftTop->x=minX;
			leftTop->y = minY;
			rightBottom->x = maxX;
			rightBottom->y = maxY;
		}

		bool Polygon2D::contains(const mathematics::linearAlgebra::Vector2F& p) const
		{
			// remember that this function only works if the vertices are in clockwise winding order
			
			// temps: side: side of the polygon; toPoint: vector from a vertex to the point p; cp: cross product
			mathematics::linearAlgebra::Vector2F side, toPoint;
			float cp;

			for (int i = 1; i < this->vertices.size(); i++)
			{
				// get vector from the previous vertex to the current vertex
				side = vertices[i] - vertices[i-1];

				// get vector from the previous vertex to the actual point p
				toPoint = p - vertices[i - 1];

				// compute the cross product of both vectors
				cp = mathematics::linearAlgebra::crossProduct2F(side, toPoint);


			}

		}
	}
}