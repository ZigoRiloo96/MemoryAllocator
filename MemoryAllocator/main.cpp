#include "MemoryManager.h"

#define g_MemoryManager r1l00::MemoryManager::staticInstance()

int main()
{

	char* c = (char*)g_MemoryManager->Allocate(1000000000 * sizeof(char));

	g_MemoryManager->Free(c);

	return 0;
}