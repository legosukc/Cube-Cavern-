#ifndef _TASK_HEADER
#define _TASK_HEADER

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

template <typename DataType = void*>
struct TaskStruct {
	void(*FunctionPtr)(DataType Data);
	DataType Data;

	Uint64 ExecuteAtMillisecond;

	size_t TaskID;
};

static std::vector<TaskStruct<>*> Tasks;

namespace LuauClasses::task {

	template <typename DataType = void*>
	TaskStruct<DataType>* CreateDelayedTask(void(*FunctionPtr)(DataType Data), DataType Data, Uint64 DelayByMilliseconds) {

		TaskStruct<DataType>* DelayedTask = new TaskStruct<DataType>;
		DelayedTask->FunctionPtr = FunctionPtr;
		DelayedTask->Data = Data;
		DelayedTask->ExecuteAtMillisecond = SDL_GetTicks64() + DelayByMilliseconds;
		DelayedTask->TaskID = Tasks.size();

		Tasks.push_back((TaskStruct<>*)DelayedTask);

		return DelayedTask;
	}

	template <typename DataType = void*>
	constexpr SDL_FORCE_INLINE TaskStruct<DataType>* CreateDelayedTask(void(*FunctionPtr)(DataType Data), DataType Data, float DelayBySeconds) {
		return CreateDelayedTask(FunctionPtr, Data, static_cast<Uint64>(DelayBySeconds * 1000.f));
	}

	template <typename DataType = void*>
	SDL_FORCE_INLINE TaskStruct<DataType>* CreateDeferedTask(void(*FunctionPtr)(DataType Data), DataType Data) {
		return CreateDelayedTask<DataType>(FunctionPtr, Data, 0ui64);
	}

	void CancelTask(TaskStruct<>* CancellingTask) {
		Tasks.at(CancellingTask->TaskID) = nullptr;
		delete CancellingTask;
	}

	SDL_FORCE_INLINE void Update() {

		for (auto& Task : Tasks) {

			if (Task == nullptr)
				continue;

			if (SDL_GetTicks64() >= Task->ExecuteAtMillisecond) {
				Task->FunctionPtr(Task->Data);
				CancelTask(Task);
			}
		}
	}
}

#endif