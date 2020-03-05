// range.h
//
// utility range class, similar implementation to python 3.0 range()
// range( start = 0, stop, step = 1 )
// e.g.
//     for ( auto const i : roam::range{ 5 } ) {}
//     for ( auto const i : roam::range{ 10, 20, 2 } ) {}
//=============================================================================

#ifndef _INC_ROAM_RANGE_H_
#define _INC_ROAM_RANGE_H_

#include "platform/platformAssert.h"
#include "roam/type_traits.h"

#include <algorithm>
#include <gsl/gsl>
#include <iterator>

//-----------------------------------------------------------------------------

namespace roam
{

template < typename ty_t >
class range
{
public:
    using value_type = ty_t;

    constexpr explicit range( ty_t const& start, ty_t const& stop, ty_t const& step = ty_t{ 1 } ) :
        start_{ start },
        stop_{ stop },
        step_{ step }
    {   // @example: range{ 0, 5[, 1] }
        AssertRelease( step_ != ty_t{ 0 }, "range -- cannot have step size of 0" );
        AssertRelease( ( start_ <= stop_ && step_ > ty_t{ 0 } ) ||
                       ( start_ >= stop_ && step_ < ty_t{ 0 } ), "invalid range and step" );
    }
    constexpr explicit range( ty_t const& stop ) : range{ ty_t{ 0 }, stop }
    {   // @example: range{ 5 }
    }
    template < typename ty2_t >
    constexpr explicit range( ty2_t const& stop ) : range{ static_cast< ty_t >( stop ) }
    {   // enable implicit conversion of input type to desired
        // @example: range< S64 >{ 10u }
        AssertRelease( !roam::is_narrow( stop, stop_ ), "range -- narrowing error" );
    }

    [[nodiscard]] constexpr auto size() const -> std::size_t
    {   // @return number of steps in range
        // @example1: range{ 5 }.size() == 5
        // @example2: range{ 2, 5, 3 }.size() == 1
        // @example3: range{ 1, 2, 10 }.size() == 1
        auto const sz = static_cast< std::ptrdiff_t >( ( stop_ - start_ ) / step_ );
        auto const v = start_ + static_cast< ty_t >( sz * step_ );
        return static_cast< std::size_t >( v == stop_ ? sz : sz + 1 );
    }
    [[nodiscard]] constexpr auto empty() const -> bool
    {
        return 0 == size();
    }
    [[nodiscard]] constexpr auto operator[]( std::ptrdiff_t const idx_in ) const -> ty_t
    {   // @return index of range, range[0] is always start
        // @note: range[-1] is last possible step < end (e.g. range{ 0, 5, 2 }[-1] == 4 )
        auto const idx = idx_in >= 0 ? idx_in : static_cast< std::ptrdiff_t >( size() ) + idx_in;
        auto const ret = start_ + static_cast< ty_t >( step_ * idx );
        AssertRelease( ( ret >= start_ && ret < stop_ && step_ > ty_t{ 0 } ) ||
                       ( ret <= start_ && ret > stop_ && step_ < ty_t{ 0 } ),
                       avar( "range -- invalid index: %lld", idx_in ) );
        return ret;
    }

    // iteration
    // @note: iteration is NOT constexpr as you can't loop at compile time
    class iterator
    {   // iterator holds reference to range and is invalidated if range destroyed
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = ty_t;
        using reference = ty_t;
        using pointer = ty_t*;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit iterator( range< ty_t > const& range, std::ptrdiff_t const& idx ) :
            range_{ range },
            idx_{ idx }
        {
        }

        [[nodiscard]] auto operator==( iterator const& rhs ) const -> bool {
            return &range_ == &rhs.range_ && idx_ == rhs.idx_;
        }
        [[nodiscard]] auto operator!=( iterator const& rhs ) const -> bool {
            return !( *this == rhs );
        }

        auto operator++() -> iterator& {
            ++idx_;
            return *this;
        }
        auto operator++( int ) -> iterator {
            auto const ret = *this;
            ++idx_;
            return ret;
        }
        auto operator--() -> iterator& {
            --idx_;
            return *this;
        }
        auto operator--( int ) -> iterator {
            auto const ret = *this;
            --idx_;
            return ret;
        }

        [[nodiscard]] auto operator*() -> reference {
            return range_[ idx_ ];
        }
        [[nodiscard]] auto operator->() -> pointer {
            return &range_[ idx_ ];
        }

    private:
        range< ty_t > const& range_{};
        std::ptrdiff_t idx_{};
    };
    using reverse_iterator = std::reverse_iterator< iterator >;

    [[nodiscard]] auto begin() const -> iterator {
        return iterator{ *this, 0 };
    }
    [[nodiscard]] auto end() const -> iterator {
        return iterator{ *this, gsl::narrow< std::ptrdiff_t >( size() ) };
    }
    [[nodiscard]] auto rbegin() const -> reverse_iterator {
        return reverse_iterator{ end() };
    }
    [[nodiscard]] auto rend() const -> reverse_iterator {
        return reverse_iterator{ begin() };
    }

private:
    ty_t start_{};
    ty_t stop_{};
    ty_t step_{};
};

// construct an integral range from an enum type (using enum's underlying type)
template < typename ty_t, typename = roam::enable_if_enum_t< ty_t > >
range( ty_t const& ) -> range< std::underlying_type_t< ty_t > >;

// min range of container.size() or count
template < typename ty_t, typename con_t >
inline auto min_range( con_t const& c, ty_t const& count )
{
    auto const sz = gsl::narrow< ty_t >( c.size() );
    return range{ std::min( sz, count ) };
}

} // roam

//-----------------------------------------------------------------------------

#endif // _INC_ROAM_RANGE_H_
