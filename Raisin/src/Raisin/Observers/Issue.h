// OBSERVER SUBJECT
#pragma once
#include "Observer.h"
class Issue
{
public:
	Issue();
	~Issue();
	void addObserver(Observer* _observer);
	void removeObserver(Observer* _observer);
private:
	Observer* m_observers[100];
	int m_registeredObservers = 0;
};

