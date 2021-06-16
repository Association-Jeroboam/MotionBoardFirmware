#pragma once
#include "ch.hpp"

void* operator new( size_t size ) { return chHeapAlloc( 0x0, size ); }
void* operator new[]( size_t size ) { return chHeapAlloc( 0x0, size ); }
void operator delete( void *p ) { chHeapFree( p ); }
void operator delete[]( void *p ) { chHeapFree( p ); }
