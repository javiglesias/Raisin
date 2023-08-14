#pragma once

#include "Core.hpp"


namespace RaisinEng
{
	
	class RAISIN_API CApplication
	{
	public:
		CApplication() {};
		virtual ~CApplication() {};
		virtual void Run();
		// Esta funcion se define en el cliente
	};
	CApplication* CreateApplication();
}
