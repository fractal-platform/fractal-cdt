#pragma once

/**
 * Define specific things for the fractal system
 */

extern "C" {
   void ftl_assert(unsigned int, const char*);
   void __cxa_pure_virtual() { ftl_assert(false, "pure virtual method called"); }
}
