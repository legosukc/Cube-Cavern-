#ifndef LUALIB_OPENGL_BUFFERBASE
#define LUALIB_OPENGL_BUFFERBASE

#include <glad/glad.h>

#include "../LibraryBase.hpp"

#include "../../define.h"

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

namespace _private::LuaLibraries::Metatables::OpenGL {

	template<class GLClass>
	class BufferBase {
	protected:

		using UserdataClass = _private::LuaLibraries::OpenGL::UserdataClass<GLClass>;

		static int Bind(lua_State* Context) {
			static_cast<GLClass*>(lua_touserdata(Context, 1))->Bind();
			lua_settop(Context, 0);
			return 1;
		}

		static int Unbind(lua_State* Context) {
			static_cast<GLClass*>(lua_touserdata(Context, 1))->Unbind();
			lua_settop(Context, 0);
			return 1;
		}

		static int __gc(lua_State* Context) {

			auto* GLBuffer = static_cast<UserdataClass*>(lua_touserdata(Context, 1));

			if (!GLBuffer->IsDestroyed) {
				GLBuffer->IsDestroyed = true;
				static_cast<GLClass*>(GLBuffer)->~GLClass();
			}

			lua_settop(Context, 0);
			return 1;
		}

		static int Destroy(lua_State* Context) {

			auto* GLBuffer = static_cast<UserdataClass*>(lua_touserdata(Context, 1));

			unlikely_branch
			if (GLBuffer->IsDestroyed) {
				lua_warning(Context, "::WARNING:: Attempt to call member function 'Destroy' on an already destroyed class.", true);
			} else {
				GLBuffer->IsDestroyed = true;
				static_cast<GLClass*>(GLBuffer)->~GLClass();
			}

			lua_settop(Context, 0);
			return 1;
		}

		static int __index(lua_State* Context);

	public:

		using OpenGLBufferClass = GLClass;

		static inline const char* MetatableName;

		static void Open(lua_State* Context) {

			luaL_newmetatable(Context, MetatableName);

			LuaHelper::SetTableIndex(Context, "Bind", Bind);
			LuaHelper::SetTableIndex(Context, "Unbind", Unbind);

			LuaHelper::SetTableIndex(Context, "Destroy", Destroy);
			LuaHelper::SetTableIndex(Context, "__gc", __gc);

			//_private::LuaLibraries::OpenGL::_BufferBase::AddBasicMemberFunctions<GLClass>(Context);

			LuaHelper::SetTableIndex(Context, "__index", __index);

			lua_pop(Context, 1);
		}
	};
}

namespace _private::LuaLibraries::OpenGL {

	template<class Base>
	class UserdataClass : public Base {
	public:
		bool IsDestroyed;
	};

	template<class Metatable>
	class BufferBase : LuaHelper::Table::LiteralTable<LuaHelper::Table::KeyValue<lua_CFunction>> { //LuaLibraries::LibraryBase {
	protected:

		template<typename WriteType>
		inline void CreateWriteMemberFunction(lua_State* Context, const char* MemberName) {
			LuaHelper::SetTableIndex(Context, MemberName, WriteToBuffer<WriteType>);
		}

		template<GLenum BufferType>
		inline void CreateReadMemberFunction(lua_State* Context, const char* MemberName) {
			LuaHelper::SetTableIndex(Context, MemberName, ReadBuffer<BufferType>);
		}

	public:

		static void newUserdata(lua_State* Context, const Metatable::OpenGLBufferClass& Buffer) {

			using BufferClassUserdata = UserdataClass<Metatable::OpenGLBufferClass>;

			auto* Userdata = static_cast<BufferClassUserdata*>(lua_newuserdata(Context, sizeof(BufferClassUserdata)));
			//std::memcpy(Userdata, &Buffer, sizeof(BufferClass));
			*static_cast<Metatable::OpenGLBufferClass*>(Userdata) = std::move(Buffer);

			Userdata->IsDestroyed = false;

			luaL_getmetatable(Context, Metatable::MetatableName);
			lua_setmetatable(Context, -2);
		}

		static inline ElementType Entries[] = {
			{"new", newUserdata}
		}
			/*
		static inline LuaLibraries::LibraryEntry Entries[] = {
			{"new", newUserdata}
		};*/

		static void Open(lua_State* Context) {
			AddToTable(Context);
			LuaLibraries::AddLibraryToTable<Derived>(Context);
		}
	};

	namespace _BufferBase {

		template<class GLBufferClass>
		int Bind(lua_State* Context) {
			static_cast<GLBufferClass*>(lua_touserdata(Context, 1))->Bind();
			lua_settop(Context, 0);
			return 1;
		}

		template<class GLBufferClass>
		int Unbind(lua_State* Context) {
			static_cast<GLBufferClass*>(lua_touserdata(Context, 1))->Unbind();
			lua_settop(Context, 0);
			return 1;
		}

		template<typename WriteType>
		inline void CreateWriteMemberFunction(lua_State* Context, const char* MemberName) {
			LuaHelper::SetTableIndex(Context, MemberName, WriteToBuffer<WriteType>);
		}

		template<GLenum BufferType>
		inline void CreateReadMemberFunction(lua_State* Context, const char* MemberName) {
			LuaHelper::SetTableIndex(Context, MemberName, ReadBuffer<BufferType>);
		}

		template<class BufferClass>
		int __gc(lua_State* Context) {

			auto* GLBuffer = static_cast<_private::LuaLibraries::OpenGL::UserdataClass<BufferClass>*>(lua_touserdata(Context, 1));

			if (!GLBuffer->IsDestroyed) {
				GLBuffer->IsDestroyed = true;
				static_cast<BufferClass*>(GLBuffer)->~BufferClass();
			}
			
			lua_settop(Context, 0);
			return 1;
		}

		template<class BufferClass>
		int Destroy(lua_State* Context) {

			auto* GLBuffer = static_cast<_private::LuaLibraries::OpenGL::UserdataClass<BufferClass>*>(lua_touserdata(Context, 1));

			if (GLBuffer->IsDestroyed) {
				lua_warning(Context, "::WARNING:: Attempt to call member function 'Destroy' on an already destroyed class.", true);
			} else {
				GLBuffer->IsDestroyed = true;
				static_cast<BufferClass*>(GLBuffer)->~BufferClass();
			}

			lua_settop(Context, 0);
			return 1;
		}

		template<class BufferClass>
		void AddBasicMemberFunctions(lua_State* Context) {

			LuaHelper::SetTableIndex(Context, "Bind", Bind<BufferClass>);
			LuaHelper::SetTableIndex(Context, "Unbind", Unbind<BufferClass>);

			LuaHelper::SetTableIndex(Context, "Destroy", Destroy<BufferClass>);
			LuaHelper::SetTableIndex(Context, "__gc", __gc<BufferClass>);
		}

		template<class BufferClass>
		void newUserdata(lua_State* Context, const BufferClass& Buffer, const char* MetatableName) {

			typedef _private::LuaLibraries::OpenGL::UserdataClass<BufferClass> BufferClassUserdata;

			auto* Userdata = static_cast<BufferClassUserdata*>(lua_newuserdata(Context, sizeof(BufferClassUserdata)));
			//std::memcpy(Userdata, &Buffer, sizeof(BufferClass));
			*static_cast<BufferClass*>(Userdata) = std::move(Buffer);

			Userdata->IsDestroyed = false;

			luaL_getmetatable(Context, MetatableName);
			lua_setmetatable(Context, -2);
		}
	}
}

#endif