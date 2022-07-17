/*******************************************************************************

   Placing XAD math functions into the std namespace for std::log type
expressions to work, as well as specialising numeric_limits.

   This file is part of XAD, a fast and comprehensive C++ library for
   automatic differentiation.

   Copyright (C) 2010-2022 Xcelerit Computing Ltd.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#pragma once

#include <XAD/Literals.hpp>
#include <XAD/MathFunctions.hpp>
#include <XAD/UnaryOperators.hpp>
#include <XAD/BinaryOperators.hpp>
#include <cmath>
#include <limits>
#include <string>
#include <functional>
#include <type_traits>

namespace std
{
using xad::abs;
using xad::acos;
using xad::acosh;
using xad::asin;
using xad::asinh;
using xad::atan;
using xad::atan2;
using xad::atanh;
using xad::cbrt;
using xad::ceil;
using xad::cos;
using xad::cosh;
using xad::erf;
using xad::erfc;
using xad::exp;
using xad::exp2;
using xad::expm1;
using xad::fabs;
using xad::floor;
using xad::fmax;
using xad::fmin;
using xad::fmod;
using xad::fpclassify;
using xad::frexp;
using xad::isfinite;
using xad::isinf;
using xad::isnan;
using xad::isnormal;
using xad::ldexp;
using xad::llround;
using xad::log;
using xad::log10;
using xad::log1p;
using xad::log2;
using xad::lround;
using xad::max;
using xad::min;
using xad::modf;
using xad::pow;
using xad::remainder;
using xad::remquo;
using xad::round;
using xad::signbit;
using xad::sin;
using xad::sinh;
using xad::sqrt;
using xad::tan;
using xad::tanh;
using xad::trunc;
using xad::nextafter;
using xad::ilogb;
using xad::scalbn;

template <class Scalar, class Derived>
inline std::string to_string(const xad::Expression<Scalar, Derived>& _Val)
{
    return to_string(value(_Val));
}

}  // namespace std



namespace std
{

// note that these return the underlying doubles, not the active type,
// but since they are constant and convertible, it's the right behaviour
// for the majority of cases

template <class T>
struct numeric_limits<xad::AReal<T>> : std::numeric_limits<T> 
{ 
};

template <class T>
struct numeric_limits<xad::FReal<T>> : std::numeric_limits<T>
{
};

}  // namespace std


// hashing for active types
namespace std
{

template <class T>
struct hash<xad::AReal<T>>
{
    std::size_t operator()(xad::AReal<T> const& s) const noexcept
    {
        return std::hash<T>{}(xad::value(s));
    }
};

template <class T>
struct hash<xad::FReal<T>>
{
    std::size_t operator()(xad::FReal<T> const& s) const noexcept
    {
        return std::hash<T>{}(xad::value(s));
    }
};

// type traits
template <class T>
struct is_floating_point<xad::AReal<T>> : std::is_floating_point<T>
{
};
template <class T>
struct is_floating_point<xad::FReal<T>> : std::is_floating_point<T>
{
};
template <class T>
struct is_arithmetic<xad::AReal<T>> : std::is_arithmetic<T>
{
};
template <class T>
struct is_arithmetic<xad::FReal<T>> : std::is_arithmetic<T>
{
};
template <class T>
struct is_pod<xad::AReal<T>> : std::false_type
{
};
template <class T>
struct is_pod<xad::FReal<T>> : std::false_type
{
};
template <class T>
struct is_fundamental<xad::AReal<T>> : std::false_type
{
};
template <class T>
struct is_fundamental<xad::FReal<T>> : std::false_type
{
};
#if !(defined(__GNUC__) && __GNUC__ < 5) || defined(__clang__)
template <class T>
struct is_trivially_copyable<xad::FReal<T>> : std::is_trivially_copyable<T>
{
};
#endif
template <class T>
struct is_scalar<xad::AReal<T>> : std::false_type
{
};
template <class T>
struct is_scalar<xad::FReal<T>> : std::false_type
{
};
template <class T>
struct is_compound<xad::AReal<T>> : std::true_type
{
};
template <class T>
struct is_compound<xad::FReal<T>> : std::true_type
{
};

#if defined(_MSC_VER)

// for MSVC, we need this workaround so that the safety checks in their STL
// for floating point types are also passing for the XAD types
#if (_MSC_VER > 1900)
// VS 2017+, when the STL checks if a type is in the list of built-in floating point types,
// this should forward the check to the wrapped type by AReal or FReal.
//
// (In GCC, std::is_floating_point is used instead, where traits above work)

template <class T>
constexpr bool _Is_any_of_v<xad::AReal<T>, float, double, long double> =
    _Is_any_of_v<T, float, double, long double>;

template <class T>
constexpr bool _Is_any_of_v<xad::FReal<T>, float, double, long double> =
    _Is_any_of_v<T, float, double, long double>;

#else
}
#include <random>
namespace std {


// prior versions of MSVC (2015) use a different check
template <class T>
struct _Is_RealType<xad::AReal<T>> : public _Is_RealType<T>
{
};

template <class T>
struct _Is_RealType<xad::FReal<T>> : public _Is_RealType<T>
{
};

#endif

#endif

}  // namespace std

