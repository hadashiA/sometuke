#ifndef __kawaii__memory_pool__
#define __kawaii__memory_pool__

#include <memory>
#include <cstdlib>
#include <functional>

#ifndef MM_DEFAULT_EXPAND_SIZE
#define MM_DEFAULT_EXPAND_SIZE 10000
#endif

namespace kawaii {
using namespace std;
    
template <class T, std::size_t Size = MM_DEFAULT_EXPAND_SIZE>
class MemoryPool {
public:
    MemoryPool() : next(0) {
        ExpandFreeList(Size);
    }
        
    ~MemoryPool() {
        MemoryPool<T, Size>* nextPtr = next;
        for (nextPtr = next; nextPtr != 0; nextPtr = next) {
            next = next->next;
            std::free(nextPtr);
        }
    }
        
    void *Alloc(std::size_t) {
        if (!next) ExpandFreeList(Size);
        MemoryPool<T, Size>* head = next;
        next = head->next;
        return head;
    }
        
    void Free(void *doomed) {
        MemoryPool<T, Size>* head = static_cast<MemoryPool<T, Size>*>(doomed);
        head->next = next;
        next = head;
    }
        
private:
    void ExpandFreeList(int how_many) {
        std::size_t size =
            (sizeof(T) > sizeof(MemoryPool<T, Size>*)) ? sizeof(T) : sizeof(MemoryPool<T, Size>*);
            
        MemoryPool<T, Size>* runner = reinterpret_cast<MemoryPool<T, Size>*>(std::malloc(size));
        next = runner;
            
        for (int i = 0; i < how_many; ++i) {
            runner->next = reinterpret_cast<MemoryPool<T, Size>*>(std::malloc(size));
            runner = runner->next;
        }
        runner->next = 0;
    }
        
private:
    MemoryPool<T, Size>* next;
};
    
template <class T, std::size_t Size = MM_DEFAULT_EXPAND_SIZE>
class Poolable {
public:
    void *operator new(std::size_t size) {
        if (size == 0) {
            size = 1;
        }
        return POOL->Alloc(size);
    }

    void operator delete(void *doomed, std::size_t) {
        if (doomed == NULL) {
            return;
        }
        POOL->Free(doomed);
    }

    void *operator new[](std::size_t size) {
        if (size == 0) {
            size = 1;
        }
        return POOL->Alloc(size);
    }

    void operator delete[](void *doomed, std::size_t) {
        if (doomed == NULL) {
            return;
        }
        POOL->Free(doomed);
    }
        
    static void Init() {
        POOL.reset(new MemoryPool<T, Size>);
    }

private:
    static unique_ptr<MemoryPool<T, Size> > POOL;
};
    
template <class T, std::size_t Size = MM_DEFAULT_EXPAND_SIZE>
class PoolableFactory {
public:
    PoolableFactory() : pool_(new MemoryPool<T, Size>) {}
    ~PoolableFactory() {}
        
    T *New() {
        T *allocated = static_cast<T*>(pool_->Alloc(sizeof(T)));
        return static_cast<T*>(new(allocated) T());
    }
        
    void Delete(T *doomed) {
        doomed->~T();
        pool_->Free(doomed);
    }
        
private:
    unique_ptr<MemoryPool<T, Size> > pool_;
};
    
struct DataPool16 { char data[16]; };
struct DataPool32 { char data[32]; };
struct DataPool64 { char data[64]; };
struct DataPool128 { char data[128]; };
struct DataPool256 { char data[256]; };
    
class GeneralMemoryPool {
public:
    static unique_ptr<GeneralMemoryPool>& Shared() {
        static unique_ptr<GeneralMemoryPool> __shared(new GeneralMemoryPool);
        return __shared;
    }

    void *Alloc(std::size_t size) {
        void *allocated = NULL;
        if (size < 16) { allocated = mp16.Alloc(size); }
        else if (size < 32) { allocated = mp32.Alloc(size); }
        else if (size < 64) { allocated = mp64.Alloc(size); }
        else if (size < 128) { allocated = mp128.Alloc(size); }
        else if (size < 256) { allocated = mp256.Alloc(size); }
        return allocated;
    }
        
    void Free(void *doomed, std::size_t size) {
        if (size < 16) { mp16.Free(doomed); }
        else if (size < 32) { mp32.Free(doomed); }
        else if (size < 64) { mp64.Free(doomed); }
        else if (size < 128) { mp128.Free(doomed); }
        else if (size < 256) { mp256.Free(doomed); }
    }
        
private:
    GeneralMemoryPool() {}
        
    MemoryPool<DataPool16> mp16;
    MemoryPool<DataPool32> mp32;
    MemoryPool<DataPool64> mp64;
    MemoryPool<DataPool128> mp128;
    MemoryPool<DataPool256> mp256;
};

class GeneralPoolable {
public:
    void *operator new(std::size_t size) {
        if (size == 0) {
            size = 1;
        }
        return GeneralMemoryPool::Shared()->Alloc(size);
    }

    void operator delete(void *doomed, std::size_t size) {
        if (doomed == NULL) {
            return;
        }
        GeneralMemoryPool::Shared()->Free(doomed, size);
    }

    void *operator new[](std::size_t size) {
        if (size == 0) {
            size = 1;
        }
        return GeneralMemoryPool::Shared()->Alloc(size);
    }

    void operator delete[](void *doomed, std::size_t size) {
        if (doomed == NULL) {
            return;
        }
        GeneralMemoryPool::Shared()->Free(doomed, size);
    }
};

template <class T, std::size_t Size>
unique_ptr<MemoryPool<T, Size> > Poolable<T, Size>::POOL;
    
template<class T>
shared_ptr<T> New() {
    shared_ptr<T> ptr(new T);
    return ptr;
}

template<class T, class Arg1, class... Args>
shared_ptr<T> New(Arg1&& arg1, Args&& ... args) {
    shared_ptr<T> ptr(new T(std::forward<Arg1>(arg1), std::forward<Args>(args)...));
    return ptr;
}


}

#endif /* defined(__kawaii__memory_pool__) */
