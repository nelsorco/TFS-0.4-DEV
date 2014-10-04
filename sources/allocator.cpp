/////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game  //
/////////////////////////////////////////////////

#ifdef __OTSERV_ALLOCATOR__
#include "otpch.h"
#include "allocator.h"

//normal new/delete
void* operator new(size_t bytes)
{
	return PoolManager::getInstance()->allocate(bytes);
}

void* operator new[](size_t bytes)
{
	return PoolManager::getInstance()->allocate(bytes);
}

void operator delete(void* p)
{
	PoolManager::getInstance()->deallocate(p);
}

void operator delete[](void* p)
{
	PoolManager::getInstance()->deallocate(p);
}

//dummy new/delete operators
void* operator new(size_t bytes, int32_t dummy)
{
	return malloc(bytes);
}

#ifdef _MSC_VER
void* operator new[](size_t bytes, int32_t dummy)
{
	return malloc(bytes);
}

void operator delete(void* p, int32_t dummy)
{
	std::free(p);
}

void operator delete[](void* p, int32_t dummy)
{
	std::free(p);
}
#endif

#ifdef __OTSERV_ALLOCATOR_STATS__
void allocatorStatsThread(void* a)
{
	while(true)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(30000));
		PoolManager::getInstance()->dumpStats();
	}
}
#endif

#endif
