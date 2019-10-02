#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

//#include "Platform.h"
#include "Allocator.h"
#include <malloc.h>

namespace r1l00
{

	class MemoryManager
	{
	public:

		static MemoryManager& staticInstance();

		static constexpr size_t MEMORY_CAPACITY = GLOBAL_MEMORY_CAPACITY;

	private:

		// Pointer to global allocated memory
		void* m_GlobalMemory;

		// Allocator used to manager memory allocation from global memory
		Allocator* m_MemoryAllocator;

		std::vector<std::pair<const char*, void*>>	m_PendingMemory;

		std::list<void*>							m_FreedMemory;

		MemoryManager(const MemoryManager&) = delete;
		MemoryManager& operator=(MemoryManager&) = delete;

	public:

		MemoryManager();
		~MemoryManager();

		void ReAllocate(size_t newMem);

		size_t GetSize()
		{
#if (defined(_WIN32) || defined(_WIN64))
			return _msize(m_GlobalMemory);
#else
			return malloc_usable_size(m_GlobalMemory);
#endif
		}

		inline void* Allocate(size_t memSize, const char* user = nullptr)
		{
			void* pMemory = m_MemoryAllocator->allocate(memSize, alignof(U8));

			this->m_PendingMemory.push_back(std::pair<const char*, void*>(user, pMemory));

			return pMemory;
		}

		inline void Free(void* pMem)
		{
			if (pMem == this->m_PendingMemory.back().second)
			{
				this->m_MemoryAllocator->free(pMem);
				this->m_PendingMemory.pop_back();

				bool bCheck = true;
				while (bCheck == true)
				{
					bCheck = false;

					// do not report already freed memory blocks.
					for (auto it : this->m_FreedMemory)
					{
						if (it == this->m_PendingMemory.back().second)
						{
							this->m_MemoryAllocator->free(pMem);
							this->m_PendingMemory.pop_back();
							this->m_FreedMemory.remove(it);

							bCheck = true;
							break;
						}
					}
				};

			}
			else
			{
				this->m_FreedMemory.push_back(pMem);
			}
		}

		void CheckMemoryLeaks();

	}; // class MemoryManager

}

#endif //__MEMORY_MANAGER__
