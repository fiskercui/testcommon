extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
 
#include "luna.h"

class Account 
{
	lua_Number m_balance;						// double
public:
	static const char className[];				// 类名，每个需要用Luna注册的类都必须有这个成员变量
	static Luna<Account>::RegType methods[];	// 成员函数列表

	Account(lua_State *L)      { m_balance = luaL_checknumber(L, 1); }	// 检查指定参数是否为number，并且返回这个number
	int deposit (lua_State *L) { m_balance += luaL_checknumber(L, 1); return 0; }
	int withdraw(lua_State *L) { m_balance -= luaL_checknumber(L, 1); return 0; }
	int balance (lua_State *L) { lua_pushnumber(L, m_balance); return 1; }
	~Account() { printf("deleted Account (%p)\n", this); }
};

const char Account::className[] = "Account";

#define method(class, name) {#name, &class::name}

Luna<Account>::RegType Account::methods[] = {
		{"deposit", &Account::deposit} ,
		{"withdraw", &Account::withdraw},
		{"balance", &Account::balance},
		{0,0}
};


int main(int argc, char *argv[])
{
	/*
		函数luaL_newstate 创建一个新环境（或state）。luaL_newstate 创建一个新的环境时，这个环境并不包括预定义的函数，
		甚至是print。为了保持Lua 的苗条，所有的标准库以单独的包提供，所以如果你不需要就不会强求你使用它们。头文件
		lualib.h 定义了打开这些库的函数。例如，调用luaopen_io，以创建io table 并注册I/O 函数（io.read,io.write 等等）
		到Lua 环境中。
	*/
	lua_State *L = luaL_newstate();	// #define lua_open()	luaL_newstate() 见lua.h

	lua_cpcall(L, luaopen_base, 0);
	lua_cpcall(L, luaopen_table, 0);
	lua_cpcall(L, luaopen_io, 0);
	lua_cpcall(L, luaopen_string, 0);
	lua_cpcall(L, luaopen_math, 0);
	lua_cpcall(L, luaopen_debug, 0);
	

	Luna<Account>::Register(L);

	int s = luaL_loadfile(L, argv[1]);
	if (s == 0)
	{
		// execute Lua program
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	lua_gc(L,LUA_GCCOLLECT, 0);  // collected garbage
	lua_close(L);
	return 0;
}
