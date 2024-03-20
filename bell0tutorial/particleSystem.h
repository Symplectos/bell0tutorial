#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		23/07/2019 - Lenningen - Luxembourg
*
* Desc:		this class defines a basic particle system
*
* History:	- 23/07/2019: basics
*
* ToDo:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// C++ includes
#include <string>
#include <vector>

// bell0bytes includes
#include "vectors.h"
#include "graphicsComponent2D.h"
#include "app.h"


// CLASSES //////////////////////////////////////////////////////////////////////////////
namespace mathematics
{
	namespace numberTheory
	{
		class NumberTheory;
	}
}

namespace physics
{
	namespace particles
	{
		// a single particle
		class Particle
		{
		private:
			mathematics::linearAlgebra::Vector2F position;				// the position of the particle
			mathematics::linearAlgebra::Vector2F velocity;				// the velocity vector
			mathematics::linearAlgebra::Vector2F acceleration;			// the acceleration vector, depends on environmental factors
			float age;									// the age of the particle; a particle is deleted once its age surpasses the maximal lifespan defined by the particle system
			std::wstring colour;						// the colour of the particle
			float intensity = 1.0f;						// the intensity diminishes as the particle nears the end of its life
			float width = 1.0f;							// the width of each particle

		public:
			// constructors
			Particle();
			Particle(const mathematics::linearAlgebra::Vector2F& position, const mathematics::linearAlgebra::Vector2F& velocity, const mathematics::linearAlgebra::Vector2F& acc, const float age = 0.0f, const std::wstring& colour = L"Black", const float width = 1.0f);

			// update
			void update(double deltaTime, float maxLifeSpan);		// returns false iff the particle should be destroyed (for example, because its lifespan is reached)

			// getters
			float getAge() const { return age; };

			// setters
			void setWidth(float w) { width = w; };

			friend class ParticleSystem;
		};

		// Environment class - Singleton
		// the environment class defines the environment for the particles, such as, for example, gravity and wind
		class Environment
		{
		private:
			// environment settings
			mathematics::linearAlgebra::Vector2F gravity;				// force due to gravity
			mathematics::linearAlgebra::Vector2F wind;					// force due to wind

		protected:
			// protected constructor -> singleton
			Environment();

		public:

			// create a single instance
			static Environment& getInstance()
			{
				static Environment instance;
				return instance;
			};

			// delete copy and assignment operators
			Environment(Environment const&) = delete;
			Environment& operator = (Environment const&) = delete;

			// getters
			mathematics::linearAlgebra::Vector2F getGravity() const { return gravity; };
			mathematics::linearAlgebra::Vector2F getWind() const { return wind; };

			// setters
			void setGravity(mathematics::linearAlgebra::Vector2F& grav) { gravity = grav; };
			void setWind(mathematics::linearAlgebra::Vector2F& wi) { wind = wi; };
		};

		// abstract particle system class
		// this abstract class defines the actual particle systems
		class ParticleSystem
		{
		private:
			unsigned int maxParticles = 150;				// the maximal number of particles
			float maxLifeSpan = 2500.0f;					// the maximal lifespan of a particle

		protected:
			core::DirectXApp& dxApp;							// the DirectXApp
			const graphics::GraphicsComponent2D& gc;			// pointer to the Direct2D object of the DirectXApp
			const mathematics::numberTheory::NumberTheory& nt;	// pointer to the number theory object of the DirectXApp
			std::vector<Particle> particles;					// the array containing the actual particles in the system
			bool regenerate = false;							// true iff the particles shoul regenerate over time (think of a water fountain)
			mathematics::linearAlgebra::Vector2F position;					// central position of the particle system

			virtual void GenerateParticle(const mathematics::linearAlgebra::Vector2F& position, mathematics::linearAlgebra::Vector2F& velocity, const mathematics::linearAlgebra::Vector2F& acceleration, const float age = 0.0f, const std::wstring& colour = L"Black", const float width = 1.0f) = 0;

			// setters
			void setMaxParticles(unsigned int mp) { maxParticles = mp; };
			void setMaxLifeSpan(float mls) { maxLifeSpan = mls; };

		public:
			ParticleSystem(core::DirectXApp& app, const graphics::GraphicsComponent2D& gc) : dxApp(app), gc(gc), nt(dxApp.getNumberTheoryComponent()) {};

			virtual bool update(double deltaTime) = 0;		// returns false iff there are no more particles alive in the system
			virtual void draw(double farSeer) const;		// render the particles

			unsigned int nParticles() const;				// returns the number of particles in the system
			float getMaxLifeSpan() const;					// returns the maximal life span possible
			unsigned int getMaxParticles() const;			// returns the possible maximal number of particles
		};
	}
}