#include "numberTheory.h"
#include <random>

namespace mathematics
{
	namespace numberTheory
	{
		NumberTheory::NumberTheory()
		{
			this->rd = new std::random_device;
			this->mt = new std::mt19937_64((*rd)());
		}

		NumberTheory::~NumberTheory()
		{
			if (rd)
				delete rd;
			if (mt)
				delete mt;
		}

		float NumberTheory::generateRandomFloat(float min, float max) const
		{
			std::uniform_real_distribution<float> dist(min, max);
			return dist(*mt);
		}
	}
}