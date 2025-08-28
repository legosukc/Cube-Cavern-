#ifndef _BINDABLEEVENT_HEADER
#define _BINDABLEEVENT_HEADER

#include <vector>

namespace LuauClasses {

	template<typename ConnectionFuncInputType = void*>
	class BindableEvent {

		typedef void(ConnectionFuncType)(void* Connector, ConnectionFuncInputType);

		typedef struct {
			ConnectionFuncType* FunctionPtr;
			void* Connector; // optional
			bool DisconnectAfterInvoke;
		} ConnectionStruct;

		std::vector<ConnectionStruct> Connections;

	public:

		void Invoke(ConnectionFuncInputType PassToConnections) {

			for (ConnectionStruct& Connection : this->Connections) {

				if (Connection.FunctionPtr != nullptr) {
					Connection.FunctionPtr(Connection.Connector, PassToConnections);

					if (Connection.DisconnectAfterInvoke) {
						Connection.FunctionPtr = nullptr;
					}
				}
			}
		}

		inline void Disconnect(size_t ConnectionID) {
			this->Connections.at(ConnectionID) = ConnectionStruct{ NULL, NULL, NULL };
		}
		
		inline size_t Connect(void(FunctionPtr)(void* Connector, ConnectionFuncInputType), void* Connector = nullptr, bool DisconnectAfterInvoke = false) noexcept {

			this->Connections.push_back(
				ConnectionStruct{
					FunctionPtr,
					Connector,
					DisconnectAfterInvoke
				}
			);

			return this->Connections.size();
		}
	};
}

#endif