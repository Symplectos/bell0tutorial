#include "particleSystem.h"
#include "kinematics.h"

namespace physics
{
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// PARTICLE ////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	Particle::Particle() : position(0.0f, 0.0f), velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), age(0), colour(L"Black")
	{
		Environment::getInstance();
	}

	Particle::Particle(const mathematics::Vector2F& pos, const mathematics::Vector2F& vel, const mathematics::Vector2F& acc, const unsigned int ag, const std::wstring& col) : position(pos), velocity(vel), acceleration(acc), age(ag), colour(col)
	{

	}

	bool Particle::update(double deltaTime)
	{
		physics::Kinematics::semiImplicitEuler(*position, *velocity, *acceleration, deltaTime);
		age++;

	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// ENVIRONMENT /////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	Environment::Environment() {};
	Environment::Environment(const mathematics::Vector2F& grav, const mathematics::Vector2F& wi) : gravity(grav), wind(wi) {};

}
