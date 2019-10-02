#include "StackAllocator.h"

#include "MemoryManager.h"
#include "Allocators.h"

namespace r1l00
{

	StackAllocator::StackAllocator(size_t memSize, const void* mem, R1_ALLOCATOR type) :
		Allocator(memSize, mem, type)
	{}

	StackAllocator::~StackAllocator()
	{
		this->clear();
	}

	void* StackAllocator::allocate(size_t memSize, U8 alignment)
	{
		assert(memSize > 0 && "allocate called with memSize = 0.");

		union
		{
			void* asVoidPtr;
			UPtr asUptr;
			AllocMetaInfo* asMeta;
		};

		asVoidPtr = (void*)this->m_pMemoryFirstAddress;

		// current address
		asUptr += this->m_MemoryUsed;

		U8 adjustment = GetAdjustment(asVoidPtr, alignment, sizeof(AllocMetaInfo));

		// check if there is enough memory available
		if (this->m_MemoryUsed + memSize + adjustment > this->m_MemorySize)
		{
			// not enough memory
			return reallocate((this->m_MemoryUsed + memSize + adjustment) - this->m_MemorySize);
		}

		// store alignment in allocation meta info
		asMeta->adjustment = adjustment;

		// determine aligned memory address
		asUptr += adjustment;

		// update book keeping
		this->m_MemoryUsed += memSize + adjustment;
		this->m_MemoryAllocations++;

		// return aligned memory address
		return asVoidPtr;
	}

	void* StackAllocator::reallocate(size_t diff)
	{
		size_t partsToAllocate = diff / MemoryManager::MEMORY_CAPACITY + 1;

		MemoryManager::staticInstance().ReAllocate(partsToAllocate * MemoryManager::MEMORY_CAPACITY);

		//const void* oldMemoryPointer = m_pMemoryFirstAddress;

		//void* newMem = malloc(MemoryManager::MEMORY_CAPACITY * partsToAllocate);

		//assert(newMem != nullptr && "Failed to reallocate global memory.");

		//memcpy(newMem, oldMemoryPointer, GetMemorySize());

		return 0;
	}

	void StackAllocator::free(void* mem)
	{
		union
		{
			void* asVoidPtr;
			UPtr asUptr;
			AllocMetaInfo* asMeta;
		};

		asVoidPtr = mem;

		// get meta info
		asUptr -= sizeof(AllocMetaInfo);

		// free used memory
		this->m_MemoryUsed -= ((UPtr)this->m_pMemoryFirstAddress + this->m_MemoryUsed) - ((UPtr)mem + asMeta->adjustment);

		// decrement allocation count
		this->m_MemoryAllocations--;
	}

	void StackAllocator::clear()
	{
		// simply reset memory
		this->m_MemoryUsed = 0;
		this->m_MemoryAllocations = 0;
	}

}
