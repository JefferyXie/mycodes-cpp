#include <gtest/gtest.h>
#include "../header.h"
#include "../ThreadOrder/usemutex.c"
#include "../ThreadOrder/usesemaphore.c"

TEST(multithreading, UseMutex) {
    UseMutex();
}

TEST(multithreading, UseSemaphore) {
    UseSemaphore();
}


