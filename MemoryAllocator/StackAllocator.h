#ifndef __STACK_ALLOCATOR_H__
#define __STACK_ALLOCATOR_H__

#include "Allocator.h"

namespace r1l00
{

	class StackAllocator : public Allocator
	{
	private:

		struct AllocMetaInfo
		{
			U8 adjustment;
		};

	public:

		StackAllocator(size_t memSize, const void* mem, R1_ALLOCATOR type);

		virtual ~StackAllocator();

		virtual void* allocate(size_t size, U8 alignment) override;
		virtual void* reallocate(size_t diff) override;
		virtual void free(void* p) override;
		virtual void clear() override;

	}; // class StackAllocator

}

#endif // __STACK_ALLOCATOR_H__
