#ifndef C__SCRIPT_H
#define C__SCRIPT_H

extern "C"
{
	#include "../3rd/lua-5.3.5/src/lua.h"
	#include "../3rd/lua-5.3.5/src/lualib.h"
	#include "../3rd/lua-5.3.5/src/lauxlib.h"
}

inline lua_State* m_lua = luaL_newstate();

inline void initLUA()
{
	luaL_openlibs(m_lua);
	luaopen_io(m_lua);
	luaopen_base(m_lua);
}

inline void executeLine(const char* _line)
{
	if (luaL_loadstring(m_lua, _line) == 0)
		lua_call(m_lua, 0, 0);
}

inline void executeScript(const char* _file)
{
	if (luaL_loadfile(m_lua, _file) == 0)
		lua_call(m_lua, 0, 0);
}
#endif
