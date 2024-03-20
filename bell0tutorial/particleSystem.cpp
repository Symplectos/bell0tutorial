#include "particleSystem.h"
#include "kinematics.h"
#include <cmath>

namespace physics
{
	namespace particles
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// PARTICLE ////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Particle::Particle() : position(0.0f, 0.0f), velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), age(0), colour(L"Black")
		{
			// add environment variables
			acceleration = Environment::getInstance().getGravity() + Environment::getInstance().getWind();
		}

		Particle::Particle(const mathematics::linearAlgebra::Vector2F& pos, const mathematics::linearAlgebra::Vector2F& vel, const mathematics::linearAlgebra::Vector2F& acc, const float ag, const std::wstring& col, const float width) : position(pos), velocity(vel), acceleration(acc), age(ag), colour(col), width(width)
		{
			// add environment variables
			acceleration += Environment::getInstance().getGravity() - Environment::getInstance().getWind();
		}

		void Particle::update(double deltaTime,float maxLifeSpan)
		{
			// update position and velocity
			physics::Kinematics::semiImplicitEuler(position, velocity, acceleration, deltaTime);
			
			// update the age and intensity
			age += 0.1f;
			intensity = (maxLifeSpan - age) / (maxLifeSpan);
		}
		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// ENVIRONMENT /////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		Environment::Environment() : gravity(mathematics::linearAlgebra::Vector2F(0.0f, 9.81f)), wind(mathematics::linearAlgebra::Vector2F(7.5f, -1.0f)) {};

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// PARTICLE SYSTEM /////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		void ParticleSystem::draw(double /*farSeer*/) const
		{
			// draw particles
			for (auto particle : particles)
			{
				if(particle.colour == L"Black")
					gc.fillRectangle(particle.position.x - particle.width, particle.position.y - particle.width, particle.position.x + particle.width, particle.position.y + particle.width, particle.intensity);
				else
					gc.fillRectangle(particle.position.x - particle.width, particle.position.y - particle.width, particle.position.x + particle.width, particle.position.y + particle.width, particle.intensity, &gc.getBrush(particle.colour));
			}
		}

		unsigned int ParticleSystem::nParticles() const
		{
			return (unsigned int)this->particles.size();
		}

		float ParticleSystem::getMaxLifeSpan() const
		{
			return this->maxLifeSpan;
		}

		unsigned int ParticleSystem::getMaxParticles() const
		{
			return this->maxParticles;
		}
	}
}
