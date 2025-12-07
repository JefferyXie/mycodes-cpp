#include <gtest/gtest.h>

#include "../core/util_math.h"

#include "../data_structure/my_hash_map.h"
#include "../data_structure/my_heap.h"
#include "../data_structure/my_list.h"
#include "../data_structure/my_queue.h"
#include "../data_structure/my_sort.h"
#include "../data_structure/my_smartpointer.h"
#include "../data_structure/my_tuple.h"

#include "../language/classtest.h"

#include "../thread/thread_order.h"

namespace {

struct MyMember {
    static int  s_count;
    int         value;
    std::string name;
    // list<int> ls;
    MyMember()
    {
        value = s_count++;
        name  = "name[" + std::to_string(value) + "]";
        std::cout << "MyMember() " << value << std::endl;
    }
    ~MyMember() { std::cout << "~MyMember() " << value << std::endl; }
    std::string ToString() { return "value:" + std::to_string(value) + ",name:" + name; }
    MyMember&   operator=(const MyMember&) = delete;
};
int MyMember::s_count = 0;

}    // namespace

TEST(utility, math_prime)
{
    static_assert(!util_math::compile_time_is_prime(0));
    static_assert(!util_math::compile_time_is_prime(1));
    static_assert(!util_math::compile_time_is_prime(2));
    static_assert(util_math::compile_time_is_prime(3));
    static_assert(!util_math::compile_time_is_prime(4));
    static_assert(util_math::compile_time_is_prime(5));
    static_assert(!util_math::compile_time_is_prime(6));

    static_assert(util_math::compile_time_count_primes(0) == 0);
    static_assert(util_math::compile_time_count_primes(5) == 1);
    static_assert(util_math::compile_time_count_primes(10) == 3);
    static_assert(util_math::compile_time_count_primes(20) == 7);
    static_assert(util_math::compile_time_count_primes(30) == 9);
}

TEST(utility, my_hash_map)
{
    run_my_hash_map();
}

TEST(utility, my_heap)
{
    run_min_heap_to_max_heap();
}

TEST(utility, my_list)
{
    my_list<int> olist;
    int          i_2  = 2;
    int          i_3  = 3;
    int          i_5  = 5;
    int          i_8  = 8;
    int          i_7  = 7;
    int          i_0  = 0;
    int          i_11 = 11;
    int          i_10 = 10;
    olist.Add(i_3);
    olist.Add(i_5);
    //    olist.Add(i_2);
    auto p = olist.Find(i_3);
    p      = olist.Insert(p, i_8);
    olist.Insert(p, i_7);
    olist.Insert2Head(i_0);
    olist.Insert(p, i_11);
    olist.Insert2End(i_5);
    olist.Insert2End(i_3);
    olist.Remove(i_8);
    p = olist.Insert2Head(i_10);
    olist.Remove(i_10);

    //    olist.Display();

    olist.Reverse();
    olist.Display();
}

TEST(DISABLED_utility, my_queue_int)
{
    my_queue<int> q1, q2;
    for (int i = 0; i < 100; ++i) {
        q1.Push(i);
        std::cout << q1.ToString() << std::endl;
    }
    while (q1.Length() > 0) {
        std::cout << q1.Pop() << ", ";
        std::cout << q1.ToString() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "--------------------------------------------------" << std::endl;

    auto writer = [&q2] {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        for (int i = 0; i < 100; ++i) {
            q2.Push(i);
            std::cout << i << " pushed * " << q2.ToString() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };
    auto reader = [&q2] {
        while (1) {
            int i = q2.Pop();
            std::cout << i << " poped * " << q2.ToString() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    };

    std::thread th_writer(writer);
    std::thread th_reader(reader);

    th_writer.join();
    th_reader.join();
}

TEST(DISABLED_utility, my_queue_class)
{
    {
        my_queue<MyMember> q1;
        for (int i = 0; i < 100; ++i) {
            q1.Push(MyMember());
            std::cout << q1.ToString() << std::endl;
        }
        while (q1.Length() > 0) {
            std::cout << q1.Pop().ToString() << ", ";
            std::cout << q1.ToString() << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "--------------------------------------------------" << std::endl;

    {
        bool               writter_finished = false;
        my_queue<MyMember> q2;
        auto               writer = [&q2, &writter_finished] {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            for (int i = 0; i < 100; ++i) {
                q2.Push(MyMember());
                std::cout << i << " pushed * " << q2.ToString() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            writter_finished = true;
        };
        auto reader = [&q2, &writter_finished] {
            while (!writter_finished || q2.Length() > 0) {
                auto i = q2.Pop().ToString();
                std::cout << i << " poped * " << q2.ToString() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        };

        std::thread th_writer(writer);
        std::thread th_reader(reader);

        th_writer.join();
        th_reader.join();
    }
    std::cout << "test end!" << std::endl;
}

TEST(utility, my_smartpointer)
{
    my_smartpointer<int> s5;
    EXPECT_EQ(s5.use_count(), 0);
    EXPECT_EQ(s5.get(), nullptr);

    s5 = my_smartpointer<int>(new int(9));
    EXPECT_EQ(s5.use_count(), 1);
    EXPECT_EQ(*s5, 9);

    my_smartpointer<int> s1(new int(2));
    EXPECT_EQ(s1.use_count(), 1);

    my_smartpointer<int> s2(s1);
    my_smartpointer<int> s3 = s1;
    EXPECT_EQ(s1.use_count(), 3);
    EXPECT_EQ(s1.get(), s3.get());

    my_smartpointer<int> s4(new int(5));
    EXPECT_EQ(*s4, 5);

    s4 = s1;
    EXPECT_EQ(s1.use_count(), 4);
    EXPECT_EQ(s4.use_count(), 4);
    EXPECT_EQ(s4.get(), s1.get());
    EXPECT_EQ(*s4, 2);

    my_smartpointer<Plain> s11(new Plain());
    my_smartpointer<Plain> s22(s11);
    my_smartpointer<Plain> s33 = s11;
    auto                   p   = s11.get();

    my_smartpointer<Plain> s44(new Plain());
    s44 = s11;
    EXPECT_EQ(s44.use_count(), 4);
    EXPECT_EQ(s44.get(), p);

    my_smartpointer<Plain> s55;
    s55 = my_smartpointer<Plain>(new Plain());

    s44.reset(new Plain());
    EXPECT_EQ(s44.use_count(), 1);
    EXPECT_EQ(s11.use_count(), 3);
    EXPECT_EQ(s11.get(), p);

    auto p_44 = s44.get();

    s44.reset(new Plain());
    EXPECT_EQ(s44.use_count(), 1);
    EXPECT_NE(s44.get(), p_44);

    struct PPlain : public Plain {
        //        PPlain() : Plain{} {}
        //        ~PPlain() {}
    };

    static_assert(std::is_convertible_v<int, long>);
    static_assert(std::is_convertible_v<int, double>);
    static_assert(std::is_convertible_v<double, int>);
    static_assert(std::is_convertible_v<PPlain, Plain>);

    static_assert(!std::is_convertible_v<int, Plain>);
    static_assert(!std::is_convertible_v<Plain, int>);
    static_assert(!std::is_convertible_v<Plain, PPlain>);

    my_smartpointer<Plain> sp1(new PPlain{});

    sp1 = my_smartpointer<PPlain>(new PPlain{});

    sp1 = my_smartpointer<Plain>(new Plain());

    sp1.reset(new PPlain{});

    auto p1 = my_make_shared<Plain>();
    p1      = sp1;

    auto p2 = my_make_shared<PPlain>();
    p1      = p2;
}

TEST(DISABLED_utility, my_tuple)
{
    run_my_tuple();
}

TEST(DISABLED_utility, UseMutex)
{
    UseMutex();
}

TEST(DISABLED_utility, UseSemaphore)
{
    UseSemaphore();
}

