// example main.cpp

#include <iostream>

#include "../range.h"

void constexpr_unit_tests()
{
    {   // simple int range
        auto constexpr a = roam::range{ 0, 5, 3 };
        auto constexpr sz = a.size();
        static_assert( a[ 0 ] == 0 );
        static_assert( a[ -1 ] == 3 );
    }
    {   // simple int range 2
        auto constexpr a = roam::range{ 10 };
        auto constexpr sz = a.size();
        static_assert( a[ 0 ] == 0 );
        static_assert( a[ -1 ] == 9 );
    }
    {   // int range across 0 barrier
        auto constexpr a = roam::range{ -5, 5, 3 };
        auto constexpr sz = a.size();
        static_assert( a[ 1 ] == -2 );
        static_assert( a[ -2 ] == 1 );
    }
    {   // oversized step
        auto constexpr a = roam::range{ 1, 2, 10 };
        auto constexpr sz = a.size();
        static_assert( a[ 0 ] == 1 );
        static_assert( a[ -1 ] == 1 );
    }
    {   // long long with negative step
        auto constexpr a = roam::range< int64_t >{ 9, -6, -3 };
        auto constexpr sz = a.size();
        static_assert( a[ 1 ] == 6 );
        static_assert( a[ -2 ] == 0 );
    }
    {   // double range
        auto constexpr a = roam::range{ -3.2, 8.0, 0.8 };
        auto constexpr sz = a.size();
        static_assert( a[ 2 ] == -1.6 );
        auto constexpr v = a[ -2 ];
        // float precision makes this tricky, e.g. 6.4
        static_assert( a[ -2 ] == -3.2 + 0.8 * ( sz - 2 ) );
    }
    {   // enum: implicit init to underlying type
        enum class test_enums { zero, one, two, three, four };
        auto constexpr a = roam::range{ test_enums::four };
        static_assert( a[ 1 ] == 1 );
        static_assert( a[ -2 ] == 2 );
    }
}

int main()
{
    auto a = roam::range< int32_t >{ 5u, 10u };
    auto b = roam::range< int8_t >{ 10u };

    for ( auto const i : roam::range{ 3 } )
    {
        std::cout << "range test: " << i << std::endl;
    }
    std::cout << std::endl;

    auto const r = roam::range{ 0, 10, 2 };
    for ( auto it = r.rbegin(), it_end = r.rend(); it != it_end; ++it )
    {
        std::cout << "range test: " << *it << std::endl;
    }
}
