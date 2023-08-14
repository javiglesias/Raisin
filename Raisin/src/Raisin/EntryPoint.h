#pragma once

#include "Application.h"

//extern RaisinEng::CApplication* RaisinEng::CreateApplication();

int main(void)
{
	auto app = RaisinEng::CreateApplication();
	app->Run();
	delete app;
	return 0;
}