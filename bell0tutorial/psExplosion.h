#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		23/07/2019 - Lenningen - Luxembourg
*
* Desc:		this class defines an explosion particle system
*
* History:
*
* ToDo:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////
#include "particleSystem.h"
#include "vectors.h"

// CLASSES //////////////////////////////////////////////////////////////////////////////

namespace physics
{
	namespace particles
	{
		class ExplosionPS : public ParticleSystem
		{
		private:

		protected:
			virtual void GenerateParticle(const mathematics::linearAlgebra::Vector2F& position, mathematics::linearAlgebra::Vector2F& velocity, const mathematics::linearAlgebra::Vector2F& acceleration, const float age = 0, const std::wstring& colour = L"Black", const float width = 1.0f) override;

		public:
			ExplosionPS(core::DirectXApp& app, const graphics::GraphicsComponent2D& gc, const mathematics::linearAlgebra::Vector2F& pos, const mathematics::linearAlgebra::Vector2F& velocity);
			virtual bool update(double deltaTime) override;
		};
	}
}