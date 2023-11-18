#pragma once
#include <string>

namespace RaisinEng
{
inline std::string _DebugPanel;
inline std::string _InfoPanel;
inline std::string _ErrorPanel;

#define DEBUG_LOG(X)	{_DebugPanel += X;\
					_DebugPanel += '\n';}
#define ERROR_LOG(X)	{_ErrorPanel += X;\
					_ErrorPanel += '\n';}
#define INFO_LOG(X)	{_InfoPanel += X;\
					_InfoPanel += '\n';}
}
