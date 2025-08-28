#ifndef _SMARTPOINTER_HEADER
#define _SMARTPOINTER_HEADER

namespace OtherClasses {

	template<typename PointerType>
	class SmartPointer {
	public:
		inline SmartPointer(PointerType* ptr) {
			this->OwnedPointer = ptr;
		}
		inline ~SmartPointer() {
			delete[] this->OwnedPointer;
		}

		inline operator PointerType*() const {
			return this->OwnedPointer;
		}

	protected:
		PointerType* OwnedPointer;
	};
}

#endif