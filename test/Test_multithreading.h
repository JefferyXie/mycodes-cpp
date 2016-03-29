#include <gtest/gtest.h>
#include "../header.h"
#include "../ThreadOrder/usemutex.c"
#include "../ThreadOrder/usesemaphore.c"

TEST(DISABLED_multithreading, UseMutex) {
    UseMutex();
}

TEST(DISABLED_multithreading, UseSemaphore) {
    UseSemaphore();
}


