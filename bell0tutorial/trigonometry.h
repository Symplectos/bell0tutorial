#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		05/04/2019 - Lenningen - Luxembourg
*
* Desc:		trigonometry
*
* History: - 05/04/2019: angles, rad and deg
*
* ToDo:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// CLASSES //////////////////////////////////////////////////////////////////////////////

namespace util
{
	template<typename T>
	class Expected;
}

namespace mathematics
{
	namespace trigonometry
	{
		// utility functions
		float degToRad(const float rad);						// returns the degrees of an angle given in radians
		float radToDeg(const float deg);						// returns the radians of an angle given in degrees
		float angleInStandardCoordinates(const float deg);		// returns the complementary angle
	}
}
