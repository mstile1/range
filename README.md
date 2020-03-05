# range
simple constexpr c++ range class that emulates python range

https://docs.python.org/3/library/stdtypes.html#ranges

examples:
```
    // container iteration with index
    for ( auto const i : roam::range{ vec.size() } )
    {
        std::cout << i << ": " << vec[ i ] << std::endl;
    }
```
```
    // iterate n
    for ( [[maybe_unused]] auto const _ : roam::range{ 32 } )
    {
        do_smth();
    }
```
```
    // non-standard container?
    for ( auto const i : roam::range{ cls->get_count() } )
    {
        auto& obj = cls->get_object( i );
    }
```
```
    // iterate enums
    enum class my_enum{ one, two, three, max };
    for ( auto const i : roam::range{ my_enum::max } )
    {
        do_smth( enum_cast< my_enum >( i ) );
    }
```
```
    // sub range with type cast
    for ( auto const i : roam::range< int32_t >{ 5u, 10u } )
    {
        std::cout << i << std::endl;
    }
    // output: 5, 6, 7, 8, 9
```
```
    // step size - reversed
    auto const r = roam::range{ 0, 10, 2 };
    for ( auto it = r.rbegin(), it_end = r.rend(); it != it_end; ++it )
    {
        std::cout << *it << std::endl;
    }
    // output: 8, 6, 4, 2, 0
```
```
    // -ve step
    for ( auto const i = roam::range{ 8, -1, -2 } )
    {
        std::cout << i << std::endl;
    }
    // output: 8, 6, 4, 2, 0
```
