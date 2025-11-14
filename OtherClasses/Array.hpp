#ifndef _ARRAY_CLASS
#define _ARRAY_CLASS

#include <cstring>
#include <cassert>

namespace OtherClasses {

	template<typename ElementType, size_t ElementCount>
	class Array {

		static_assert(ElementCount != 0);

		ElementType ArrayBlock[ElementCount];
		size_t ArraySize = ElementCount;

	public:
		
		constexpr inline Array(ElementType[] CopyArray) {
			std::memcpy(this->ArrayBlock, CopyArray, ElementCount);
		}

		inline ElementType& operator[](int Index) {
			return this->ArrayBlock[Index];
		}
		inline const ElementType& operator[](int Index) const {
			return this->ArrayBlock[Index];
		}

		inline size_t size() const {
			return this->ArraySize;
		}
	};
}

#endif