#ifndef BATMAN_POWER_UNIT_H
#define BATMAN_POWER_UNIT_H

#include <limits>
#include <ratio>

namespace batman {

template<class Rep, class Period>
class power_unit;

template<class ToUnit, class Rep, class Period>
ToUnit power_unit_cast(const power_unit<Rep, Period>& from);

template<class Rep, class Period = std::ratio<1>>
class power_unit
{
public:
    using rep = Rep;
    using period = Period;

    constexpr power_unit() = default;

    constexpr power_unit(const power_unit&) = default;

    template<class Rep2>
    constexpr explicit power_unit(const Rep2& r) :
            count_{r}
    {}

    template<class Rep2, class Period2>
    constexpr explicit power_unit(const power_unit<Rep2, Period2>& other) :
            count_{power_unit_cast<power_unit>(other).count()}
    {};

    power_unit& operator=(const power_unit&) = default;

    constexpr Rep count() const
    {
        return count_;
    }

    static constexpr power_unit zero()
    {
        return {0};
    }

    static constexpr power_unit min()
    {
        return {std::numeric_limits<Rep>::min()};
    }

    static constexpr power_unit max()
    {
        return {std::numeric_limits<Rep>::max()};
    }

private:
    Rep count_ = {};
};

template<class Rep, class Period, class Rep2, class Period2>
constexpr auto operator+(
        const power_unit<Rep, Period>& lhs, const power_unit<Rep2, Period2>& rhs
)
{
    using common_type = std::common_type_t<
            power_unit<Rep, Period>, power_unit<Rep2, Period2>
    >;
    return common_type(common_type(lhs).count() + common_type(rhs).count());
}

template<class Rep, class Period, class Rep2, class Period2>
constexpr auto operator-(
        const power_unit<Rep, Period>& lhs, const power_unit<Rep2, Period2>& rhs
)
{
    using common_type = std::common_type_t<
            power_unit<Rep, Period>, power_unit<Rep2, Period2>
    >;
    return common_type(common_type(lhs).count() - common_type(rhs).count());
}

namespace detail {

template<
        class FromUnit, class ToUnit, class Period = std::ratio_divide<
                typename FromUnit::period, typename ToUnit::period
        >
>
struct __power_unit_cast
{
    constexpr ToUnit operator()(const FromUnit& from) const
    {
        using common_type = std::common_type_t<
                typename FromUnit::rep, typename ToUnit::rep, std::intmax_t
        >;
        return ToUnit{
                static_cast<typename ToUnit::rep>(
                        static_cast<common_type>(from.count()) *
                        static_cast<common_type>(Period::num) /
                        static_cast<common_type>(Period::den)
                )
        };
    }
};

} /* namespace detail */

template<class ToUnit, class Rep, class Period>
ToUnit power_unit_cast(const power_unit<Rep, Period>& from)
{
    return detail::__power_unit_cast<power_unit<Rep, Period>, ToUnit>{}(from);
};

using milliwatts = power_unit<std::int64_t, std::milli>;
using watts = power_unit<std::int64_t>;
using kilowatts = power_unit<std::int64_t, std::ratio<1000>>;

constexpr watts operator ""_w(unsigned long long value)
{
    return watts{static_cast<watts::rep>(value)};
}

} /* namespace batman */

namespace std {

template<class Rep, class Period, class Rep2, class Period2>
struct common_type<
        batman::power_unit<Rep, Period>, batman::power_unit<Rep2, Period2>
>
{
public:
    using type = batman::power_unit<
            typename std::common_type_t<Rep, Rep2>,
            typename std::ratio_divide<Period, Period2>
    >;
};

} /* namespace std */

#endif //BATMAN_POWER_UNIT_H
