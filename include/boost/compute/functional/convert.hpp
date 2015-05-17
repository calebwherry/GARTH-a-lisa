//---------------------------------------------------------------------------//
// Copyright (c) 2013 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://kylelutz.github.com/compute for more information.
//---------------------------------------------------------------------------//

#ifndef BOOST_COMPUTE_FUNCTIONAL_CONVERT_HPP
#define BOOST_COMPUTE_FUNCTIONAL_CONVERT_HPP

namespace boost {
namespace compute {
namespace detail {

template<class T, class Arg>
struct invoked_convert
{
    invoked_convert(const Arg &arg)
        : m_arg(arg)
    {
    }

    Arg m_arg;
};

} // end detail namespace

template<class T>
struct convert
{
    typedef T result_type;

    template<class Arg>
    detail::invoked_convert<T, Arg> operator()(const Arg &arg) const
    {
        return detail::invoked_convert<T, Arg>(arg);
    }
};

} // end compute namespace
} // end boost namespace

#endif // BOOST_COMPUTE_FUNCTIONAL_CONVERT_HPP
