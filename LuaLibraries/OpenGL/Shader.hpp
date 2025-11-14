#ifndef LUALIB_OPENGL_SHADER
#define LUALIB_OPENGL_SHADER

#include <glad/glad.h>
#include "../OtherClasses/GLClasses.hpp"

#include "../LuaHelper.hpp"
#include "../Enums/OpenGLShaderTypes.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

namespace LuaLibraries::OpenGL::Shader {

	static int newUserdata(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, LUA_TNUMBER)) {
			return -1;
		}

		const GLenum ShaderType = static_cast<GLenum>(lua_tointeger(Context, 1));
		lua_settop(Context, 0);

		if (!Enums::OpenGLShaderTypes::IsValidEnum(ShaderType)) {
			return -1;
		}

		typedef _private::LuaLibraries::OpenGL::UserdataClass<GLClasses::Shader> ShaderUserdata;

		auto* Userdata = static_cast<ShaderUserdata*>(lua_newuserdata(Context, sizeof(ShaderUserdata)));
		*static_cast<GLClasses::Shader*>(Userdata) = std::move(GLClasses::Shader(ShaderType));

		luaL_getmetatable(Context, "OpenGL.Shader");
		lua_setmetatable(Context, -2);

		Userdata->IsDestroyed = false;

		return 1;
	}


	inline void Open(lua_State* Context) {

		using _private::LuaLibraries::OpenGL::UserdataClass;

		// SHADER
		luaL_newmetatable(Context, "OpenGL.Shader");

		LuaHelper::SetTableIndex(Context, "SetSource", [](lua_State* Context) {

			if (!LuaHelper::AssertArguments(Context, 1, 1, LUA_TSTRING)) {
				return -1;
			}

			auto* Shader = static_cast<GLClasses::Shader*>(lua_touserdata(Context, 1));
			Shader->SetSource(lua_tostring(Context, 2));

			lua_settop(Context, 0);
			return 1;
			});

		LuaHelper::SetTableIndex(Context, "Compile", [](lua_State* Context) {

			auto* Shader = static_cast<GLClasses::Shader*>(lua_touserdata(Context, 1));
			Shader->Compile();

			lua_settop(Context, 0);
			return 1;
			});


		LuaHelper::SetTableIndex(Context, "Destroy", [](lua_State* Context) {

			auto* Shader = static_cast<UserdataClass<GLClasses::Shader>*>(lua_touserdata(Context, 1));

			if (Shader->IsDestroyed) {
				lua_warning(Context, "::WARNING:: Called 'Destroy' on an already destroyed 'Shader' class.", true);
			} else {
				Shader->IsDestroyed = true;
				static_cast<GLClasses::Shader*>(Shader)->~Shader();
			}

			lua_settop(Context, 0);
			return 1;
			});

		LuaHelper::SetTableIndex(Context, "__gc", [](lua_State* Context) {

			auto* Shader = static_cast<UserdataClass<GLClasses::Shader>*>(lua_touserdata(Context, 1));

			if (!Shader->IsDestroyed) {
				Shader->IsDestroyed = true;
				static_cast<GLClasses::Shader*>(Shader)->~Shader();
			}

			lua_settop(Context, 0);
			return 1;
		});


		lua_pop(Context, 1);
		// END OF METATABLE

		
		// Shader Subtable

		lua_createtable(Context, 0, 1);
		LuaHelper::SetTableIndex(Context, "new", newUserdata);
		lua_setfield(Context, -2, "Shader");
	}
}

#endif