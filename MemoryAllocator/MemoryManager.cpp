#include "Allocators.h"
#include "MemoryManager.h"

namespace r1l00
{
	MemoryManager& MemoryManager::staticInstance()
	{
		static MemoryManager g_mm;
		return g_mm;
	}

	MemoryManager::MemoryManager()
	{
		// allocate global memory
		this->m_GlobalMemory = malloc(MemoryManager::MEMORY_CAPACITY);
		if (this->m_GlobalMemory != nullptr)
		{
			printf("%d bytes of memory allocated.\n", (int)MemoryManager::MEMORY_CAPACITY);
		}
		else
		{
			printf("Failed to allocate %d bytes of memory!", (int)MemoryManager::MEMORY_CAPACITY);
			assert(this->m_GlobalMemory != nullptr && "Failed to allocate global memory.");
		}

		// create allocator
		this->m_MemoryAllocator = GetAllocator(R1_ALLOCATOR::STACK, MemoryManager::MEMORY_CAPACITY, this->m_GlobalMemory);
		assert(this->m_MemoryAllocator != nullptr && "Failed to create memory allocator!");

		this->m_PendingMemory.clear();
		this->m_FreedMemory.clear();
	}

	MemoryManager::~MemoryManager()
	{
		this->m_MemoryAllocator->clear();

		delete this->m_MemoryAllocator;
		this->m_MemoryAllocator = nullptr;

		free(this->m_GlobalMemory);
		this->m_GlobalMemory = nullptr;
	}

	void MemoryManager::ReAllocate(size_t timesCapacity)
	{
		void* oldMemoryPointer = this->m_GlobalMemory;

		this->m_GlobalMemory = malloc(MemoryManager::MEMORY_CAPACITY * timesCapacity);

		if (this->m_GlobalMemory != nullptr)
		{
			printf("%d bytes of memory reallocated.\n", (int)MemoryManager::MEMORY_CAPACITY * timesCapacity);
		}
		else
		{
			printf("Failed to reallocate %d bytes of memory!", (int)MemoryManager::MEMORY_CAPACITY * timesCapacity);
			assert(this->m_GlobalMemory != nullptr && "Failed to reallocate global memory.");
		}

		memcpy(this->m_GlobalMemory, oldMemoryPointer, this->m_MemoryAllocator->GetMemorySize());

		// create allocator
		Allocator* newAllocator = GetAllocator(this->m_MemoryAllocator);

		delete(this->m_MemoryAllocator);

		this->m_MemoryAllocator = newAllocator;

		assert(this->m_MemoryAllocator != nullptr && "Failed to memory reallocate!");

		//this->m_PendingMemory.clear();
		//this->m_FreedMemory.clear();
	}

	void MemoryManager::CheckMemoryLeaks()
	{
		assert(!(this->m_FreedMemory.size() > 0 && this->m_PendingMemory.size() == 0) && "Implementation failure!");

		if (this->m_PendingMemory.size() > 0)
		{
			printf("!MEMORY_LEAK!\n");

			for (auto i : this->m_PendingMemory)
			{
				bool isFreed = false;

				for (auto j : this->m_FreedMemory)
				{
					if (i.second == j)
					{
						isFreed = true;
						break;
					}
				}

				if (isFreed == false)
				{
					printf("\'%s\' memory user didn't release allocated memory %p!", i.first, i.second);
				}
			}
		}
		else
		{
			printf("No memory leaks detected.\n");
		}
	}

}
