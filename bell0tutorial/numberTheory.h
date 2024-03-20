#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		23/07/2019 - Lenningen - Luxembourg
*
* Desc:		number theory class
*
* History:	- 24/07/2019: singleton
*
* ToDo:
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////
#include <random>

// CLASSES //////////////////////////////////////////////////////////////////////////////
namespace mathematics
{
	namespace numberTheory
	{
		class NumberTheory
		{
		private:
			// random device
			std::random_device* rd;
			std::mt19937_64* mt;

		protected:
			// protected constructor -> singleton
			NumberTheory();

		public:
			// destructor
			~NumberTheory();

			// create a single instance
			static NumberTheory& getInstance()
			{
				static NumberTheory instance;
				return instance;
			};

			// delete copy and assignment operators
			NumberTheory(NumberTheory const&) = delete;
			NumberTheory& operator = (NumberTheory const&) = delete;

			// random number generators
			float generateRandomFloat(float min, float max) const;		// generates a random number between min and max
		};
	}
}