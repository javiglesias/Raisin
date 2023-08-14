#include "Issue.h"

Issue::Issue()
{
	m_registeredObservers = 0;
}

Issue::~Issue()
{
	delete m_observers;
	m_registeredObservers = 0;
}

void Issue::addObserver(Observer* _observer)
{
	m_observers[m_registeredObservers] = _observer;
	++m_registeredObservers;
}

void Issue::removeObserver(Observer* _observer)
{
	--m_registeredObservers;
}
