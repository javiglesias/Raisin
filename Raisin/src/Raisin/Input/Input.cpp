#include "Input.h"

namespace RaisinEng
{
	void _process_input(int _key)
	{
		if(m_inputs[_key].callback)
			m_inputs[_key].callback();
	}

	void _add_callback_input(eINPUTKEY _key, void(*_callback)())
	{
		m_inputs[_key] = sInputRegistered{_key, _callback};
	}
}
