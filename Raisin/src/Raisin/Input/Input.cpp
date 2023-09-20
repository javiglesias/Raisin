#include "Input.h"

namespace RaisinEng
{
	void _process_input(int _key)
	{
		if(m_inputs[_key].key != UNKNOWN)
		{
			m_inputs[_key].callback(_key);
		}
	}

	void _add_callback_input(eINPUTKEY _key, void(*_callback)(int _key))
	{
		m_inputs[_key] = sInputRegistered{_key, _callback};
	}
}
