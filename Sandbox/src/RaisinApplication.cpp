#include "Raisin.hpp"

#include <cstdio>

void w_pressed()
{
	fprintf(stderr, "W pressed on Game Side.");
}
class SandApplication : public RaisinEng::CApplication
{
public:
	SandApplication()
	{
	}
	~SandApplication() {}
};

RaisinEng::CApplication* RaisinEng::CreateApplication()
{
	return new SandApplication();
}
