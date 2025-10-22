#ifndef _SMARTPOINTER_HEADER
#define _SMARTPOINTER_HEADER

#include <map>

#include <SDL2/SDL_stdinc.h>

namespace OtherClasses {

	template<typename PointerType>
	class SmartPointer {
	public:
		inline SmartPointer(PointerType* ptr) {
			this->OwnedPointer = ptr;
		}
		inline ~SmartPointer() {
			delete this->OwnedPointer;
		}

		inline operator PointerType*() const {
			return this->OwnedPointer;
		}

		inline PointerType* operator->() const {
			return this->OwnedPointer;
		}

	protected:
		PointerType* OwnedPointer;
	};

	static std::map<void*, Uint16> SharedPointerReferences;

	template<typename PointerType, Uint16 FreeAtReferenceCount = 0u>
	class SharedPointer {

		PointerType* OwnedPointer;

	public:
		SharedPointer(PointerType* ptr) {
			this->OwnedPointer = ptr;

			if (!SharedPointerReferences.contains((void*)ptr)) {
				SharedPointerReferences.insert({ (void*)ptr, 0 });
			}

			++SharedPointerReferences[(void*)ptr];
		}

		SharedPointer() {
			this->OwnedPointer = new PointerType;

			if (!SharedPointerReferences.contains((void*)this->OwnedPointer)) {
				SharedPointerReferences.insert({ (void*)this->OwnedPointer, 0 });
			}

			++SharedPointerReferences[(void*)this->OwnedPointer];
		}

		~SharedPointer() {
			if (--SharedPointerReferences[(void*)this->OwnedPointer] == FreeAtReferenceCount) {
				delete this->OwnedPointer;
			}
		}

		inline operator PointerType*() const noexcept {
			return this->OwnedPointer;
		}

		inline PointerType* operator->() const noexcept {
			return this->OwnedPointer;
		}
	};
}

#endif