#include "vectors.h"
#include "kinematics.h"
#include "serviceLocator.h"

// C++ io
#include <sstream>

// C++ mathematics
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

namespace physics
{
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// General /////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	const float Kinematics::metersPerPixel = 0.000264583f;		// 1 pixel = 0.000264583 meters
	const float Kinematics::pixelsPerMeter = 3779.5275590551f;	// 1 meter = 3779.5275590551 pixels

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// One - Dimensional ///////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	float Kinematics::posUM(const float x0, const float v0, const float a, double dt)
	{
		return (float)(x0 + v0 * dt + 0.5f * (double)a * dt * dt);
	}

	void Kinematics::semiImplicitEuler(float& pos, float& vel, const float acc, const double dt)
	{
		// semi-implicit Euler integration (symplectic, thus conserves the energy of the Hamiltonian system)
		vel += (float)(acc * dt);
		pos += (float)(vel * dt);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Two - Dimensional ///////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void Kinematics::semiImplicitEuler(mathematics::linearAlgebra::Vector2F& pos, mathematics::linearAlgebra::Vector2F& vel, const mathematics::linearAlgebra::Vector2F acc, const double dt)
	{
		// semi-implicit Euler integration (symplectic, thus conserves the energy of the Hamiltonian system)
		vel += acc * dt;
		pos += vel * dt;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Projectiles /////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	// angle of reach - see bell0bytes.eu for the formula
	float Kinematics::computeLaunchAngle(const float launchSpeed, const float desiredRange, const float gravity)
	{
		// compute desired angle of reach
		float launchAngle = 0.5f * std::asinf((gravity * desiredRange) / (launchSpeed * launchSpeed));

		return (float)((double)launchAngle * 180 / M_PI);
	}

	// launch angle to hit target - see bell0bytes.eu for the formula
	bool Kinematics::computeLaunchAngle(float& angle, const mathematics::linearAlgebra::Vector2F& target, const float v, const float g)
	{
		// see bell0bytes
		float root = v * v*v*v - g * (g*target.x*target.x + 2 * target.y*target.y*v*v);
		if (root < 0)
			return false;
		root = std::sqrtf(root);
		root += v * v;
		root /= (g*target.x);

		angle = (float)((double)std::atanf(root) * 180 / M_PI);
		return true;
	}

	// compute initial velocity and key characteristics
	void Projectile::computeInitialVelocityAndKeyCharacteristics()
	{
		// get launch angle in radians
		float launchAngleRad = ((float)M_PI / 180)*launchAngle;

		// compute starting velocity
		float cosAngle = std::cosf(launchAngleRad);
		float sinAngle = std::sinf(launchAngleRad);
		velocity.x = launchSpeed * cosAngle;
		velocity.y = -launchSpeed * sinAngle;

		// calculate characteristics
		if (position.x == 0 && position.y == 1080)
		{
			// starting position is at the lower left of the screen
			timeOfFleight = (2 * launchSpeed / gravity)*sinAngle;
			range = (launchSpeed*launchSpeed / gravity) * 2 * sinAngle*cosAngle;
		}
		else
		{
			timeOfFleight = (launchSpeed*sinAngle + std::sqrtf(launchSpeed*launchSpeed*sinAngle*sinAngle + 2 * gravity*position.y)) / gravity;
			range = (launchSpeed*launchSpeed*std::sinf(2 * launchAngleRad)) / (2 * gravity) * (1 + std::sqrtf(1 + (2 * gravity*position.y) / (launchSpeed*launchSpeed*sinAngle*sinAngle)));
		}

		peak = (launchSpeed * launchSpeed * sinAngle * sinAngle) / (2 * gravity);
	}

	// the constructors
	Projectile::Projectile(const float launchSpeed, const float launchAngle, const float launchX, const float launchY, const float gravity, const float frictionX) : launchSpeed(launchSpeed), launchAngle(launchAngle), gravity(gravity)
	{
		// set starting position
		position.x = launchX;
		position.y = launchY;

		// set initial acceleration
		acceleration.x = -frictionX;
		acceleration.y = this->gravity;

		// compute initial velocity
		computeInitialVelocityAndKeyCharacteristics();
	}

	// recalibrate projectile
	void Projectile::setLaunchAngle(const float angle)
	{
		this->launchAngle = angle;

		// recompute initial velocity and key characteristics
		computeInitialVelocityAndKeyCharacteristics();
	}

	void Projectile::reduceLaunchAngle()
	{
		this->launchAngle -= 1.25f;

		// recompute initial velocity and key characteristics
		computeInitialVelocityAndKeyCharacteristics();
	}

	void Projectile::increaseLaunchAngle()
	{
		this->launchAngle += 1.25;

		// recompute initial velocity and key characteristics
		computeInitialVelocityAndKeyCharacteristics();
	}

	void Projectile::reduceLaunchSpeed()
	{
		this->launchSpeed -= 50;

		// recompute initial velocity and key characteristics
		computeInitialVelocityAndKeyCharacteristics();
	}

	void Projectile::increaseLaunchSpeed()
	{
		this->launchSpeed += 50;

		// recompute initial velocity and key characteristics
		computeInitialVelocityAndKeyCharacteristics();
	}

	void Projectile::increaseGravity()
	{
		this->gravity += 50;
		acceleration.y = this->gravity;

		// recompute initial velocity and key characteristics
		computeInitialVelocityAndKeyCharacteristics();
	}

	void Projectile::decreaseGravity()
	{
		this->gravity -= 50;
		acceleration.y = this->gravity;

		// recompute initial velocity and key characteristics
		computeInitialVelocityAndKeyCharacteristics();
	}

	// get direction of movement - angle in degree
	float Projectile::getMovementDirection() const
	{
		return (float)((double)std::atanf(velocity.y / velocity.x) * 180 / M_PI);
	}

	// update position
	void Projectile::update(const double dt)
	{
		// update the position and velocity of the projectile using semi-implicit Euler integration
		Kinematics::semiImplicitEuler(position, velocity, acceleration, dt);
	}

	
}