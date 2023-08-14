#ifndef C__PROFILING_H
#define C__PROFILING_H
/* STANDALONE LIB
 * USAGE:
 *	init();
 *	Start(type, name)
 *	end(type, name)
 *	recordData();
 */
#include <cstdio>
#include <ctime>
#include <string>
namespace PROFILING
{
	#define HEADER "{\"traceEvents\": ["
	#define FOOTER "]}"

	enum PROF_TYPE
	{
		DRAW,
		LOGIC,
		NONE,
		COUNT
	};
	inline char PROF_NAMES[COUNT][128] =
	{
		"DRAW",
		"LOGIC",
		"NONE"
	};
	inline FILE* container;
	inline std::string ProfilingData;
	inline clock_t begin_ts;
	inline clock_t end_ts;
	inline bool IsProfilingEnabled = false;
	inline void _init_()
	{
		if(!IsProfilingEnabled) return;
		container = fopen("profiling.json", "w+");
		if (!container)
			fprintf(stderr, "ERROR OPENING PROFILING.json");
	}
	inline void _enable_profiling_()
	{
		IsProfilingEnabled = true;
	}
	inline void _start_(int _type, const char* _name)
	{
		if (!IsProfilingEnabled) return;
		int  pid = 0;
		int  tid = 0;
		char ph  = 'B';
		auto ts = std::to_string(clock());

		ProfilingData = ProfilingData
			+ "\n{\"cat\": \""   + PROF_NAMES[_type]
			+ "\",\"name\" : \"" + _name
			+ "\",\"ph\" : \""	 + ph 
			+ "\",\"pid\" : "	 + std::to_string(pid)
 			+ ",\"tid\" : "		 + std::to_string(tid)
			+ ",\"ts\" : "		 + ts
			+ "},";
	}

	inline void _end_(int _type, const char* _name)
	{
		if (!IsProfilingEnabled) return;
		int  pid = 0;
		int  tid = 0;
		char ph  = 'E';
		auto ts = std::to_string(clock());

		ProfilingData = ProfilingData
			+ "\n{\"cat\": \""	 + PROF_NAMES[_type]
			+ "\",\"name\" : \"" + _name
			+ "\",\"ph\" : \""	 + ph
			+ "\",\"pid\" : "	 + std::to_string(pid)
			+ ",\"tid\" : "		 + std::to_string(tid)
			+ ",\"ts\" : "		 + ts
			+ "},";
	}

	inline void _recordData_()
	{
		if (!IsProfilingEnabled) return;
		fprintf(container, "%s", HEADER);
		fprintf(container, "%s", ProfilingData.c_str());
		fprintf(container, "%s", FOOTER);
		fclose(container);
	}
}
#endif
