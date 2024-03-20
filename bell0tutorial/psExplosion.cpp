#include "psExplosion.h"
#include "kinematics.h"
#include <algorithm>
#include "numberTheory.h"

namespace physics
{
	namespace particles
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// CONSTRUCTOR /////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		ExplosionPS::ExplosionPS(core::DirectXApp& app, const graphics::GraphicsComponent2D& gc, const mathematics::linearAlgebra::Vector2F& pos, const mathematics::linearAlgebra::Vector2F& velocity) : ParticleSystem(app, gc)
		{
			this->position = pos;
			float velo = velocity.getLength()*0.5f;

			for (unsigned int i = 0; i < getMaxParticles(); i++)
			{
				mathematics::linearAlgebra::Vector2F posi(pos.x + nt.generateRandomFloat(-50.0f, 50.0f), pos.y - nt.generateRandomFloat(-50.0f, 50.0f));
				mathematics::linearAlgebra::Vector2F vel(nt.generateRandomFloat(-velo, velo), nt.generateRandomFloat(-velo, velo));
				mathematics::linearAlgebra::Vector2F acc(0.0f, 0.0f);
				float age = nt.generateRandomFloat(0.0f, 500.0f);

				if(i % 3 == 0)
					GenerateParticle(posi, vel, acc, age, L"Black", nt.generateRandomFloat(0.25f, 2.5f));
				else if(i % 3 == 1)
					GenerateParticle(posi, vel, acc, age, L"DarkGoldenrod", nt.generateRandomFloat(0.25f, 2.5f));
				else
					GenerateParticle(posi, vel, acc, age, L"DarkRed", nt.generateRandomFloat(0.25f, 2.5f));
			}
		}

		void ExplosionPS::GenerateParticle(const mathematics::linearAlgebra::Vector2F& pos, mathematics::linearAlgebra::Vector2F& vel, const mathematics::linearAlgebra::Vector2F& acc, const float age, const std::wstring& col, const float width)
		{
			this->particles.push_back(Particle(pos, vel, acc, age, col, width));
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// UPDATE //////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		bool ExplosionPS::update(double deltaTime)
		{
			for (auto it = particles.begin(); it != particles.end();)
			{
				it->update(deltaTime, getMaxLifeSpan());
				if (it->getAge() > getMaxLifeSpan())
					it = particles.erase(it);
				else
					it++;
			}

			if (particles.size() == 0)
				return false;
			else
				return true;
		}
	}
}