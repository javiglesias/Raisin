#include "Application.h"
#include "Render/RaiEng.h"

namespace RaisinEng
{
	void CApplication::Run()
	{
		Vector2D v(3.f, 4.f);
		Vector2D w(3.f, 4.f);
		auto t = v - w;
		printf("Raisin Loaded\n");
		_Init();
		_Loop();
		_Destroy();
	}
}