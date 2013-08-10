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
	static const char className[];				// ������ÿ����Ҫ��Lunaע����඼�����������Ա����
	static Luna<Account>::RegType methods[];	// ��Ա�����б�

	Account(lua_State *L)      { m_balance = luaL_checknumber(L, 1); }	// ���ָ�������Ƿ�Ϊnumber�����ҷ������number
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
		����luaL_newstate ����һ���»�������state����luaL_newstate ����һ���µĻ���ʱ�����������������Ԥ����ĺ�����
		������print��Ϊ�˱���Lua �����������еı�׼���Ե����İ��ṩ����������㲻��Ҫ�Ͳ���ǿ����ʹ�����ǡ�ͷ�ļ�
		lualib.h �����˴���Щ��ĺ��������磬����luaopen_io���Դ���io table ��ע��I/O ������io.read,io.write �ȵȣ�
		��Lua �����С�
	*/
	lua_State *L = luaL_newstate();	// #define lua_open()	luaL_newstate() ��lua.h

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
