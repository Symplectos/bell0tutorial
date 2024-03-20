#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		27/06/2018 - Lenningen - Luxembourg
*
* Desc:		geometry
*
* History:	- 20/03/2019: computation of the centroid of convex polygons
*			- 29/07/2019: basic geometrical objects for collision detection
*
* ToDo:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// C++
#include <vector>

// bell0bytes mathematics
#include "vectors.h"

// CLASSES //////////////////////////////////////////////////////////////////////////////

namespace util
{
	template<typename T>
	class Expected;
}

namespace mathematics
{
	namespace geometry
	{
		class Line2D
		{
		public:
			mathematics::linearAlgebra::Vector2F normal;		// the normal vector of the line
			float d;											// the distance to the center of the vector space

			Line2D();											// create the x-axis as line
			Line2D(const mathematics::linearAlgebra::Vector2F& n, const float dist);	// creates a line with normal vector n and distance dist
		};

		class Ray2D
		{
		public:
			mathematics::linearAlgebra::Vector2F startPoint;	// the point of origin
			mathematics::linearAlgebra::Vector2F direction;		// the direction of the ray

			Ray2D();																											// creates an "empty" ray, starting at (0,0) and pointing to (1,0)
			Ray2D(const mathematics::linearAlgebra::Vector2F& origin, const mathematics::linearAlgebra::Vector2F& direction);	// creates a ray with point of origin and direction

			// ride the ray
			mathematics::linearAlgebra::Vector2F ride(const float t) const;	// returns the point on the ray after tracing the ray for t amounts of time
		};


		class LineSegment2D
		{
		public:
			mathematics::linearAlgebra::Vector2F startPoint, endPoint;			// the start and ending point of the line segment
			mathematics::linearAlgebra::Vector2F directionVector;				// the vector joining those two points
			mathematics::linearAlgebra::Vector2F normalVector;					// the normal vector of the line segment

			LineSegment2D(mathematics::linearAlgebra::Vector2F startPoint, mathematics::linearAlgebra::Vector2F endPoint);
			LineSegment2D(mathematics::linearAlgebra::Vector2F startPoint, mathematics::linearAlgebra::Vector2F endPoint, mathematics::linearAlgebra::Vector2F normalVector);
		};

		class Sphere2D
		{
		public:
			mathematics::linearAlgebra::Vector2F center;						// the center of the sphere
			float radius;														// the radius of the sphere

			Sphere2D();																// creates the unit sphere (center: (0,0) - radius: 1)
			Sphere2D(const mathematics::linearAlgebra::Vector2F& c, const float r);	// creates a sphere with radius r and center c

			mathematics::linearAlgebra::Vector2F point(const float angle);		// returns the coordinates of a point on the circle defined by the angle (between 0 and 2Pi)
		};

		class Rectangle2D
		{
		public:
			mathematics::linearAlgebra::Vector2F upperLeft;						// the upper left corner of the rectangle
			mathematics::linearAlgebra::Vector2F lowerRight;					// the lower right corner of the rectangle

			Rectangle2D();														// the standard constructor constructs the unit rectangle with the upper left corner glued to (0,0)
			Rectangle2D(const mathematics::linearAlgebra::Vector2F& up, const mathematics::linearAlgebra::Vector2F& lr);	// creates a rectangle with the specified extremeties
		};

		class Capsule2D
		{
		public:
			LineSegment2D lineSegment;				// the line segment defining the capsule
			float radius;							// radius of the circles on each edge

			Capsule2D();							// defines a capsule with segment from (0,0) to (0,1) and radius 0.5f
			Capsule2D(const LineSegment2D& ls, const float r);	// defines a capsule with a specified line segment and radius
			Capsule2D(const mathematics::linearAlgebra::Vector2F& startPoint, const mathematics::linearAlgebra::Vector2F& endPoint, const float r);
		};

		class Polygon2D
		{
		public:
			std::vector<mathematics::linearAlgebra::Vector2F> vertices;		// the vertices of the polygon
			mathematics::linearAlgebra::Vector2F centroid;					// the centroid of the polygon
			float normal;					// the normal of the polygon

			Polygon2D();													// creates an empty polygon
			Polygon2D(const std::vector<mathematics::linearAlgebra::Vector2F>& v, const mathematics::linearAlgebra::Vector2F& c);	// creates a polygon with a specified list of vertices, the vertices should be given in counter-clockwise order, and a specified centroid; the normal vector is computed automatically
			Polygon2D(const std::vector<mathematics::linearAlgebra::Vector2F>& v); // creates a polygon with a given set of vertices (should be given in counter-clockwise order), the centroid is computed automatically, as well as the normal vector

			bool contains(const mathematics::linearAlgebra::Vector2F& p) const;		// returns iff the point p is inside the polygon
		};

		// points on circles and ellipses
		void computeCoordinatesOnEllipse(const mathematics::linearAlgebra::Vector2F& ellipsePoint, const mathematics::linearAlgebra::Vector2F& ellipseRadius, const float angle, mathematics::linearAlgebra::Vector2F& point); // computes the x and y-coordinates of a point on an ellipse given by the angle

		// distance functions
		float distance2D(const mathematics::linearAlgebra::Vector2F& A, const mathematics::linearAlgebra::Vector2F& B);			// computes the distance between two points in euclidean space
		float squareDistance2D(const mathematics::linearAlgebra::Vector2F& A, const mathematics::linearAlgebra::Vector2F& B);	// computes the square distance between two points in euclidean space

		// polygons
		void computeCentroid(const std::vector<mathematics::linearAlgebra::Vector2F>& vertices, mathematics::linearAlgebra::Vector2F* centroid);														// this function computes the centroid of a convex and closed polygon
		void computeBoundingBox(const std::vector<mathematics::linearAlgebra::Vector2F>& vertices, mathematics::linearAlgebra::Vector2F* leftTop, mathematics::linearAlgebra::Vector2F* rightBottom);	// computes the bounding box for a polygon

		// line segments
		bool segmentIntersection2D(const LineSegment2D& segment1, const LineSegment2D& segment2, mathematics::linearAlgebra::Vector2F* t = NULL);	// returns true iff both line segments intersect each other

		// collisions
		bool intersection(const Sphere2D& s1, const Sphere2D& s2);																						// returns true iff the spheres intersection - uses squared distance test
		bool intersection(const Rectangle2D& r1, const Rectangle2D& r2);																				// returns true iff the rectangles intersection - checks for four cases where the rectangles definited can't collide
		bool intersection(const LineSegment2D& ls, const Line2D& l, mathematics::linearAlgebra::Vector2F* const intersectionPoint = nullptr);			// returns true iff the two line segments collide - uses bilear form to compute intersection point
		bool intersection(const LineSegment2D& ls1, const LineSegment2D& ls2, mathematics::linearAlgebra::Vector2F* const intersectionPoint = nullptr);	// returns true iff the two line segments collide - uses matrix equation

	}
}
