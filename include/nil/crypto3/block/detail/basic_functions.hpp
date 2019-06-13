//---------------------------------------------------------------------------//
// Copyright (c) 2018-2019 Nil Foundation AG
// Copyright (c) 2018-2019 Mikhail Komarov <nemo@nilfoundation.org>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_BASIC_FUNCTIONS_HPP
#define CRYPTO3_BASIC_FUNCTIONS_HPP

#include <vector>

#include <boost/integer.hpp>
#include <boost/static_assert.hpp>

namespace nil {
    namespace crypto3 {
        namespace block {
            namespace detail {
                template<std::size_t WordBits>
                struct basic_functions {
                    typedef typename boost::uint_t<CHAR_BIT>::exact byte_type;

                    constexpr static const std::size_t word_bits = WordBits;
                    typedef typename boost::uint_t<word_bits>::exact word_type;

                    template<std::size_t Size, typename Integer>
                    static inline typename boost::uint_t<Size>::exact extract_uint_t(Integer v, std::size_t position) {
                        return static_cast<typename boost::uint_t<Size>::exact>(v
                                >> (((~position) & (sizeof(Integer) - 1)) << 3));
                    }

                    template<std::size_t Size, typename T>
                    static inline typename boost::uint_t<Size>::exact make_uint_t(
                            const std::initializer_list<T> &args) {
                        typedef typename std::initializer_list<T>::value_type value_type;
                        typename boost::uint_t<Size>::exact result = 0;

#pragma clang loop unroll(full)
                        for (const value_type &itr : args) {
                            result = static_cast<typename boost::uint_t<Size>::exact>(
                                    (result << std::numeric_limits<value_type>::digits) | itr);
                        }

                        return result;
                    }

                    template<std::size_t Size, typename ...Args>
                    static inline typename boost::uint_t<Size>::exact make_uint_t(Args... args) {
                        return basic_functions<WordBits>::template make_uint_t<Size, typename std::tuple_element<0,
                                                                                                                 std::tuple<
                                                                                                                         Args...>>::type>(
                                {args...});
                    }

                    static inline word_type shr(word_type x, std::size_t n) {
                        return x >> n;
                    }

                    template<std::size_t n>
                    static inline word_type shr(word_type x) {
                        BOOST_STATIC_ASSERT(n < word_bits);
                        return x >> n;
                    }

                    static inline word_type shl(word_type x, std::size_t n) {
                        return x << n;
                    }

                    template<std::size_t n>
                    static inline word_type shl(word_type x) {
                        BOOST_STATIC_ASSERT(n < word_bits);
                        return x << n;
                    }

                    static inline word_type rotr(word_type x, std::size_t n) {
                        return shr(x, n) | shl(x, word_bits - n);
                    }

                    template<std::size_t n>
                    static inline word_type rotr(word_type x) {
                        return shr<n>(x) | shl<word_bits - n>(x);
                    }

                    static inline word_type rotl(word_type x, std::size_t n) {
                        return shl(x, n) | shr(x, word_bits - n);
                    }

                    template<std::size_t n>
                    static inline word_type rotl(word_type x) {
                        return shl<n>(x) | shr<word_bits - n>(x);
                    }
                };
            } // namespace detail
        }
    }
} // namespace nil

#endif // CRYPTO3_BASIC_FUNCTIONS_HPP
