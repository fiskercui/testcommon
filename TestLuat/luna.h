
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
	} RegType;	// 函数名到函数的对应结构

	/*
		注册之后将产生以下全局table
		tbClass = 
		{
			["new"] = new_T, 
			["metatable"] = {__call = new_T},
			["FunName1"] = Fun,(Fun通过closure来区分不同的T成员函数)
			["FunName2"] = Fun,
			...
		}

		以及可以通过luaL_getmetatable(L, T::className)得到下面的metatable，New出来的对象通常会指定其metatable为它
		{
			["__metatable"] = tbClass, (hidemetatable)
			["__index"] = tbClass,
			["__tostring"] = tostring_T,
			["__gc"] = gc_T,
		}
	*/

	// 为一个类注册相应脚本调用方式
	static void Register(lua_State *L) 
	{
		lua_newtable(L);
		int methods = lua_gettop(L);

		// 这个表用于userdata(T的对象)的metatable
		luaL_newmetatable(L, T::className);
		int metatable = lua_gettop(L);

		// 为类建立全局表tbClass
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
			userdata 在 Lua 中表示一个 C 值。 light userdata 表示一个指针。它是一个像数字一样的值：你不需要专门创建它，它也没有独立的 metatable，
			而且也不会被收集（因为从来不需要创建）。只要表示的 C 地址相同，两个 light userdata 就相等。 
		*/
		/*
			当创建了	一个 C 函数后，你可以给它关联一些值，这样就是在创建一个 C closure；接下来无论函数何时被调用，
			这些值都可以被这个函数访问到。为了将一些值关联到一个 C 函数上，首先这些值需要先被压入堆栈（如果有多个值，第一个先压）。
			接下来调用 lua_pushcclosure 来创建出 closure 并把这个 C 函数压到堆栈上。参数 n 告之函数有多少个值需要关联到函数上。 
			lua_pushcclosure 也会把这些值从栈上弹出。 
		*/
		// 为tbClass填充成员函数
		for (RegType *l = T::methods; l->name; l++) 
		{
			/* edited by Snaily: shouldn't it be const RegType *l ... ? */
			lua_pushstring(L, l->name);
			lua_pushlightuserdata(L, (void*)l);		// 把(函数名,函数地址)pair以lightuserdata的形式作为C closure的upvalue入栈
			lua_pushcclosure(L, thunk, 1);			// 把一个新的 C closure 压入堆栈。1为upvalue的个数，并指定回调函数统一为thunk
			lua_settable(L, methods);				// tbClass[FunName] = Function
		}

		lua_pop(L, 2);	// 弹出栈内属于的2个元素
	}

private:
	Luna();

	// 所有成员函数都会调用到这里，然后根据upvalue来执行具体的成员函数
	static int thunk(lua_State *L)
	{
		// stack has userdata, followed by method args
		T *obj = check(L, 1);  // the object pointer from the table at index 0.
		lua_remove(L, 1);  // remove self so member function args start at index 1
		// get member function from upvalue
		RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));  
		return (obj->*(l->mfunc))(L);  // call member function
	}

	// 根据指定位置narg获得对象指针,这个userdata是在new_T的时候创建的
	static T *check(lua_State *L, int narg) 
	{
		void *pUserData = luaL_checkudata(L, narg, T::className);	// 栈中指定位置的对象是否为带有给定名字的metatable的usertatum
		userdataType *ud = static_cast<userdataType*>(pUserData);	// 这个是函数的upvalue
		if(!ud) 
			luaL_typerror(L, narg, T::className);
		return ud->pT;
	}

	// 创建一个新的T对象，并创建一个基于userdataType的userdata，其中保护了指向T对象的指针
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
流程
[methods]			-- 4.Push methodsTable, it is pushed in the frist step
"Account"			-- 3.Push ClassName Account
[metatable] = {} 	-- 2.Register metatable, Key is Account，Index is metatable
[methods] = {} 		-- 1.Push NewTable Index is methods


-- 5.lua_settable
--GLOBA[T::className] = Value(methods) 即_G.Account = {}

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

-- 注册函数
{"FunName", &Account::FunName}		-- 27. lua_pushlightuserdata
"FunName"							-- 26. push string 函数名入栈
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
