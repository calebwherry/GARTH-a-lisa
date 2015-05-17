//---------------------------------------------------------------------------//
// Copyright (c) 2013 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://kylelutz.github.com/compute for more information.
//---------------------------------------------------------------------------//

#ifndef BOOST_COMPUTE_ALGORITHM_EXCLUSIVE_SCAN_HPP
#define BOOST_COMPUTE_ALGORITHM_EXCLUSIVE_SCAN_HPP

#include <boost/compute/system.hpp>
#include <boost/compute/command_queue.hpp>
#include <boost/compute/algorithm/detail/scan.hpp>

namespace boost {
namespace compute {

/// Performs an exclusive scan of the elements in the range [\p first, \p last)
/// and stores the results in the range beginning at \p result.
///
/// Each element in the output is assigned to the sum of all the previous
/// values in the input.
///
/// \param first first element in the range to scan
/// \param last last element in the range to scan
/// \param result first element in the result range
/// \param queue command queue to perform the operation
///
/// \return \c OutputIterator to the end of the result range
///
/// \snippet test/test_scan.cpp exclusive_scan_int
///
/// \see inclusive_scan()
template<class InputIterator, class OutputIterator>
inline OutputIterator
exclusive_scan(InputIterator first,
               InputIterator last,
               OutputIterator result,
               command_queue &queue = system::default_queue())
{
    return detail::scan(first, last, result, true, queue);
}

} // end compute namespace
} // end boost namespace

#endif // BOOST_COMPUTE_ALGORITHM_EXCLUSIVE_SCAN_HPP
