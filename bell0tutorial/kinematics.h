#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		15/03/2019 - Lenningen - Luxembourg
*
* Desc:		This class models kinematics using various types of integration
*
* History:	- 27/03/2019: added a symplectic integrator for one-dimensional kinematics
			- 29/03/2019: added a symplectic integrator for two-dimensional motion
*
* ToDo:		- lots
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
	namespace linearAlgebra
	{
		class Vector2F;
	}
}

namespace physics
{
	class Projectile
	{
	private:
		// key characteristics
		float launchSpeed;						// launch speed in pixels per second
		float launchAngle;						// launch angle in degree
		float gravity;							// acceleration of gravity in pixels per second
		float range;							// range in pixels
		float peak;								// peak in pixels
		float timeOfFleight;					// in seconds

		// update
		mathematics::linearAlgebra::Vector2F position;		// the current position of the projectile (in pixels)
		mathematics::linearAlgebra::Vector2F velocity;		// the current velocity of the projectile (in pixels per second)
		mathematics::linearAlgebra::Vector2F acceleration;	// the current acceleration of the projectile (in pixels per seconds squared)
		
		// recompute initial velocity and key characteristics
		void computeInitialVelocityAndKeyCharacteristics();
		
	public:
		// constructor
		Projectile(const float launchSpeed, const float launchAngle, const float launchX = 0.0f, const float launchY = 0.0f, const float gravity = 9.81f, const float frictionX = 0.0f);
		
		// update and farseer
		void update(const double dt);			// updates the position of the projectile - uses semi-implicit Euler integration
		
		// recalibrate projectile
		void setLaunchAngle(const float angle);
		void reduceLaunchAngle();
		void increaseLaunchAngle();
		void reduceLaunchSpeed();
		void increaseLaunchSpeed();
		void increaseGravity();
		void decreaseGravity();

		// getters
		float getPositionX() const { return position.x; };
		float getPositionY() const { return position.y; };
		float getRange() const { return range; };
		float getVelocityX() const { return velocity.x; };
		float getVelocityY() const { return velocity.y; };
		float getMovementDirection() const;
	};

	class Kinematics
	{
	private:
		
	public:
		// unit conversions
		static const float metersPerPixel;														// 1 pixel = 0.0002645 meters
		static const float pixelsPerMeter;														// 1 meter = 3779.5275590551 pixels

		// one-dimensional kinematics
		static float posUM(const float x0, const float v0, const float a, double dt);				// computes the position after a time t (assume uniform acceleration)
		static void semiImplicitEuler(float& pos, float& vel, const float acc, const double dt);	// updates the position and velocity of an object using semi-implicit Euler integration (symplectic)

		// two-dimensional kinematics
		static void semiImplicitEuler(mathematics::linearAlgebra::Vector2F& pos, mathematics::linearAlgebra::Vector2F& vel, const mathematics::linearAlgebra::Vector2F acc, const double dt);	// updates the position and velocity of an object using semi-implicit Euler integration

		// projectiles
		static float computeLaunchAngle(const float launchSpeed, const float desiredRange, const float gravity = 9.81f);
		static bool computeLaunchAngle(float& angle, const mathematics::linearAlgebra::Vector2F& target, const float v, const float g = 9.81f);
	};
}
