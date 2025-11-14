#ifndef LUALIB_OPENGL_WRAPPERS
#define LUALIB_OPENGL_WRAPPERS

#include "../LibraryBase.hpp"

#include <glad/glad.h>

#include "../buffer.hpp"
#include "../../LuaHelper.hpp"

#include "../../Enums/OpenGLDrawModes.hpp"
#include "../../Enums/OpenGLBuffers.hpp"
#include "../../Enums/OpenGLComparison.hpp"
#include "../../Enums/OpenGLOperations.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include "../../define.h"

namespace LuaLibraries::OpenGL {

	class Wrappers : public LuaLibraries::LibraryBase {

		static int DrawElementBuffer(lua_State* Context);

		static int DrawArrayBuffer(lua_State* Context);

		static int SetClearColor(lua_State* Context);

		static inline int ClearColorBuffer(lua_State* Context);


		template<GLenum Enum>
		static inline int EnableBase(lua_State* Context);

		template<GLenum Enum>
		static inline int DisableBase(lua_State* Context);


		static int SetStencilMask(lua_State* Context);

		static int SetStencilOp(lua_State* Context);

		static int SetStencilFunc(lua_State* Context);



		template<typename WriteType>
		static int WriteToBuffer(lua_State* Context);

		template<typename WriteType>
		static int UploadBufferToGLBuffer(lua_State* Context);

		static int AllocateGLBuffer(lua_State* Context);


		template<GLenum BufferType>
		static int ReadBuffer(lua_State* Context);

	public:

		static inline const char* Name = "Wrappers";

		static inline LibraryEntry Entries[] = {
			{"SetClearColor", SetClearColor},
			{"ClearColorBuffer", ClearColorBuffer},

			{"DrawElementBuffer", DrawElementBuffer},
			{"DrawArrayBuffer", DrawArrayBuffer},


			{"EnableDepthTest", EnableBase<GL_DEPTH_TEST>},
			{"EnableStencilTest", EnableBase<GL_STENCIL_TEST>},
			{"EnableBackfaceCulling", EnableBase<GL_CULL_FACE>},

			{"DisableDepthTest", DisableBase<GL_DEPTH_TEST>},
			{"DisableStencilTest", DisableBase<GL_STENCIL_TEST>},
			{"DisableBackfaceCulling", DisableBase<GL_CULL_FACE>},


			{"SetStencilMask", SetStencilMask},
			{"SetStencilFunc", SetStencilFunc},
			{"SetStencilOp", SetStencilOp}
		};

		static inline void Open(lua_State* Context) {
			LuaLibraries::AddLibraryToTable<Wrappers>(Context);
		}
	};

	int Wrappers::DrawElementBuffer(lua_State* Context) {

		unlikely_branch
		if (!LuaHelper::AssertArguments(Context, 0, 2, LUA_TNUMBER, LUA_TNUMBER)) {
			return -1;
		}

		const GLenum DrawType = static_cast<GLenum>(lua_tointeger(Context, 1));
		const GLsizei DrawIndicesCount = static_cast<GLsizei>(lua_tointeger(Context, 2));
		lua_settop(Context, 0);

		unlikely_branch
		if (!Enums::OpenGLDrawModes::IsValidEnum(DrawType)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'DrawElementBuffer' with an invalid 'OpenGLDrawMode' enum on parameter 'DrawType'", true);
			return 1;
		}

		glDrawElements(DrawType, DrawIndicesCount, GL_UNSIGNED_INT, 0);
		return 1;
	}

	int Wrappers::DrawArrayBuffer(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, 3, LUA_TNUMBER, LUA_TNUMBER, LUA_TNUMBER)) {
			return -1;
		}

		const GLenum DrawType = static_cast<GLenum>(lua_tointeger(Context, 1));

		const GLint StartingIndex = static_cast<GLint>(lua_tointeger(Context, 2));
		const GLsizei DrawVertexCount = static_cast<GLsizei>(lua_tointeger(Context, 3));
		lua_settop(Context, 0);

		if (!Enums::OpenGLDrawModes::IsValidEnum(DrawType)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'DrawArrayBuffer' with an invalid 'OpenGLDrawMode' enum on parameter 'DrawType'", true);
			return -1;
		}

		glDrawArrays(DrawType, StartingIndex, DrawVertexCount);
		return 1;
	}

	int Wrappers::SetClearColor(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, 4, LUA_TNUMBER, LUA_TNUMBER, LUA_TNUMBER, LUA_TNUMBER)) {
			return -1;
		}

		const GLfloat R = static_cast<GLfloat>(lua_tonumber(Context, 1));
		const GLfloat G = static_cast<GLfloat>(lua_tonumber(Context, 2));
		const GLfloat B = static_cast<GLfloat>(lua_tonumber(Context, 3));
		const GLfloat A = static_cast<GLfloat>(lua_tonumber(Context, 4));
		lua_settop(Context, 0);

		glClearColor(R, G, B, A);
		return 1;
	}

	int Wrappers::ClearColorBuffer(lua_State* Context) {
		glClear(GL_COLOR_BUFFER_BIT);
		lua_settop(Context, 0);
		return 1;
	}


	template<GLenum Enum>
	int Wrappers::EnableBase(lua_State* Context) {
		glEnable(Enum);
		lua_settop(Context, 0);
		return 1;
	}

	template<GLenum Enum>
	int Wrappers::DisableBase(lua_State* Context) {
		glDisable(Enum);
		lua_settop(Context, 0);
		return 1;
	}


	int Wrappers::SetStencilMask(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, 1, LUA_TNUMBER)) {
			return -1;
		}

		glStencilMask(static_cast<GLuint>(lua_tointeger(Context, 1)));
		lua_settop(Context, 0);
		return 1;
	}

	int Wrappers::SetStencilOp(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, 3, LUA_TNUMBER, LUA_TNUMBER, LUA_TNUMBER)) {
			return -1;
		}

		GLenum OnStencilFail = static_cast<GLenum>(lua_tointeger(Context, 1));
		GLenum OnDepthFail = static_cast<GLenum>(lua_tointeger(Context, 2));
		GLenum OnSuccess = static_cast<GLenum>(lua_tointeger(Context, 3));
		lua_settop(Context, 0);

		if (!Enums::OpenGLOperations::IsValidEnum(OnStencilFail)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'SetStencilOp' with an invalid 'OpenGLOperation' enum on argument 1: 'OnStencilFail'. Using 'GL_KEEP'", true);
			OnStencilFail = GL_KEEP;
		}

		if (!Enums::OpenGLOperations::IsValidEnum(OnDepthFail)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'SetStencilOp' with an invalid 'OpenGLOperation' enum on argument 2: 'OnDepthFail'. Using 'GL_KEEP'", true);
			OnDepthFail = GL_KEEP;
		}

		if (!Enums::OpenGLOperations::IsValidEnum(OnSuccess)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'SetStencilOp' with an invalid 'OpenGLOperation' enum on argument 3: 'OnSuccess'. Using 'GL_REPLACE'", true);
			OnSuccess = GL_REPLACE;
		}

		glStencilOp(OnStencilFail, OnDepthFail, OnSuccess);
		return 1;
	}

	int Wrappers::SetStencilFunc(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, 3, LUA_TNUMBER, LUA_TNUMBER, LUA_TNUMBER)) {
			return -1;
		}

		const GLenum ComparisonOp = static_cast<GLenum>(lua_tointeger(Context, 1));
		const GLint CompareTo = static_cast<GLint> (lua_tointeger(Context, 2));
		const GLuint Mask = static_cast<GLuint>(lua_tointeger(Context, 3));
		lua_settop(Context, 0);

		if (!Enums::OpenGLComparison::IsValidEnum(ComparisonOp)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'SetStencilFunc' with an invalid 'OpenGLComparison' enum on parameter 'ComparisonOp'", true);
			return 1;
		}

		glStencilFunc(ComparisonOp, CompareTo, Mask);
		return 1;
	}



	template<typename WriteType>
	int Wrappers::WriteToBuffer(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, 3, LUA_TNUMBER, LUA_TNUMBER, LUA_TNUMBER)) {
			return -1;
		}

		const WriteType WriteValue = static_cast<WriteType>(lua_tointeger(Context, 3));
		const GLintptr Offset = static_cast<GLintptr>(lua_tointeger(Context, 2));
		const GLenum BufferType = static_cast<GLenum>(lua_tointeger(Context, 1));
		lua_settop(Context, 0);

		if (!Enums::OpenGLBuffers::IsValidEnum(BufferType)) {
			luaL_error(Context, "::ERROR:: Failed to write to a OpenGL Buffer with an invalid 'OpenGLBuffer' enum.");
			return -1;
		}

		glBufferSubData(BufferType, Offset, sizeof(WriteType), &WriteValue);
		return 1;
	}

	template<typename WriteType>
	int Wrappers::UploadBufferToGLBuffer(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, 3, LUA_TNUMBER, LUA_TNUMBER, LUA_TNUMBER)) {
			return -1;
		}

		const WriteType WriteValue = static_cast<WriteType>(lua_tointeger(Context, 3));
		const GLintptr Offset = static_cast<GLintptr>(lua_tointeger(Context, 2));
		const GLenum BufferType = static_cast<GLenum>(lua_tointeger(Context, 1));
		lua_settop(Context, 0);

		if (!Enums::OpenGLBuffers::IsValidEnum(BufferType)) {
			luaL_error(Context, "::ERROR:: Failed to write to a OpenGL Buffer with an invalid 'OpenGLBuffer' enum.");
			return -1;
		}

		glBufferSubData(BufferType, Offset, sizeof(WriteType), &WriteValue);
		return 1;
	}

	int Wrappers::AllocateGLBuffer(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, 3, LUA_TNUMBER, LUA_TNUMBER, LUA_TNUMBER)) {
			return -1;
		}

		const GLenum BufferType = static_cast<GLenum>(lua_tointeger(Context, 1));
		const GLsizei AllocateBytes = static_cast<GLsizei>(lua_tointeger(Context, 2));
		const GLenum Usage = static_cast<GLenum>(lua_tointeger(Context, 3));
		lua_settop(Context, 0);

		if (!Enums::OpenGLBuffers::IsValidEnum(BufferType)) {
			luaL_error(Context, "::ERROR:: Failed to write to a OpenGL Buffer with an invalid 'OpenGLBuffer' enum.");
			return -1;
		}

		glBufferData(BufferType, AllocateBytes, nullptr, Usage);
		return 1;
	}


	template<GLenum BufferType>
	int Wrappers::ReadBuffer(lua_State* Context) {

		if (!LuaHelper::AssertArguments(Context, 0, 3, LUA_TNUMBER, LUA_TNUMBER, LUA_TNUMBER)) {
			return -1;
		}

		const GLenum Target = static_cast<GLenum>(lua_tointeger(Context, 1));
		const GLintptr Offset = static_cast<GLintptr>(lua_tointeger(Context, 2));
		const GLsizeiptr ReadBytes = static_cast<GLsizeiptr>(lua_tointeger(Context, 3));
		lua_settop(Context, 0);

		LuaLibraries::buffer::lua_Buffer* Buffer = LuaLibraries::buffer::newBufferUserdata(Context, ReadBytes);
		glGetBufferSubData(Target, Offset, ReadBytes, static_cast<void*>(Buffer->BufferContents));
		return 1;
	}

	/*
	inline void Open(lua_State* Context) {

		// Drawcall Functions

		LuaHelper::SetTableIndex(Context, "SetClearColor", Wrappers::SetClearColor);
		LuaHelper::SetTableIndex(Context, "ClearColorBuffer", Wrappers::ClearColorBuffer);

		LuaHelper::SetTableIndex(Context, "DrawElementBuffer", Wrappers::DrawElementBuffer);
		LuaHelper::SetTableIndex(Context, "DrawArrayBuffer", Wrappers::DrawArrayBuffer);


		LuaHelper::SetTableIndex(Context, "EnableDepthTest", Wrappers::EnableBase<GL_DEPTH_TEST>);
		LuaHelper::SetTableIndex(Context, "EnableStencilTest", Wrappers::EnableBase<GL_STENCIL_TEST>);
		LuaHelper::SetTableIndex(Context, "EnableBackfaceCulling", Wrappers::EnableBase<GL_CULL_FACE>);

		LuaHelper::SetTableIndex(Context, "DisableDepthTest", Wrappers::DisableBase<GL_DEPTH_TEST>);
		LuaHelper::SetTableIndex(Context, "DisableStencilTest", Wrappers::DisableBase<GL_STENCIL_TEST>);
		LuaHelper::SetTableIndex(Context, "DisableBackfaceCulling", Wrappers::DisableBase<GL_CULL_FACE>);


		LuaHelper::SetTableIndex(Context, "SetStencilMask", Wrappers::SetStencilMask);
		LuaHelper::SetTableIndex(Context, "SetStencilFunc", Wrappers::SetStencilFunc);
		LuaHelper::SetTableIndex(Context, "SetStencilOp", Wrappers::SetStencilOp);


		// Buffer Operation Functions
		/*
		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Int8>(Context, "writei8");
		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Uint8>(Context, "writeu8");

		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Int16>(Context, "writei16");
		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Uint16>(Context, "writeu16");

		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Int32>(Context, "writei32");
		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Uint32>(Context, "writeu32");

		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<float>(Context, "writef32");*/
	//}
}

#endif