#include "observer.h"

#include "expected.h"

namespace util
{
	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// The Observer /////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// The Subject //////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	void Subject::addObserver(Observer* const observer)
	{
		// add observer to observer list
		observers.insert(observer);
	}

	void Subject::removeObserver(Observer* const observer)
	{
		// remove the observer from the list
		observers.erase(observer);
	}

	util::Expected<void> Subject::notify(const int event) const
	{
		util::Expected<void> notificationResult;

		for (auto x : observers)
		{
			notificationResult = x->onNotify(event);
			if (!notificationResult.isValid())
				return notificationResult;
		}

		return { };
	}
}