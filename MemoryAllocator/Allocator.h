#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "Platform.h"

namespace r1l00
{

	// returns address aligned
	static inline void* AlignForward(void* address, U8 alignment)
	{
		return (void*)((reinterpret_cast<UPtr>(address) + static_cast<UPtr>(alignment - 1))& static_cast<UPtr>(~(alignment - 1)));
	}

	// returns the number of bytes needed to align the address
	static inline U8 GetAdjustment(const void* address, U8 alignment)
	{
		U8 adjustment = alignment - (reinterpret_cast<UPtr>(address)& static_cast<UPtr>(alignment - 1));

		return adjustment == alignment ? 0 : adjustment;
	}

	static inline U8 GetAdjustment(const void* address, U8 alignment, U8 extra)
	{
		U8 adjustment = GetAdjustment(address, alignment);

		U8 neededSpace = extra;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			//Increase adjustment to fit header
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0)
				adjustment += alignment;
		}

		return adjustment;
	}

	class Allocator
	{
	protected:

		const size_t m_MemorySize;
		const void* m_pMemoryFirstAddress;

		size_t m_MemoryUsed;
		U64 m_MemoryAllocations;

		R1_ALLOCATOR m_Type;

	public:
		Allocator(const size_t memSize, const void* mem, R1_ALLOCATOR type) :
			m_MemorySize(memSize),
			m_pMemoryFirstAddress(mem),
			m_MemoryUsed(0),
			m_MemoryAllocations(0),
			m_Type(type)
		{}
		virtual ~Allocator()
		{}

		virtual void* allocate(size_t size, U8 alignment) = 0;
		virtual void* reallocate(size_t diff) = 0;
		virtual void free(void* p) = 0;
		virtual void clear() = 0;

		// ACCESSOR
		inline size_t GetMemorySize() const
		{
			return this->m_MemorySize;
		}

		inline const void* GetMemoryAddress0() const
		{
			return this->m_pMemoryFirstAddress;
		}

		inline size_t GetUsedMemory() const
		{
			return this->m_MemoryUsed;
		}

		inline U64 GetAllocationCount() const
		{
			return this->m_MemoryAllocations;
		}

		inline R1_ALLOCATOR GetType() const
		{
			return this->m_Type;
		}
	};

}

#endif //__ALLOCATOR_H__
