#include "stdafx.h"
#include "..\test03\LuaPlus.h"
#include <string>
#include <Windows.h>

using std::string;
using namespace LuaPlus;

typedef unsigned long long ui64;
typedef unsigned long TypeTime;

class cPerformanceClock
{
public:
	cPerformanceClock() : c_uiTh(1000) {
		QueryPerformanceFrequency(&m_initlv);
		ui64 init = m_initlv.QuadPart * c_uiTh / m_initlv.QuadPart;
		LARGE_INTEGER lv;
		QueryPerformanceCounter( &lv );
		m_init = (lv.QuadPart * c_uiTh / m_initlv.QuadPart) - init;
	}
	ui64 GetLongTime() {
		LARGE_INTEGER lv;
		QueryPerformanceCounter( &lv );
		return (lv.QuadPart * c_uiTh / m_initlv.QuadPart) - m_init;
	}
	TypeTime GetTime() {
		return (TypeTime)GetLongTime();
	}
private:
	const unsigned int	c_uiTh;
	LARGE_INTEGER		m_initlv;
	ui64				m_init;
};

cPerformanceClock& GetPerformanceClock() {
	static cPerformanceClock s_pc;
	return s_pc;
}

inline TypeTime _GetCurTime()
{
	return GetPerformanceClock().GetTime();
	//return GetCycleCount();
}


ui64 GetCurLongTime()
{
	return GetPerformanceClock().GetLongTime();
}

namespace LPCD
{
	inline bool	Match(TypeWrapper<std::string>, lua_State* L, int idx)
	{  return lua_type(L, idx) == LUA_TSTRING;  }
	inline bool	Match(TypeWrapper<std::string&>, lua_State* L, int idx)
	{  return lua_type(L, idx) == LUA_TSTRING;  }
	inline bool	Match(TypeWrapper<const std::string&>, lua_State* L, int idx)
	{  return lua_type(L, idx) == LUA_TSTRING;  }
	inline std::string Get(TypeWrapper<std::string>, lua_State* L, int idx)
	{  return static_cast<const char*>(lua_tostring(L, idx));  }
	inline std::string Get(TypeWrapper<std::string&>, lua_State* L, int idx)
	{  return static_cast<const char*>(lua_tostring(L, idx));  }
	inline std::string Get(TypeWrapper<const std::string&>, lua_State* L, int idx)
	{  return static_cast<const char*>(lua_tostring(L, idx));  }
	inline void Push(lua_State* L, std::string& value)								{  lua_pushstring(L, value.c_str());  }
	inline void Push(lua_State* L, const std::string& value)						{  lua_pushstring(L, value.c_str());  }
}


int LS_PrintNumber(LuaState* state)
{
	LuaStack args(state);

	if (args[1].IsNumber()) {
		printf("%f\n", args[1].GetNumber());
	}
	return 0;
}

std::string LS_GetString()
{
// 	LuaStack args(state);
// 
// 	if (args[1].IsNumber()) {
// 		printf("%f\n", args[1].GetNumber());
// 	}
	return "Hello";
}

void test() 
{


	int s = 1000;
	// ����״̬
	LuaStateOwner state;

	// ע��c++������Ϳ�����lua�е�����
	state->GetGlobals().Register("PrintNumber", LS_PrintNumber);
	state->GetGlobals().RegisterDirect("GetString", LS_GetString);

	// �򿪲����ļ�
	int iret = state->DoFile("test.lua");

	printf("===��ʼ===\n");
	// �õ�һ��ȫ�ֱ���
	LuaObject sObj = state->GetGlobal("health");
	int mytest = sObj.GetInteger();
	printf("mytest = %d\n", mytest);

	// �������ֵ
	sObj.AssignInteger(state, 30);
	// �ٴεõ����ֵ
	mytest = sObj.GetInteger();
	printf("mytest = %d\n", mytest);

	int t1 = _GetCurTime();
	printf("Now Time is %d\nms", t1);
	for(int i=0; i< 1000000; i++)
	{
		LuaFunction<string> Get =  state->GetGlobal("Get");
		std::string sRet = Get();
	//	printf("mytest = %s\n", sRet.c_str());

	}
	int t2 = _GetCurTime();
	printf("Now Time is %d\nms", t2);

	// ����һ��lua�ж���ĺ���
	//LuaFunction<float> Add =  state->GetGlobal("Add");
	//float myret = Add(3.14f,5.25f);
	//printf("myret = %f\n", myret);

}

int _tmain(int argc, _TCHAR* argv[])
{
	test();

	system("Pause");
	return 0;
}

