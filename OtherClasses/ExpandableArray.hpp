#ifndef _EXPANDABLEARRAY_CLASS
#define _EXPANDABLEARRAY_CLASS

#include <minmax.h>
#include <cstring>

namespace OtherClasses {

	template<typename ElementType, typename IndexType = size_t>
	class ExpandableArray {

		ElementType* ArrayPointer;
		IndexType ReservedBlocks;

	public:
		inline ExpandableArray(IndexType Length = 1) {
			this->ArrayPointer = new ElementType[Length];
			this->ReservedBlocks = Length;
		}

		inline ~ExpandableArray() {
			delete[] this->ArrayPointer;
		}

		inline ElementType& operator[](IndexType Index) {
			return this->ArrayPointer[Index];
		}
		inline const ElementType& operator[](IndexType Index) const {
			return this->ArrayPointer[Index];
		}

		ElementType* Find(ElementType& LookFor) {

			for (IndexType i = 0; i < this->ReservedBlocks; ++i) {

				if (&this->ArrayPointer[i] == LookFor) {
					return &this->ArrayPointer[i];
				}
			}

			return nullptr;
		}

		void SetSizeTo(IndexType NewSize) {

			ElementType* ResizedBuffer = new ElementType[NewSize];
			std::memcpy(ResizedBuffer, this->ArrayPointer, static_cast<size_t>(min(NewSize, this->ReservedBlocks)) * sizeof(ElementType));

			delete[] this->ArrayPointer;
			this->ArrayPointer = ResizedBuffer;

			this->ReservedBlocks = NewSize;
		}

		inline void ReserveMoreBlocks(IndexType ReserveCount) {
			this->SetSizeTo(this->ReservedBlocks + ReserveCount);
		}

		inline IndexType ReservedSize() const {
			return this->ReservedBlocks;
		}

		inline void Fill(IndexType StartAt, IndexType StopAt, const ElementType& With) {

			for (IndexType i = StartAt; i < StopAt; ++i) {
				this->ArrayPointer[i] = With;
			}
		}
		inline void Fill(const ElementType& With) {
			this->Fill(0, this->ReservedBlocks, With);
		}
	};
}

#endif