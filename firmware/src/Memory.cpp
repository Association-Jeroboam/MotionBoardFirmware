#include <ch.hpp>
#include "Memory.hpp"

static bool mallocAuthorised = true;

void deactivateMemoryAllocation() {
    mallocAuthorised = false;
}

void free(void* ptr) {
    chHeapFree(ptr);
}

void operator delete(void* ptr) noexcept {
    chHeapFree(ptr);
}

void operator delete[](void* ptr) noexcept {
    chHeapFree(ptr);
}

void operator delete(void* ptr, size_t sz) {
    (void)sz;
    chHeapFree(ptr);
}

void operator delete[](void* ptr, size_t sz) {
    (void)sz;
    chHeapFree(ptr);
}

void* operator new(size_t size) {
    chDbgAssert(mallocAuthorised, "unauthorised malloc");
    return chHeapAlloc(nullptr, size);
}

void* operator new[](size_t size) {
    chDbgAssert(mallocAuthorised, "unauthorised malloc");
    return chHeapAlloc(nullptr, size);
}

void* operator new(size_t size, stkalign_t alignment) {
    chDbgAssert(mallocAuthorised, "unauthorised malloc");
    return chHeapAllocAligned(nullptr, size, alignment);
}

void* operator new[](size_t size, stkalign_t alignment) {
    chDbgAssert(mallocAuthorised, "unauthorised malloc");
    return chHeapAllocAligned(nullptr, size, alignment);
}
