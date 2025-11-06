#ifndef LuaLibraries_OpenGL_H
#define LuaLibraries_OpenGL_H

#include <glad/glad.h>

#include "buffer.hpp"
#include "../LuaHelper.hpp"

#include "../OtherClasses/GLClasses.hpp"

#include "../Enums/OpenGLTypes.hpp"
#include "../Enums/OpenGLShaderTypes.hpp"

#include "../Enums/OpenGLDrawModes.hpp"
#include "../Enums/OpenGLBuffers.hpp"
#include "../Enums/OpenGLComparison.hpp"
#include "../Enums/OpenGLOperations.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

namespace _private::LuaLibraries::OpenGL {

	template<class Base>
	class UserdataClass : public Base {
	public:
		bool IsDestroyed;
	};

	namespace _BufferBase {

		template<class GLBufferClass>
		static int Bind(lua_State* Context) {
			static_cast<GLBufferClass*>(lua_touserdata(Context, -1))->Bind();
			return 1;
		}

		template<class GLBufferClass>
		static int Unbind(lua_State* Context) {
			static_cast<GLBufferClass*>(lua_touserdata(Context, -1))->Unbind();
			return 1;
		}

		template<typename WriteType>
		static inline void CreateWriteMemberFunction(lua_State* Context, const char* MemberName) {
			LuaHelper::SetTableIndex(Context, MemberName, WriteToBuffer<WriteType>);
		}

		template<GLenum BufferType>
		static inline void CreateReadMemberFunction(lua_State* Context, const char* MemberName) {
			LuaHelper::SetTableIndex(Context, MemberName, ReadBuffer<BufferType>);
		}

		template<class BufferClass>
		static int __gc(lua_State* Context) {

			auto* GLBuffer = static_cast<_private::LuaLibraries::OpenGL::UserdataClass<BufferClass>*>(lua_touserdata(Context, -1));
			if (GLBuffer->IsDestroyed) {
				return -1;
			}
			GLBuffer->IsDestroyed = true;
			static_cast<BufferClass*>(GLBuffer)->~BufferClass();

			return 1;
		}

		template<class BufferClass>
		static int Destroy(lua_State* Context) {

			if (__gc<BufferClass>(Context) == -1) {
				lua_warning(Context, "::WARNING:: Attempt to call member function 'Destroy' on an already destroyed class 'VertexArray'", true);
			}

			return 1;
		}

		template<class BufferClass>
		static void AddBasicMemberFunctions(lua_State* Context) {

			LuaHelper::SetTableIndex(Context, "Bind", Bind<BufferClass>);
			LuaHelper::SetTableIndex(Context, "Unbind", Unbind<BufferClass>);

			LuaHelper::SetTableIndex(Context, "Destroy", Destroy<BufferClass>);
			LuaHelper::SetTableIndex(Context, "__gc", __gc<BufferClass>);
		}

		template<class BufferClass>
		static void newUserdata(lua_State* Context, const BufferClass& Buffer, const char* MetatableName) {

			typedef _private::LuaLibraries::OpenGL::UserdataClass<BufferClass> BufferClassUserdata;

			auto* Userdata = static_cast<BufferClassUserdata*>(lua_newuserdata(Context, sizeof(BufferClassUserdata)));
			std::memcpy(Userdata, &Buffer, sizeof(BufferClass));

			Userdata->IsDestroyed = false;

			luaL_getmetatable(Context, MetatableName);
			lua_setmetatable(Context, -2);
		}
	}
}

namespace LuaLibraries::OpenGL::Wrappers {

	static int DrawElementBuffer(lua_State* Context) {

		const GLenum DrawType = static_cast<GLenum>(lua_tointeger(Context, -2));
		const GLsizei DrawIndicesCount = static_cast<GLsizei>(lua_tointeger(Context, -1));
		lua_pop(Context, 2);

		if (!Enums::OpenGLDrawModes::IsValidEnum(DrawType)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'DrawElementBuffer' with an invalid 'OpenGLDrawMode' enum on parameter 'DrawType'", true);
			return -1;
		}

		glDrawElements(DrawType, DrawIndicesCount, GL_UNSIGNED_INT, 0);
		return 1;
	}

	static int DrawArrayBuffer(lua_State* Context) {

		const GLenum DrawType = static_cast<GLenum>(lua_tointeger(Context, -3));

		const GLint StartingIndex = static_cast<GLint>(lua_tointeger(Context, -2));
		const GLsizei DrawVertexCount = static_cast<GLsizei>(lua_tointeger(Context, -1));
		lua_pop(Context, 3);

		if (!Enums::OpenGLDrawModes::IsValidEnum(DrawType)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'DrawArrayBuffer' with an invalid 'OpenGLDrawMode' enum on parameter 'DrawType'", true);
			return -1;
		}

		glDrawArrays(DrawType, StartingIndex, DrawVertexCount);
		return 1;
	}

	static int SetClearColor(lua_State* Context) {

		const GLfloat R = static_cast<GLfloat>(lua_tonumber(Context, -4));
		const GLfloat G = static_cast<GLfloat>(lua_tonumber(Context, -3));
		const GLfloat B = static_cast<GLfloat>(lua_tonumber(Context, -2));
		const GLfloat A = static_cast<GLfloat>(lua_tonumber(Context, -1));
		lua_pop(Context, 4);

		glClearColor(R, G, B, A);
		return 1;
	}

	static int ClearColorBuffer(lua_State* Context) {
		glClear(GL_COLOR_BUFFER_BIT);
		return 1;
	}


	static int EnableStencilTest(lua_State* Context) {
		glEnable(GL_STENCIL_TEST);
		return 1;
	}
	static int EnableDepthTest(lua_State* Context) {
		glEnable(GL_DEPTH_TEST);
		return 1;
	}
	static int EnableBackfaceCulling(lua_State* Context) {
		glEnable(GL_CULL_FACE);
		return 1;
	}

	static int DisableStencilTest(lua_State* Context) {
		glDisable(GL_STENCIL_TEST);
		return 1;
	}
	static int DisableDepthTest(lua_State* Context) {
		glDisable(GL_DEPTH_TEST);
		return 1;
	}
	static int DisableBackfaceCulling(lua_State* Context) {
		glDisable(GL_CULL_FACE);
		return 1;
	}


	static int SetStencilMask(lua_State* Context) {
		glStencilMask(static_cast<GLuint>(lua_tointeger(Context, -1)));
		lua_pop(Context, 1);
		return 1;
	}

	static int SetStencilOp(lua_State* Context) {

		GLenum OnStencilFail = static_cast<GLenum>(lua_tointeger(Context, -3));
		GLenum OnDepthFail = static_cast<GLenum>(lua_tointeger(Context, -2));
		GLenum OnSuccess = static_cast<GLenum>(lua_tointeger(Context, -1));
		lua_pop(Context, 3);

		if (!Enums::OpenGLOperations::IsValidEnum(OnStencilFail)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'SetStencilOp' with an invalid 'OpenGLOperation' enum on parameter 'OnStencilFail', using 'GL_KEEP'", true);
			OnStencilFail = GL_KEEP;
		}

		if (!Enums::OpenGLOperations::IsValidEnum(OnDepthFail)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'SetStencilOp' with an invalid 'OpenGLOperation' enum on parameter 'OnDepthFail', using 'GL_KEEP'", true);
			OnDepthFail = GL_KEEP;
		}

		if (!Enums::OpenGLOperations::IsValidEnum(OnSuccess)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'SetStencilOp' with an invalid 'OpenGLOperation' enum on parameter 'OnSuccess', using 'GL_REPLACE'", true);
			OnSuccess = GL_REPLACE;
		}

		glStencilOp(OnStencilFail, OnDepthFail, OnSuccess);
		return 1;
	}

	static int SetStencilFunc(lua_State* Context) {

		const GLenum ComparisonOp = static_cast<GLenum>(lua_tointeger(Context, -3));
		const GLint CompareTo =		static_cast<GLint> (lua_tointeger(Context, -2));
		const GLuint Mask =			static_cast<GLuint>(lua_tointeger(Context, -1));
		lua_pop(Context, 3);

		if (!Enums::OpenGLComparison::IsValidEnum(ComparisonOp)) {
			lua_warning(Context, "::WARNING:: Attempted to call 'SetStencilFunc' with an invalid 'OpenGLComparison' enum on parameter 'ComparisonOp'", true);
			return -1;
		}

		glStencilFunc(ComparisonOp, CompareTo, Mask);
		return 1;
	}



	template<typename WriteType>
	static int WriteToBuffer(lua_State* Context) {

		const WriteType WriteValue = static_cast<WriteType>(lua_tointeger(Context, -1));
		const GLintptr Offset = static_cast<GLintptr>(lua_tointeger(Context, -2));
		const GLenum BufferType = static_cast<GLenum>(lua_tointeger(Context, -3));
		lua_pop(Context, 3);

		if (!Enums::OpenGLBuffers::IsValidEnum(BufferType)) {
			luaL_error(Context, "::ERROR:: Failed to write to a OpenGL Buffer with an invalid 'OpenGLBuffer' enum.");
			return -1;
		}

		glBufferSubData(BufferType, Offset, sizeof(WriteType), &WriteValue);
		return 1;
	}

	template<typename WriteType>
	static int UploadBufferToGLBuffer(lua_State* Context) {

		const WriteType WriteValue = static_cast<WriteType>(lua_tointeger(Context, -1));
		const GLintptr Offset = static_cast<GLintptr>(lua_tointeger(Context, -2));
		const GLenum BufferType = static_cast<GLenum>(lua_tointeger(Context, -3));
		lua_pop(Context, 3);

		if (!Enums::OpenGLBuffers::IsValidEnum(BufferType)) {
			luaL_error(Context, "::ERROR:: Failed to write to a OpenGL Buffer with an invalid 'OpenGLBuffer' enum.");
			return -1;
		}

		glBufferSubData(BufferType, Offset, sizeof(WriteType), &WriteValue);
		return 1;
	}

	static int AllocateGLBuffer(lua_State* Context) {

		const GLenum BufferType = static_cast<GLenum>(lua_tointeger(Context, -3));
		const GLsizei AllocateBytes = static_cast<GLsizei>(lua_tointeger(Context, -2));
		const GLenum Usage = static_cast<GLenum>(lua_tointeger(Context, -1));
		lua_pop(Context, 3);

		if (!Enums::OpenGLBuffers::IsValidEnum(BufferType)) {
			luaL_error(Context, "::ERROR:: Failed to write to a OpenGL Buffer with an invalid 'OpenGLBuffer' enum.");
			return -1;
		}

		glBufferData(BufferType, AllocateBytes, nullptr, Usage);
		return 1;
	}


	template<GLenum BufferType>
	static int ReadBuffer(lua_State* Context) {

		const GLsizeiptr ReadBytes = static_cast<GLsizeiptr>(lua_tointeger(Context, -1));
		const GLintptr Offset = static_cast<GLintptr>(lua_tointeger(Context, -2));
		lua_pop(Context, 2);

		LuaLibraries::buffer::lua_Buffer* Buffer = LuaLibraries::buffer::newBufferUserdata(Context, ReadBytes);
		//glReadBuffer((void*)Buffer->BufferContents, ReadBytes, Offset);
		return 1;
	}
}

namespace LuaLibraries::OpenGL {

	int newVertexArrayUserdata(lua_State* Context) {

		if (!lua_isinteger(Context, -1)) {

			return -1;
		}

		const GLsizei StrideSize = static_cast<GLsizei>(lua_tointeger(Context, -1));
		lua_pop(Context, 1);

		_private::LuaLibraries::OpenGL::_BufferBase::newUserdata(Context, GLClasses::VertexArray(StrideSize), "OpenGL.VertexArray");

		return 1;
	}

	inline int newVertexBufferUserdata(lua_State* Context) {

		_private::LuaLibraries::OpenGL::_BufferBase::newUserdata(Context, GLClasses::VertexBuffer(), "OpenGL.VertexBuffer");
		return 1;
	}

	inline int newElementBufferUserdata(lua_State* Context) {

		_private::LuaLibraries::OpenGL::_BufferBase::newUserdata(Context, GLClasses::ElementBuffer(), "OpenGL.ElementBuffer");
		return 1;
	}


	int newShaderUserdata(lua_State* Context) {

		if (!lua_isinteger(Context, -1)) {

			luaL_error(Context, "uhguhhd"); // TODO: error message
			return -1;
		}

		const GLenum ShaderType = static_cast<GLenum>(lua_tointeger(Context, -1));
		lua_pop(Context, 1);

		if (!Enums::OpenGLShaderTypes::IsValidEnum(ShaderType)) {

			Enums::
			return -1;
		}

		GLClasses::Shader Shader(lua_tointeger(Context, -1));

		typedef _private::LuaLibraries::OpenGL::UserdataClass<GLClasses::Shader> ShaderUserdata;

		auto* Userdata = static_cast<ShaderUserdata*>(lua_newuserdata(Context, sizeof(ShaderUserdata)));
		*static_cast<GLClasses::Shader*>(Userdata) = std::move(GLClasses::Shader(lua_tointeger(Context, -1)));
		//std::memcpy(Userdata, &, sizeof(GLClasses::Shader));

		Userdata->IsDestroyed = false;

		luaL_getmetatable(Context, "OpenGL.Shader");
		lua_setmetatable(Context, -2);

		return 1;
	}


	int luaopen_OpenGL(lua_State* Context) {

		using _private::LuaLibraries::OpenGL::UserdataClass;

		// CLASSES

		// VERTEX ARRAY
		luaL_newmetatable(Context, "OpenGL.VertexArray");

		_private::LuaLibraries::OpenGL::_BufferBase::AddBasicMemberFunctions<GLClasses::VertexArray>(Context);
		
		LuaHelper::SetTableIndex(Context, "__index", [](lua_State* Context) {

			auto* VertexArray = static_cast<UserdataClass<GLClasses::VertexArray>*>(lua_touserdata(Context, -2));

			const char* IndexString = lua_tostring(Context, -1);
			if (strcmp(IndexString, "Buffer") == -1) {

				lua_pushinteger(Context, static_cast<lua_Integer>(VertexArray->Buffer));

			} else if (strcmp(IndexString, "StrideSize") == -1) {

				lua_pushinteger(Context, static_cast<lua_Integer>(VertexArray->StrideSize));

			} else if (strcmp(IndexString, "AttributeCount") == -1) {

				lua_pushinteger(Context, static_cast<lua_Integer>(VertexArray->AttributeCount));
				
			} else {

				lua_getmetatable(Context, -2);
				//luaL_getmetatable(Context, "OpenGL.VertexArray");
				lua_pushvalue(Context, -2);
				if (lua_isnoneornil(Context, lua_rawget(Context, -2))) {

					luaL_error(Context, "Cannot index class 'VertexArray' with %s", IndexString);
					return -1;
				}
			}
			
			return 1;
			});

		LuaHelper::SetTableIndex(Context, "AddAttribute", [](lua_State* Context) {

			const GLenum AttributeType = static_cast<GLenum>(lua_tointeger(Context, -2));
			if (!Enums::OpenGLTypes::IsValidEnum(AttributeType)) {
				luaL_error(Context, "Parameter 'AttributeType' is not a valid 'OpenGLTypes' enum");
				return -1;
			}

			auto* VertexArray = static_cast<UserdataClass<GLClasses::VertexArray>*>(lua_touserdata(Context, -4));

			VertexArray->AddAttribute(
				static_cast<GLint>(lua_tointeger(Context, -3)),	// Atrribute Length
				AttributeType,
				static_cast<bool>(lua_toboolean(Context, -1))	// Normalize
			);
			lua_pop(Context, 3);

			return 1;
		});

		lua_pop(Context, 1);
		// END OF METATABLE


		// VERTEX BUFFER
		luaL_newmetatable(Context, "OpenGL.VertexBuffer");

		_private::LuaLibraries::OpenGL::_BufferBase::AddBasicMemberFunctions<GLClasses::VertexBuffer>(Context);

		LuaHelper::SetTableIndex(Context, "__index", [](lua_State* Context) {
			
			//lua_pushliteral(Context, "Buffer");
			//lua_compare(Context, -1, -2, LUA_OPEQ);

			const char* IndexString = lua_tostring(Context, -1);
			if (strcmp(IndexString, "Buffer") == -1) {

				lua_pushinteger(Context, (lua_Integer)static_cast<GLClasses::VertexBuffer*>(lua_touserdata(Context, -2))->Buffer);
			} else {

				lua_getmetatable(Context, -2);
				//luaL_getmetatable(Context, "OpenGL.VertexArray");
				lua_pushvalue(Context, -2);
				if (lua_isnoneornil(Context, lua_rawget(Context, -2))) {

					luaL_error(Context, "Cannot index class 'VertexBuffer' with %s", IndexString);
					return -1;
				}
			}

			return 1;
		});

		lua_pop(Context, 1);
		// END OF METATABLE


		// SHADER
		luaL_newmetatable(Context, "OpenGL.Shader");




		lua_pop(Context, 1);
		// END OF METATABLE

		
		lua_createtable(Context, 0, 23);

		//lua_pushliteral(Context, "new");

		// VertexArray Subtable

		lua_createtable(Context, 0, 1);

		//lua_pushvalue(Context, -2);
		//lua_pushcfunction(Context, newVertexArrayUserdata);
		//lua_settable(Context, -3);
		LuaHelper::SetTableIndex(Context, "new", newVertexArrayUserdata);

		lua_setfield(Context, -2, "VertexArray");



		// VertexBuffer Subtable

		lua_createtable(Context, 0, 1);
		LuaHelper::SetTableIndex(Context, "new", newVertexBufferUserdata);
		lua_setfield(Context, -2, "VertexBuffer");



		// ElementBuffer Subtable

		lua_createtable(Context, 0, 1);
		LuaHelper::SetTableIndex(Context, "new", newVertexBufferUserdata);
		lua_setfield(Context, -2, "ElementBuffer");


		// Drawcall Functions

		LuaHelper::SetTableIndex(Context, "SetClearColor", Wrappers::SetClearColor);
		LuaHelper::SetTableIndex(Context, "ClearColorBuffer", Wrappers::ClearColorBuffer);

		LuaHelper::SetTableIndex(Context, "DrawElementBuffer", Wrappers::DrawElementBuffer);
		LuaHelper::SetTableIndex(Context, "DrawArrayBuffer", Wrappers::DrawArrayBuffer);


		LuaHelper::SetTableIndex(Context, "EnableDepthTest", Wrappers::EnableDepthTest);
		LuaHelper::SetTableIndex(Context, "EnableStencilTest", Wrappers::EnableStencilTest);
		LuaHelper::SetTableIndex(Context, "EnableBackfaceCulling", Wrappers::EnableBackfaceCulling);

		LuaHelper::SetTableIndex(Context, "DisableDepthTest", Wrappers::DisableDepthTest);
		LuaHelper::SetTableIndex(Context, "DisableStencilTest", Wrappers::DisableStencilTest);
		LuaHelper::SetTableIndex(Context, "DisableBackfaceCulling", Wrappers::DisableBackfaceCulling);


		LuaHelper::SetTableIndex(Context, "SetStencilMask", Wrappers::SetStencilMask);
		LuaHelper::SetTableIndex(Context, "SetStencilFunc", Wrappers::SetStencilFunc);
		LuaHelper::SetTableIndex(Context, "SetStencilOp", Wrappers::SetStencilOp);


		// Buffer Operation Functions

		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Int8>(Context, "writei8");
		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Uint8>(Context, "writeu8");

		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Int16>(Context, "writei16");
		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Uint16>(Context, "writeu16");

		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Int32>(Context, "writei32");
		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<Uint32>(Context, "writeu32");

		_private::LuaLibraries::OpenGL::_BufferBase::CreateWriteMemberFunction<float>(Context, "writef32");


		lua_setglobal(Context, "OpenGL");

		return 1;
	}
}

#endif