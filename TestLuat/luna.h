
extern "C" {
#include "lua.h"
#include "lauxlib.h"
}



template <typename T> 
class Luna 
{
	typedef struct { T *pT; } userdataType;
public:
	typedef int (T::*mfp)(lua_State *L);

	typedef struct
	{
		const char *name; 
		mfp mfunc; 
	} RegType;	// �������������Ķ�Ӧ�ṹ

	/*
		ע��֮�󽫲�������ȫ��table
		tbClass = 
		{
			["new"] = new_T, 
			["metatable"] = {__call = new_T},
			["FunName1"] = Fun,(Funͨ��closure�����ֲ�ͬ��T��Ա����)
			["FunName2"] = Fun,
			...
		}

		�Լ�����ͨ��luaL_getmetatable(L, T::className)�õ������metatable��New�����Ķ���ͨ����ָ����metatableΪ��
		{
			["__metatable"] = tbClass, (hidemetatable)
			["__index"] = tbClass,
			["__tostring"] = tostring_T,
			["__gc"] = gc_T,
		}
	*/

	// Ϊһ����ע����Ӧ�ű����÷�ʽ
	static void Register(lua_State *L) 
	{
		lua_newtable(L);
		int methods = lua_gettop(L);

		// ���������userdata(T�Ķ���)��metatable
		luaL_newmetatable(L, T::className);
		int metatable = lua_gettop(L);

		// Ϊ�ཨ��ȫ�ֱ�tbClass
		lua_pushstring(L, T::className);
		lua_pushvalue(L, methods);
		lua_settable(L, LUA_GLOBALSINDEX);

		// hide metatable from Lua getmetatable()
		lua_pushliteral(L, "__metatable");
		lua_pushvalue(L, methods);
		lua_settable(L, metatable);

		// metatable["__index"] = Value(methods)
		lua_pushliteral(L, "__index");
		lua_pushvalue(L, methods);
		lua_settable(L, metatable);

		// metatable["__tostring"] = tostring_T
		lua_pushliteral(L, "__tostring");
		lua_pushcfunction(L, tostring_T);
		lua_settable(L, metatable);

		// metatable["__gc"] = gc_T
		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, gc_T);
		lua_settable(L, metatable);

		// tbClass.new = new_T, 
		lua_pushliteral(L, "new");
		lua_pushcfunction(L, new_T);
		lua_settable(L, methods);

		// tbMetatable.__call = new_T
		lua_newtable(L);
		int mt = lua_gettop(L);
		lua_pushliteral(L, "__call");
		lua_pushcfunction(L, new_T);
		lua_settable(L, mt);

		// tbClass.metatable = tbMetatable
		lua_setmetatable(L, methods);

		/*
			userdata �� Lua �б�ʾһ�� C ֵ�� light userdata ��ʾһ��ָ�롣����һ��������һ����ֵ���㲻��Ҫר�Ŵ���������Ҳû�ж����� metatable��
			����Ҳ���ᱻ�ռ�����Ϊ��������Ҫ��������ֻҪ��ʾ�� C ��ַ��ͬ������ light userdata ����ȡ� 
		*/
		/*
			��������	һ�� C ����������Ը�������һЩֵ�����������ڴ���һ�� C closure�����������ۺ�����ʱ�����ã�
			��Щֵ�����Ա�����������ʵ���Ϊ�˽�һЩֵ������һ�� C �����ϣ�������Щֵ��Ҫ�ȱ�ѹ���ջ������ж��ֵ����һ����ѹ����
			���������� lua_pushcclosure �������� closure ������� C ����ѹ����ջ�ϡ����� n ��֮�����ж��ٸ�ֵ��Ҫ�����������ϡ� 
			lua_pushcclosure Ҳ�����Щֵ��ջ�ϵ����� 
		*/
		// ΪtbClass����Ա����
		for (RegType *l = T::methods; l->name; l++) 
		{
			/* edited by Snaily: shouldn't it be const RegType *l ... ? */
			lua_pushstring(L, l->name);
			lua_pushlightuserdata(L, (void*)l);		// ��(������,������ַ)pair��lightuserdata����ʽ��ΪC closure��upvalue��ջ
			lua_pushcclosure(L, thunk, 1);			// ��һ���µ� C closure ѹ���ջ��1Ϊupvalue�ĸ�������ָ���ص�����ͳһΪthunk
			lua_settable(L, methods);				// tbClass[FunName] = Function
		}

		lua_pop(L, 2);	// ����ջ�����ڵ�2��Ԫ��
	}

private:
	Luna();

	// ���г�Ա����������õ����Ȼ�����upvalue��ִ�о���ĳ�Ա����
	static int thunk(lua_State *L)
	{
		// stack has userdata, followed by method args
		T *obj = check(L, 1);  // the object pointer from the table at index 0.
		lua_remove(L, 1);  // remove self so member function args start at index 1
		// get member function from upvalue
		RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));  
		return (obj->*(l->mfunc))(L);  // call member function
	}

	// ����ָ��λ��narg��ö���ָ��,���userdata����new_T��ʱ�򴴽���
	static T *check(lua_State *L, int narg) 
	{
		void *pUserData = luaL_checkudata(L, narg, T::className);	// ջ��ָ��λ�õĶ����Ƿ�Ϊ���и������ֵ�metatable��usertatum
		userdataType *ud = static_cast<userdataType*>(pUserData);	// ����Ǻ�����upvalue
		if(!ud) 
			luaL_typerror(L, narg, T::className);
		return ud->pT;
	}

	// ����һ���µ�T���󣬲�����һ������userdataType��userdata�����б�����ָ��T�����ָ��
	static int new_T(lua_State *L) 
	{
		lua_remove(L, 1);   // use classname:new(), instead of classname.new()
		T *obj = new T(L);  // call constructor for T objects
		userdataType *ud =
			static_cast<userdataType*>(lua_newuserdata(L, sizeof(userdataType)));
		ud->pT = obj;  // store pointer to object in userdata
		luaL_getmetatable(L, T::className);  // lookup metatable in Lua registry
		lua_setmetatable(L, -2);
		return 1;  // userdata containing pointer to T object
	}

	// garbage collection metamethod
	static int gc_T(lua_State *L) 
	{
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		delete obj;  // call destructor for T objects
		return 0;
	}

	static int tostring_T (lua_State *L) 
	{
		char buff[32];
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		sprintf(buff, "%p", obj);
		lua_pushfstring(L, "%s (%s)", T::className, buff);
		return 1;
	}
};

/*
����
[methods]			-- 4.Push methodsTable, it is pushed in the frist step
"Account"			-- 3.Push ClassName Account
[metatable] = {} 	-- 2.Register metatable, Key is Account��Index is metatable
[methods] = {} 		-- 1.Push NewTable Index is methods


-- 5.lua_settable
--GLOBA[T::className] = Value(methods) ��_G.Account = {}

[methods]			-- 7.Push methodsTable, it is pushed in the frist step
"__metatable"		-- 6.Push String
[metatable] = {}
[methods] = {}


-- 8.lua_settable
-- [metatable].__metatable = [methods]

[methods]			-- 10.Push methodsTable, it is pushed in the frist step
"__index"			-- 9.push string
[metatable] = {}
[methods] = {}

-- 11.lua_settable
-- [metatable].__index = [methods]

tostring_T			-- 13.push c function
"__tostring"		-- 12.push string
[metatable] = {}
[methods] = {}


-- 14.lua_settable
-- [metatable].__tostring = tostring_T


gc_T				-- 16.push c function
"__gc"				-- 15.push string
[metatable] = {}
[methods] = {}

-- 17.lua_settable
-- [metatable].__gc = __gc


new_T				-- 22.push c function
"new"				-- 21.push string
new_T				-- 20.push c function
"__call"			-- 19.push string
[mt] = {}			-- 18.newTable
[metatable] = {}
[methods] = {}


-- 23.lua_settable
-- [methods].new = new_T
new_T				-- 20.push c function
"__call"			-- 19.push string
[mt] = {}			-- 18.newTable
[metatable] = {}
[methods] = {}


-- 24.lua_settable
-- [mt].__call = new_T
[mt] = {}			-- 18.newTable
[metatable] = {}
[methods] = {}


-- 25.lua_setmetatable
--[methods].metatable = [mt]
[metatable] = {}
[methods] = {}

-- ע�ắ��
{"FunName", &Account::FunName}		-- 27. lua_pushlightuserdata
"FunName"							-- 26. push string ��������ջ
[metatable] = {}
[methods] = {}

-- 28.lua_pushcclosure(L, thunk, 1);
closureFun thunk
"FunName"
[metatable] = {}
[methods] = {}


-- 29.lua_settable
--[methods].FunName = thunk
[metatable] = {}
[methods] = {}

-- 30.lua_pop(L, 2);

*/
