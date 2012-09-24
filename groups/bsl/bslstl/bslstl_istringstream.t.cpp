// bslstl_istringstream.t.cpp                                         -*-C++-*-
#include <bslstl_istringstream.h>
#include <bslstl_string.h>
#include <bslstl_allocator.h>
#include <bslma_allocator.h>
#include <bslma_default.h>
#include <bslma_testallocator.h>
#include <bslma_defaultallocatorguard.h>

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <algorithm>

#include <cstdio>
#include <cstdlib>
#include <cstring>

//=============================================================================
//                                 TEST PLAN
//-----------------------------------------------------------------------------
//                                  Overview
//                                  --------
// 'bsl::basic_istringstream' is a simple wrapper over 'std::basic_istream'
// that uses 'bsl::basic_stringbuf' as an input buffer.  Therefore we don't
// neeed to test all the functionality derived from 'std::basic_istream', only
// some basic input functions and the new methods provided by
// 'bsl::basic_istringstream' itself.
// =============================
// ISTRINGSTREAM:
// [ 2] TESTING ISTRINGSTREAM
//-----------------------------------------------------------------------------
// [ 3] USAGE EXAMPLE
// [ 1] BREATHING TEST

//==========================================================================
//                       STANDARD BDE ASSERT TEST MACRO
//--------------------------------------------------------------------------

using std::printf;
using std::fflush;
using std::atoi;

namespace {

int testStatus = 0;

void aSsErT(int c, const char *s, int i)
{
    if (c) {
        printf("Error " __FILE__ "(%d): %s    (failed)\n", i, s);
        if (testStatus >= 0 && testStatus <= 100) ++testStatus;
    }
}

}  // close unnamed namespace

#define ASSERT(X) { aSsErT(!(X), #X, __LINE__); }

#define ASSERT_FAIL(expr) BSLS_ASSERTTEST_ASSERT_FAIL(expr)
#define ASSERT_PASS(expr) BSLS_ASSERTTEST_ASSERT_PASS(expr)
#define ASSERT_SAFE_FAIL(expr) BSLS_ASSERTTEST_ASSERT_SAFE_FAIL(expr)
#define ASSERT_SAFE_PASS(expr) BSLS_ASSERTTEST_ASSERT_SAFE_PASS(expr)

#define ASSERT_FAIL_RAW(expr) BSLS_ASSERTTEST_ASSERT_FAIL_RAW(expr)
#define ASSERT_PASS_RAW(expr) BSLS_ASSERTTEST_ASSERT_PASS_RAW(expr)
#define ASSERT_SAFE_FAIL_RAW(expr) BSLS_ASSERTTEST_ASSERT_SAFE_FAIL_RAW(expr)
#define ASSERT_SAFE_PASS_RAW(expr) BSLS_ASSERTTEST_ASSERT_SAFE_PASS_RAW(expr)

//=============================================================================
//                    STANDARD BDE LOOP-ASSERT TEST MACROS
//-----------------------------------------------------------------------------
// NOTE: This implementation of LOOP_ASSERT macros must use printf since
//       cout uses new and must not be called during exception testing.

#define LOOP_ASSERT(I,X) { \
    if (!(X)) { printf("%s", #I ": "); dbg_print(I); printf("\n"); \
                fflush(stdout); aSsErT(1, #X, __LINE__); } }

#define LOOP2_ASSERT(I,J,X) { \
    if (!(X)) { printf("%s", #I ": "); dbg_print(I); printf("\t"); \
                printf("%s", #J ": "); dbg_print(J); printf("\n"); \
                fflush(stdout); aSsErT(1, #X, __LINE__); } }

#define LOOP3_ASSERT(I,J,K,X) {                    \
    if (!(X)) { printf("%s", #I ": "); dbg_print(I); printf("\t"); \
                printf("%s", #J ": "); dbg_print(J); printf("\t"); \
                printf("%s", #K ": "); dbg_print(K); printf("\n"); \
                fflush(stdout); aSsErT(1, #X, __LINE__); } }

#define LOOP4_ASSERT(I,J,K,L,X) {                  \
    if (!(X)) { printf("%s", #I ": "); dbg_print(I); printf("\t"); \
                printf("%s", #J ": "); dbg_print(J); printf("\t"); \
                printf("%s", #K ": "); dbg_print(K); printf("\t"); \
                printf("%s", #L ": "); dbg_print(L); printf("\n"); \
                fflush(stdout); aSsErT(1, #X, __LINE__); } }

#define LOOP5_ASSERT(I,J,K,L,M,X) {                \
    if (!(X)) { printf("%s", #I ": "); dbg_print(I); printf("\t"); \
                printf("%s", #J ": "); dbg_print(J); printf("\t"); \
                printf("%s", #K ": "); dbg_print(K); printf("\t"); \
                printf("%s", #L ": "); dbg_print(L); printf("\t"); \
                printf("%s", #M ": "); dbg_print(M); printf("\n"); \
                fflush(stdout); aSsErT(1, #X, __LINE__); } }

#define LOOP6_ASSERT(I,J,K,L,M,N,X) {                \
    if (!(X)) { printf("%s", #I ": "); dbg_print(I); printf("\t"); \
                printf("%s", #J ": "); dbg_print(J); printf("\t"); \
                printf("%s", #K ": "); dbg_print(K); printf("\t"); \
                printf("%s", #L ": "); dbg_print(L); printf("\t"); \
                printf("%s", #M ": "); dbg_print(M); printf("\n"); \
                printf("%s", #N ": "); dbg_print(N); printf("\n"); \
                fflush(stdout); aSsErT(1, #X, __LINE__); } }

//=============================================================================
//                      SEMI-STANDARD TEST OUTPUT MACROS
//-----------------------------------------------------------------------------
#define Q(X) printf("<| " #X " |>\n");     // Quote identifier literally.
#define P(X) dbg_print(#X " = ", X, "\n")  // Print identifier and value.
#define P_(X) dbg_print(#X " = ", X, ", ") // P(X) without '\n'
#define L_ __LINE__                        // current Line number
#define T_ putchar('\t');                  // Print a tab (w/o newline)

//=============================================================================
//                    GLOBAL HELPER FUNCTIONS FOR TESTING
//-----------------------------------------------------------------------------

// Fundamental-type-specific print functions.
inline void dbg_print(bool b) { printf(b ? "true" : "false"); fflush(stdout); }
inline void dbg_print(char c) { printf("%c", c); fflush(stdout); }
inline void dbg_print(unsigned char c) { printf("%c", c); fflush(stdout); }
inline void dbg_print(signed char c) { printf("%c", c); fflush(stdout); }
inline void dbg_print(short val) { printf("%d", (int)val); fflush(stdout); }
inline void dbg_print(unsigned short val) {
    printf("%d", (int)val); fflush(stdout);
}
inline void dbg_print(int val) { printf("%d", val); fflush(stdout); }
inline void dbg_print(unsigned int val) { printf("%u", val); fflush(stdout); }
inline void dbg_print(long val) { printf("%ld", val); fflush(stdout); }
inline void dbg_print(unsigned long val) {
    printf("%lu", val); fflush(stdout);
}
inline void dbg_print(long long val) { printf("%lld", val); fflush(stdout); }
inline void dbg_print(unsigned long long val) {
    printf("%llu", val); fflush(stdout);
}
inline void dbg_print(float val) {
    printf("'%f'", (double)val); fflush(stdout);
}
inline void dbg_print(double val) { printf("'%f'", val); fflush(stdout); }
inline void dbg_print(long double val) {
    printf("'%Lf'", val); fflush(stdout);
}
inline void dbg_print(const char* s) { printf("\"%s\"", s); fflush(stdout); }
inline void dbg_print(char* s) { printf("\"%s\"", s); fflush(stdout); }
inline void dbg_print(void* p) { printf("%p", p); fflush(stdout); }

//=============================================================================
//               GLOBAL HELPER CLASSES AND FUNCTION FOR TESTING
//-----------------------------------------------------------------------------

namespace
{

template <typename Stream, typename BaseStream>
void testCreateDefaultStream()
{
    Stream strm1;
    ASSERT(strm1.str().empty());

    BaseStream & strmref = strm1;
    ASSERT(strm1.rdbuf());
    ASSERT(strm1.rdbuf() == strmref.rdbuf());

    Stream strm2(std::ios_base::in);
    ASSERT(strm2.str().empty());

    Stream strm3(std::ios_base::out);
    ASSERT(strm3.str().empty());

    Stream strm4(std::ios_base::in | std::ios_base::out);
    ASSERT(strm4.str().empty());
}

template <typename Istream>
void testCreateIstreamWithAllocator()
{
    using namespace BloombergLP;

    Istream strm1(std::ios_base::in, bsl::allocator<char>());
    Istream strm2(std::ios_base::in, bslma::Default::allocator());
    Istream strm3(bsl::string("something"),
                  std::ios_base::in,
                  bslma::Default::allocator());
}

template <typename Istream>
void testCreateIstreamWithString()
{
    std::string init("abc");

    Istream strm1(init);
    ASSERT(strm1.str() == init);

    Istream strm2(init, std::ios_base::in);
    ASSERT(strm2.str() == init);

    Istream strm3(init, std::ios_base::out);
    ASSERT(strm3.str() == init);

    Istream strm4(init, std::ios_base::in | std::ios_base::out);
    ASSERT(strm4.str() == init);
}

template <typename Istream>
void testIstreamBasicInput()
{
    Istream strm1;
    ASSERT(!strm1.eof());

    char c;
    strm1 >> c;
    ASSERT(strm1.eof());

    Istream strm2("abc");
    strm2 >> c;
    ASSERT(c == 'a');
    strm2 >> c;
    ASSERT(c == 'b');
    strm2 >> c;
    ASSERT(c == 'c');
    strm2 >> c;
    ASSERT(strm2.eof());

    Istream strm3("123abc");
    int i;
    strm3 >> i;
    ASSERT(i == 123);
    ASSERT(strm3.tellg() == native_std::streampos(3));

    strm3.seekg(1);
    strm3 >> i;
    ASSERT(i == 23);
    ASSERT(strm3.tellg() == native_std::streampos(3));
}

}

//=============================================================================
//                               USAGE EXAMPLE
//-----------------------------------------------------------------------------

namespace
{
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Basic input operations
///- - - - - - - - - - - - - - - - -
// The following example demonstrates the use of 'bsl::istringstream' to read
// data of various types from a 'bsl::string' object.
//
// Suppose we want to implement a simplified converter from a generic type
// 'TYPE' to 'bsl::string'.  We use 'bsl::istringstream' to implement the
// 'fromStream' function.  We initialize the input stream with the string
// passed as a parameter and the we read the data from the input stream with
// 'operator>>':
//..
template <typename TYPE>
TYPE fromString(const bsl::string& from)
{
    bsl::istringstream in(from);
    TYPE val;
    in >> val;
    return val;
}
//..
}

//=============================================================================
//                                MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    using namespace BloombergLP;

    int test = argc > 1 ? atoi(argv[1]) : 0;

    int verbose = argc > 2;
    int veryVerbose = argc > 3;
    // int veryVeryVerbose = argc > 4;
    int veryVeryVeryVerbose = argc > 5;

    // As part of our overall allocator testing strategy, we will create
    // three test allocators.

    bslma::TestAllocator *globalAllocator_p,
                         *defaultAllocator_p,
                         *objectAllocator_p;

    // Object Test Allocator.
    bslma::TestAllocator objectAllocator("Object Allocator",
                                         veryVeryVeryVerbose);
    objectAllocator_p = &objectAllocator;
    (void) objectAllocator_p;

    // Default Test Allocator.
    bslma::TestAllocator defaultAllocator("Default Allocator",
                                          veryVeryVeryVerbose);
    bslma::DefaultAllocatorGuard guard(&defaultAllocator);
    defaultAllocator_p = &defaultAllocator;
    (void) defaultAllocator_p;

    // Global Test Allocator.
    bslma::TestAllocator  globalAllocator("Global Allocator",
                                          veryVeryVeryVerbose);
    bslma::Allocator *originalGlobalAllocator =
                          bslma::Default::setGlobalAllocator(&globalAllocator);
    globalAllocator_p = &globalAllocator;
    (void) globalAllocator_p;

    setbuf(stdout, NULL);    // Use unbuffered output

    printf("TEST " __FILE__ " CASE %d\n", test);

    switch (test) { case 0:  // Zero is always the leading case.
      case 3: {
        // --------------------------------------------------------------------
        // USAGE EXAMPLE
        //
        // Concerns:
        //: 1 Usage example compiles and executes without failures.
        //
        // Plan:
        //: 1 Copy and paste the usage example from the component header into
        //:   the test driver and replace 'assert' with 'ASSERT'.
        // --------------------------------------------------------------------

        if (verbose) printf("\nUSAGE EXAMPLE"
                            "\n=============\n");

// Finally, we verify that our 'fromString' function works on some simple test
// cases:
//..
        ASSERT(fromString<int>("1234") == 1234);
        ASSERT(fromString<short>("-5") == -5);
        ASSERT(fromString<bsl::string>("abc") == "abc");
//..
      } break;
      case 2: {
        // --------------------------------------------------------------------
        // TESTING ISTRINGSTREAM
        //
        // Concerns:
        //: 1 'istringstream' object can be created with a with default
        //:   constructor.
        //: 2 'istringstream' object can be created with a constructor with
        //:   allocator.
        //: 3 'istringstream' object can be created with a constructor with
        //:   string.
        //: 4 'istringstream' object can be used to perform basic input
        //:   operations.
        //
        // Plan:
        //: 1 Create 'istringstream' object with a default constructor.
        //: 2 Create 'istringstream' object with a constructor with allocator.
        //: 3 Create 'istringstream' object with a constructor with string.
        //: 4 Exersice basic 'istringstream' input operations.
        // --------------------------------------------------------------------

        if (verbose) printf("\nTESTING ISTRINGSTREAM"
                            "\n=====================\n");

        if (veryVerbose) printf("\tcreate default istringstream\n");

        testCreateDefaultStream<bsl::istringstream, std::istream>();

        if (veryVerbose) printf("\tcreate istringstream with allocator\n");

        testCreateIstreamWithAllocator<bsl::istringstream>();

        if (veryVerbose) printf("\tcreate istringstream with string\n");

        testCreateIstreamWithString<bsl::istringstream>();

        if (veryVerbose) printf("\tbasic input from istringstream\n");

        testIstreamBasicInput<bsl::istringstream>();

      } break;
      case 1: {
        // --------------------------------------------------------------------
        // BREATHING TEST
        //
        // Concerns:
        //   We want to exercise the basic functionality.
        //
        // Plan:
        //   This "test" *exercises* basic functionality.
        // --------------------------------------------------------------------

        if (verbose) printf("\nBREATHING TEST"
                            "\n==============\n");

      } break;
      default: {
        fprintf(stderr, "WARNING: CASE `%d' NOT FOUND.\n", test);
        testStatus = -1;
      }
    }

    bslma::Default::setGlobalAllocator(originalGlobalAllocator);

    if (testStatus > 0) {
        fprintf(stderr, "Error, non-zero test status = %d.\n", testStatus);
    }

    return testStatus;
}


// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2012
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
