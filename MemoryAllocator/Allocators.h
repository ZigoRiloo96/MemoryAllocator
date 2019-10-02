#ifndef __ALLOCATORS_H__
#define __ALLOCATORS_H__

#include "Allocator.h"
#include "StackAllocator.h"

namespace r1l00
{

	enum R1_ALLOCATOR
	{
		NONE,
		STACK = BIT(0)
	};

    static Allocator* GetAllocator(R1_ALLOCATOR allocator, size_t memSize, const void* mem)
	{
		switch (allocator)
		{
		case r1l00::STACK:
			return new StackAllocator(memSize, mem, allocator);
		case r1l00::NONE:
		default:
			return 0;
		}
	}

	static Allocator* GetAllocator(const Allocator* allocator)
	{
		switch (allocator->GetType())
		{
		case r1l00::STACK:
			return new StackAllocator(*(StackAllocator*)allocator);
		case r1l00::NONE:
		default:
			return 0;
		}
	}
}

#endif // !__ALLOCATORS_H__
