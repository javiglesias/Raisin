#include "Raisin.hpp"

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
