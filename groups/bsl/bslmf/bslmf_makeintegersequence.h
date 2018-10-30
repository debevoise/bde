// bslmf_makeintegersequence.h                                        -*-C++-*-
#ifndef INCLUDED_BSLMF_MAKEINTEGERSEQUENCE
#define INCLUDED_BSLMF_MAKEINTEGERSEQUENCE

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a template parameter pack of integers.
//
//@CLASSES:
//  bslmf::MakeIntegerSequence: compile-time integer sequence factory function
//
//@DESCRIPTION: This component defines a factory function
// 'bslmf::MakeIntegerSequence' that generate a compile-time sequence of
// increasing integer values starting with 0.
//
// 'bslmf::MakeIntegerSequence' meets the requirements of the
// 'make_integer_sequence' template introduced in the C++14 standard
// [intseq.intseq] and can be used in a client's code if the compiler supports
// the C++11 standard.
//
// Note, that this component has no (emulated) support for pre-standard C++11
// compilers, as its only purpose is to be used in deduction with template
// parameter packs.  There is no language support to emulate in earlier
// compilers.
//
///Usage
///-----
// In this section we show intended use of this component.
//
///Example 1: Pass C-array as a parameter to a function with variadic template
///- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Suppose we want to initialize a C-Array of known size 'N' with data read
// from a data source using a library class that provides a variadic template
// interface that loads a data of variable length into the supplied parameter
// pack.
//
// First, define a class template 'DataReader',
//..
// template <std::size_t N>
// class DataReader {
//   public:
//..
// Then, implement a method that loads the specified parameter pack 'args' with
// data read from a data source.
//..
//     template <class ...T>
//     void read(T*... args) const
//     {
//         static_assert(sizeof...(args) == N, "");
//         read_impl(args...);
//     }
//..
// Next, for the test purpose provide simple implementation of the recursive
// variadic 'read_impl' function that streams the index of the C-Array's
// element to 'stdout'.
//..
// private:
//     template <class U, class ...T>
//     void read_impl(U*, T*... args) const
//     {
//         printf("read element #%i\n",
//                static_cast<int>(N - 1 - sizeof...(args)));
//         read_impl(args...);
//     }
//..
// Then, implement the recursion break condition.
//..
//     void read_impl() const
//     {
//     }
// };
//..
// Next, define a helper function template 'readData' that expands the
// parameter pack of indices 'I' and invokes the variadic template 'read'
// method of the specified 'reader' object.
//..
// namespace {
// template<class R, class T, std::size_t... I>
// void readData(const R&  reader,
//               T        *data,
//               bslmf::IntegerSequence<std::size_t, I...>)
// {
//     reader.read(&data[I]...);
//         // In pseudocode, this is equivalent to:
//         // reader.read(&data[0],
//         //             &data[1],
//         //             &data[2],
//         //             ...
//         //             &data[N-1]);
// }
// }
//..
// Now, define function template 'readData' that invokes the helper function
// Note, that the 'bslmf::MakeIntegerSequence<std::size_t, N>' function
// generates an object of an integer sequence class instantiated with a
// template parameter pack of integers that will be expanded and used as an
// array's indices in the helper function when calling the
// 'Reader::read(T*...)' variadic template function.
//..
// template<class T, std::size_t N>
// void readData(const DataReader<N>& reader, T *data)
// {
//     readData(reader, data, bslmf::MakeIntegerSequence<std::size_t, N>());
// }
//..
// Finally, define a 'data' C-Array and 'reader' variables and pass them to the
// 'readData' function as parameters.
//..
// constexpr int      k_SIZE = 5;
// DataReader<k_SIZE> reader;
// int                data[k_SIZE] = {0};
//
// readData(reader, data);
//..
// The streaming operator produces output in the following format on 'stdout':
//..
// read element #0
// read element #1
// read element #2
// read element #3
// read element #4
//..

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLMF_INTEGERSEQUENCE
#include <bslmf_integersequence.h>
#endif

#ifndef INCLUDED_BSLMF_INTEGRALCONSTANT
#include <bslmf_integralconstant.h>
#endif

#ifndef INCLUDED_BSLS_LIBRARYFEATURES
#include <bsls_libraryfeatures.h>
#endif

#ifndef INCLUDED_CSTDDEF
#include <cstddef>  // 'std::size_t'
#define INCLUDED_CSTDDEF
#endif

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP14_INTEGER_SEQUENCE

namespace BloombergLP {
namespace bslmf {
                     // =============================================
                     // private struct MakeIntegerSequence_ConcatUtil
                     // =============================================

template <class S1, class S2>
struct MakeIntegerSequence_ConcatUtil;
    // This component-private class template provides a specialization that
    // concatenates two integer sequences.  This template is not defined unless
    // the (template parameter) types 'S1' and 'S2' are specializations of the
    // class template 'bslmf::IntegerSequence'.

template <class T, T... I1, T... I2>
struct MakeIntegerSequence_ConcatUtil<bslmf::IntegerSequence<T, I1...>,
                                      bslmf::IntegerSequence<T, I2...>>
    // This partial specialization of 'bslmf::MakeIntegerSequence_ConcatUtil'
    // appends all indices from the specified 'I2' sequence at the end of the
    // indices of the specified 'I1' sequence.
{
    using type = bslmf::IntegerSequence<T, I1..., (sizeof...(I1) + I2)...>;
        // 'type' is an alias to an integer sequence type that represents the
        // result of concatenation of two integer sequences 'I1' and 'I2'.
        //
        // Consider 'I1 = {0, 1, 2}' and 'I2 = {0, 1, 2}', then the result of
        // concatenation is 'type = {0, 1, 2, 3 + 0, 3 + 1, 3 + 2}'. That is
        // 'type = {0, 1, 2, 3, 4, 5}'.
};

template <class S1, class S2>
using MakeIntegerSequence_ConcatUtil_t =
                         typename MakeIntegerSequence_ConcatUtil<S1, S2>::type;
    // 'bslmf::MakeIntegerSequence_ConcatUtil_t' is an alias to the result type
    // of the 'bslmf::MakeIntegerSequence_ConcatUtil' meta-function.

                     // =======================================
                     // private struct MakeIntegerSequence_Impl
                     // =======================================

template <class T, class N>
struct MakeIntegerSequence_Impl
    // This is recursive meta-function that generates a sequence of increasing
    // integer values in a range of [0..N::value) having the specified length
    // 'N::value'.
{
    using FirstPart  = bsl::integral_constant<std::size_t, N::value/2>;
    using SecondPart =
                    bsl::integral_constant<std::size_t, N::value - N::value/2>;

    using type = MakeIntegerSequence_ConcatUtil_t<
                       typename MakeIntegerSequence_Impl<T, FirstPart >::type,
                       typename MakeIntegerSequence_Impl<T, SecondPart>::type>;
        // 'type' is an alias to the result of the
        // 'bslmf::MakeIntegerSequence_ConcatUtil_t' meta-function instantiated
        // with two integer sequences of two lengths 'N/2' and 'N - N/2', that
        // implies logarithmic depth of the 'bslmf::MakeIntegerSequence_Impl'
        // meta-function instantiation.
};

template <class T>
struct MakeIntegerSequence_Impl<T, bsl::integral_constant<std::size_t, 0> >
    // This partial specialization of the 'bslmf::MakeIntegerSequence_Impl'
    // meta-function is a recursion break condition for an empty integer
    // sequence.
{
    using type = bslmf::IntegerSequence<T>;
};

template <class T>
struct MakeIntegerSequence_Impl<T, bsl::integral_constant<std::size_t, 1> >
    // This partial specialization of the 'bslmf::MakeIntegerSequence_Impl'
    // meta-function is a recursion break condition for an integer sequence
    // having the length 1.
{
    using type = bslmf::IntegerSequence<T, 0>;
};

template <class T>
struct MakeIntegerSequence_Impl<T, bsl::integral_constant<std::size_t, 2> >
    // This partial specialization of the 'bslmf::MakeIntegerSequence_Impl'
    // meta-function is a recursion break condition for an integer sequence
    // having the length 2.
{
    using type = bslmf::IntegerSequence<T, 0, 1>;
};

template <class T>
struct MakeIntegerSequence_Impl<T, bsl::integral_constant<std::size_t, 3> >
    // This partial specialization of the 'bslmf::MakeIntegerSequence_Impl'
    // meta-function is a recursion break condition for an integer sequence
    // having the length 3.
{
    using type = bslmf::IntegerSequence<T, 0, 1, 2>;
};

template <class T>
struct MakeIntegerSequence_Impl<T, bsl::integral_constant<std::size_t, 4> >
    // This partial specialization of the 'bslmf::MakeIntegerSequence_Impl'
    // meta-function is a recursion break condition for an integer sequence
    // having the length 4.
{
    using type = bslmf::IntegerSequence<T, 0, 1, 2, 3>;
};

template <class T>
struct MakeIntegerSequence_Impl<T, bsl::integral_constant<std::size_t, 5> >
    // This partial specialization of the 'bslmf::MakeIntegerSequence_Impl'
    // meta-function is a recursion break condition for an integer sequence
    // having the length 5.
{
    using type = bslmf::IntegerSequence<T, 0, 1 ,2, 3, 4>;
};

template <class T>
struct MakeIntegerSequence_Impl<T, bsl::integral_constant<std::size_t, 6> >
    // This partial specialization of the 'bslmf::MakeIntegerSequence_Impl'
    // meta-function is a recursion break condition for an integer sequence
    // having the length 6.
{
    using type = bslmf::IntegerSequence<T, 0, 1, 2, 3, 4, 5>;
};

template <class T>
struct MakeIntegerSequence_Impl<T, bsl::integral_constant<std::size_t, 7> >
    // This partial specialization of the 'bslmf::MakeIntegerSequence_Impl'
    // meta-function is a recursion break condition for an integer sequence
    // having the length 7.
{
    using type = bslmf::IntegerSequence<T, 0, 1, 2, 3, 4, 5, 6>;
};

template <class T>
struct MakeIntegerSequence_Impl<T, bsl::integral_constant<std::size_t, 8> >
    // This partial specialization of the 'bslmf::MakeIntegerSequence_Impl'
    // meta-function is a recursion break condition for an integer sequence
    // having the length 8.
{
    using type = bslmf::IntegerSequence<T, 0, 1, 2, 3, 4, 5, 6, 7>;
};

// ALIASES
template <class T, class N>
using MakeIntegerSequence_Impl_t =
                                 typename MakeIntegerSequence_Impl<T, N>::type;
    // 'bslmf::MakeIntegerSequence_Impl_t' is an alias to the result type of
    // the 'bslmf::MakeIntegerSequence_Impl' meta-function.

template <class T, T N>
using MakeIntegerSequence = BloombergLP::bslmf::MakeIntegerSequence_Impl_t<
                                      T,
                                      bsl::integral_constant<std::size_t, N> >;
    // 'MakeIntegerSequence' is defined to simplify creation of
    // 'bslmf::IntegerSequence' type that represents a collection of increasing
    // integer values of the specified type 'T' in a range of [0..N) having the
    // specified N-value length.

}  // close package namespace
}  // close enterprise namespace

#endif  // BSLS_LIBRARYFEATURES_HAS_CPP14_INTEGER_SEQUENCE
#endif  // INCLUDED_BSLMF_INTEGERSEQUENCE

// ----------------------------------------------------------------------------
// Copyright 2018 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
