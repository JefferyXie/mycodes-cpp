#pragma once

#include "../core/utility.h"
#include "for_each.h"
#include <type_traits>

void run_atomic_is_lock_free()
{
// TODO: linux likely requires __atomic_always_lock_free(sz, ...) @sz compile time number...
#ifdef __APPLE__
    // Rule: sizeof(M) must be power of 2, and not exceed 16; change the alignment with 'alignas(#)' to make the
    // structure lock free
    for_each_run<0, 32>([](auto sz) {
        std::cout << "sizeof(A)=" << sz << ", __atomic_always_lock_free=" << __atomic_always_lock_free(sz, 0)
                  << std::endl;
    });
#endif

    //
    // struct alignas(N) M { ... };
    // using ATOM = std::atomic<M>;
    //
    // 1) by default, i.e. without explicit 'alignas(N)', std::alignment_of<M>() == biggest alignment of individual
    // member; 2) sizeof(M) must be times of alignas(N) which is same as std::alignment_of<M>(); 3) if sizeof(M) <= 16,
    // the sizeof(ATOM) == the least exponential of 2 that is >= sizeof(M); and,
    //    std::alignment_of<ATOM>() == sizeof(ATOM);
    //    ATOM::is_always_lock_free == true.
    // 4) if sizeof(M) > 16, the sizeof(ATOM) == sizeof(M); and,
    //    std::alignment_of<ATOM>() == std::alignment_of<M>();
    //    ATOM::is_always_lock_free == false.
    //

    {
        struct M {
        };
        std::cout << "--- struct M { }; ---" << std::endl;
        static_assert(sizeof(M) == 1);
        static_assert(std::alignment_of<M>() == 1);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 1);
        static_assert(std::alignment_of<ATOM>() == 1);
        static_assert(ATOM::is_always_lock_free);
    }
    {
        struct M {
            uint8_t a;
        };
        std::cout << "--- struct M { uint8_t a; }; ---" << std::endl;
        static_assert(sizeof(M) == 1);
        static_assert(std::alignment_of<M>() == 1);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 1);
        static_assert(std::alignment_of<ATOM>() == 1);
        static_assert(ATOM::is_always_lock_free);
    }
    {
        struct M {
            double  a;
            uint8_t b;
        };
        std::cout << "--- struct M { double a; uint8_t b; }; ---" << std::endl;
        static_assert(sizeof(M) == 16);
        static_assert(std::alignment_of<M>() == 8);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 16);
        static_assert(std::alignment_of<ATOM>() == 16);

        // TODO: check why???
#ifdef __APPLE__
        static_assert(ATOM::is_always_lock_free);
#endif
    }
    {
        struct M {
            double  a;
            uint8_t b;
        } __attribute__((packed));
        std::cout << "--- struct M { double a; uint8_t b; }; ---" << std::endl;
        static_assert(sizeof(M) == 9);
        static_assert(std::alignment_of<M>() == 1);

        using ATOM = std::atomic<M>;

        // TODO: check why???
#ifdef __APPLE__
        static_assert(sizeof(ATOM) == 16);
        static_assert(std::alignment_of<ATOM>() == 16);
        static_assert(ATOM::is_always_lock_free);
#endif
    }

    {
        struct M {
            uint8_t a[3];
        };
        std::cout << "--- struct M { uint8_t a[3]; }; ---" << std::endl;
        static_assert(sizeof(M) == 3);
        static_assert(std::alignment_of<M>() == 1);

        using ATOM = std::atomic<M>;

        // TODO: check why???
#ifdef __APPLE__
        static_assert(sizeof(ATOM) == 4);
        static_assert(std::alignment_of<ATOM>() == 4);
        static_assert(ATOM::is_always_lock_free);
#endif
    }
    {
        struct alignas(2) M {
            uint8_t a[3];
        };
        std::cout << "--- struct alignas(2) M { uint8_t a[3]; }; ---" << std::endl;
        static_assert(sizeof(M) == 4);
        static_assert(std::alignment_of<M>() == 2);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 4);
        static_assert(std::alignment_of<ATOM>() == 4);
        static_assert(ATOM::is_always_lock_free);
    }
    {
        struct M {
            uint8_t a[5];
        };
        std::cout << "--- struct M { uint8_t a[5]; }; ---" << std::endl;
        static_assert(sizeof(M) == 5);
        static_assert(std::alignment_of<M>() == 1);

        using ATOM = std::atomic<M>;

        // TODO: check why???
#ifdef __APPLE__
        static_assert(sizeof(ATOM) == 8);
        static_assert(std::alignment_of<ATOM>() == 8);
        static_assert(ATOM::is_always_lock_free);
#endif
    }
    {
        struct alignas(2) M {
            uint8_t a[5];
        };
        std::cout << "--- struct alignas(2) M { uint8_t a[5]; }; ---" << std::endl;
        static_assert(sizeof(M) == 6);
        static_assert(std::alignment_of<M>() == 2);

        using ATOM = std::atomic<M>;

        // TODO: check why???
#ifdef __APPLE__
        static_assert(sizeof(ATOM) == 8);
        static_assert(std::alignment_of<ATOM>() == 8);
        static_assert(ATOM::is_always_lock_free);
#endif
    }
    {
        struct alignas(4) M {
            uint8_t a[5];
        };
        std::cout << "--- struct alignas(4) M { uint8_t a[5]; }; ---" << std::endl;
        static_assert(sizeof(M) == 8);
        static_assert(std::alignment_of<M>() == 4);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 8);
        static_assert(std::alignment_of<ATOM>() == 8);
        static_assert(ATOM::is_always_lock_free);
    }
    {
        struct M {
            uint8_t a[16];
        };
        std::cout << "--- struct alignas(4) M { uint8_t a[4]; }; ---" << std::endl;
        static_assert(sizeof(M) == 16);
        static_assert(std::alignment_of<M>() == 1);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 16);
        static_assert(std::alignment_of<ATOM>() == 16);

        // TODO: check why???
#ifdef __APPLE__
        static_assert(ATOM::is_always_lock_free);
#endif
    }
    {
        struct M {
            uint8_t a[17];
        };
        std::cout << "--- struct alignas(4) M { uint8_t a[4]; }; ---" << std::endl;
        static_assert(sizeof(M) == 17);
        static_assert(std::alignment_of<M>() == 1);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 17);
        // So since sizeof(ATOM) > 16, the std::alignment_of<ATOM>() == std::alignment_of<M>() and ATOM is not lock
        // free!
        static_assert(std::alignment_of<ATOM>() == 1);
        static_assert(ATOM::is_always_lock_free == false);
    }
    {
        struct M {
            uint32_t a;
            uint8_t  b;
        };
        std::cout << "--- struct M { uint32_t a; uint8_t b; }; ---" << std::endl;
        static_assert(sizeof(M) == 8);
        static_assert(std::alignment_of<M>() == 4);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 8);
        static_assert(std::alignment_of<ATOM>() == 8);
        static_assert(ATOM::is_always_lock_free);
    }
    {
        struct M {
            double a;
            char   b;
        };
        std::cout << "--- struct M { double a; uint32_t b; }; ---" << std::endl;
        static_assert(sizeof(M) == 16);
        static_assert(std::alignment_of<M>() == 8);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 16);
        static_assert(std::alignment_of<ATOM>() == 16);

        // TODO: check why???
#ifdef __APPLE__
        static_assert(ATOM::is_always_lock_free);
#endif
    }
    {
        struct alignas(32) M {
            uint32_t a;
        };
        std::cout << "--- struct alignas(32) M { uint32_t a; }; ---" << std::endl;
        static_assert(sizeof(M) == 32);
        static_assert(std::alignment_of<M>() == 32);

        using ATOM = std::atomic<M>;
        static_assert(sizeof(ATOM) == 32);
        static_assert(std::alignment_of<ATOM>() == 32);
        static_assert(ATOM::is_always_lock_free == false);
    }
}

