// bslmf_isconvertible.t.cpp                                          -*-C++-*-

#include <bslmf_isconvertible.h>

#include <bsls_platform.h>

#include <cstdlib>     // atoi()
#include <cstring>     // strcmp()
#include <iostream>

// These 4 compilers cannot handle conversions to/from cv-qualified types
// in all cases.
#if defined(BSLS_PLATFORM__CMP_SUN)  \
 || defined(BSLS_PLATFORM__CMP_MSVC) \
 || defined(BSLS_PLATFORM__CMP_HP)   \
 || defined(BSLS_PLATFORM__CMP_HP)   \
 || defined(BSLS_PLATFORM__CMP_CLANG)
    #define BSLMF_ODD_COMPILER_CONST_OR_VOLATILE_CONVERSION_RULES
#endif

using namespace BloombergLP;

//=============================================================================
//                                TEST PLAN
//-----------------------------------------------------------------------------
//                                Overview
//                                --------
//-----------------------------------------------------------------------------
// [ 1] bslmf::IsConvertible
// [ 2] bslmf::IsConvertible warnings during conversion
//-----------------------------------------------------------------------------
// [ 3] Usage Example
//=============================================================================
//                  STANDARD BDE ASSERT TEST MACRO
//-----------------------------------------------------------------------------
static int testStatus = 0;

static void aSsErT(int c, const char *s, int i) {
    if (c) {
        std::cout << "Error " << __FILE__ << "(" << i << "): " << s
                  << "    (failed)" << std::endl;
        if (testStatus >= 0 && testStatus <= 100) ++testStatus;
    }
}
#define ASSERT(X) { aSsErT(!(X), #X, __LINE__); }
//-----------------------------------------------------------------------------
#define LOOP_ASSERT(I,X) { \
   if (!(X)) { std::cout << #I << ": " << I << "\n"; aSsErT(1, #X, __LINE__);}}

#define LOOP2_ASSERT(I,J,X) { \
    if (!(X)) { std::cout << #I << ": " << I << "\t" << #J << ": " \
        << J << "\n"; aSsErT(1, #X, __LINE__); } }
//=============================================================================
#define P(X) std::cout << #X " = " << (X) << std::endl; // Print identifier and
                                                        // value.
#define Q(X) std::cout << "<| " #X " |>" << std::endl;  // Quote identifier
                                                        // literally.
#define P_(X) std::cout << #X " = " << (X) << ", " << flush;
                                                        // P(X) without '\n'
#define L_ __LINE__                                     // current Line number
#define T_() std::cout << '\t' << flush;             // Print tab w/o linefeed.

//=============================================================================
//                  GLOBAL TYPEDEFS/CONSTANTS FOR TESTING
//-----------------------------------------------------------------------------

template <typename T>
class ConvertibleTo {
    // Object that's convertible to 'T'.
    T d_value;

  public:
    ConvertibleTo();  // Declared but not defined
    operator T const&() const { return d_value; }
    operator T&() { return d_value; }
    operator T() const { return d_value; }
};

template <typename T>
class ConvertibleFrom {
    // Object that's convertible From 'T', for use in name only.
    T d_value;

  public:
    ConvertibleFrom() : d_value() { }
    ConvertibleFrom(T value) : d_value(value) { }
};

class my_Class {
    int d_i;

  public:
    // Convertible from int
    my_Class(int i) : d_i(i) {}
};

class my_OtherClass
{
    // Convertible to my_Class
  public:
    operator my_Class&();
};

class my_ThirdClass
{
    // Convertible from my_Class
  public:
    my_ThirdClass(const my_Class&);
};

class my_AbstractClass
{
    // Abstract class
  public:
    my_AbstractClass() { }
    virtual ~my_AbstractClass() = 0;

    int func1(int);
};

class my_DerivedClass : public my_AbstractClass
{
  public:
    my_DerivedClass() { }
    ~my_DerivedClass();

    int func2(int);
};

class my_IncompleteClass;
class my_IncompleteClass2;

enum my_Enum { MY_VAL0, MY_VAL1 };

class my_EnumClass
{
  public:
    enum Type { VAL0, VAL1 };
};

class my_BslmaAllocator;

template <typename TYPE>
class my_StlAllocator
{
  public:
    my_StlAllocator(my_BslmaAllocator*);
};

// Because this is a very low-level component, we are not allowed to use
// '#include <new>' nor on '#include <bslma::PlacementNew>'.  Thus, we create
// our own placement new that won't conflict with the standard one but does
// the same thing.
struct my_PlacementNew { void *d_p; my_PlacementNew(void *p) : d_p(p) { } };
void *operator new(size_t, my_PlacementNew p) { return p.d_p; }

// from component doc

static const int A = bslmf::IsConvertible<int, char >::value; // A is 1
static const int B = bslmf::IsConvertible<int, char*>::value; // B is 0

// verify that the 'value' member is evaluated at compile-time

static char C0[1 + bslmf::IsConvertible<int, int>::value];     // sz=2
static char C1[1 + bslmf::IsConvertible<int, char>::value];    // sz=2
static char C2[1 + bslmf::IsConvertible<void*, int>::value];   // sz=1
static char C3[1 + bslmf::IsConvertible<int, int *>::value];   // sz=1

// Usage Example
// ----------------------------------------------------------------------------

struct MyAllocator {
    void *allocate(size_t sz)       { return operator new(sz); }
    void  deallocate(void *address) { operator delete(address); }
};

struct Foo {
    Foo(MyAllocator *) {}
};

struct Bar {
    Bar() {}
};

template<class T>
void createObj(T *space, MyAllocator *, bsl::false_type)
{
   // Use the type's default constructor if
   // bslmf::IsConvertible<MyAllocator*, T>::value == 0, i.e., there is
   // no conversion from a MyAllocator pointer to a T.

   new (space) T();
}

template<class T>
void createObj(T *space, MyAllocator *alloc, bsl::true_type)
{
   // Use the type's constructor that takes a pointer to an allocator if
   // bslmf::IsConvertible<MyAllocator*, T>::value == 1, i.e., there is
   // a conversion from a MyAllocator pointer to a T.

   new (space) T(alloc);
}

template <class T>
class MyContainer {
    T           *d_array_p; // underlying array
    MyAllocator *d_alloc_p; // allocator protocol
    int          d_length;  // logical length of array
    // ...

    void resizeInternalArrayIfNeeded() { /* ... */ }
  public:
    MyContainer(MyAllocator *alloc)
    : d_alloc_p(alloc)
    , d_length(0)
    {
        d_array_p = (T*) d_alloc_p->allocate(sizeof(T));
    }

    ~MyContainer()
    {
        d_alloc_p->deallocate(d_array_p);
    }

    void addObj()
    {
        resizeInternalArrayIfNeeded();
        // kludge to workaround some Sun's compiler weirdness
        // the code won't compile with just the typename
        typedef typename bslmf::IsConvertible<MyAllocator*, T>::type isAlloc;
        createObj(d_array_p + d_length++, d_alloc_p, isAlloc());
    }
};

//=============================================================================
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int test = argc > 1 ? std::atoi(argv[1]) : 0;
    int verbose = argc > 2;
    // int veryVerbose = argc > 3;

    // Silence compiler warnings about unused static variables
    (void) C0[0];
    (void) C1[0];
    (void) C2[0];
    (void) C3[0];

    std::cout << "TEST " << __FILE__ << " CASE " << test << std::endl;

    switch (test) { case 0:  // Zero is always the leading case.
      case 4: {
        // --------------------------------------------------------------------
        // Testing:
        //   Usage Example
        // --------------------------------------------------------------------

        if (verbose) std::cout << "\nbslmf_IsConvertible"
                               << "\n===================" << std::endl;

        MyAllocator a;

        MyContainer<int> ic(&a);
        ic.addObj();

        MyContainer<Foo> fc(&a);
        fc.addObj();

        MyContainer<Bar> bc(&a);
        bc.addObj();

      } break;
      case 3: {
        // --------------------------------------------------------------------
        // Concern:
        //   Gcc, for instance, generates a warning for integral to floating
        //   point conversions for implicit conversion.  This is vexing,
        //   because even fixing case 2 below does not take care of the
        //   user-defined classes convertible to or from fundamental types.
        //
        // Test Plan:
        //   Instantiate 'bslmf::IsConvertible' with various fundamental type
        //   combinations and verify that the 'VALUE' member is initialized
        //   properly, and that no warning is generated for conversions between
        //   floating point types and integral types.
        // --------------------------------------------------------------------

        if (verbose)
            std::cout << "\nTesting gcc warnings for float to int conversions"
                      << "\nvia a user-defined class"
                      << "\n================================================="
                      << std::endl;

        // Two user conversions is one too many.
        ASSERT(0 == (bslmf::IsConvertible<ConvertibleFrom<int>,
                                          ConvertibleTo<int> >::value));

        // Test conversion of basic types via a user-defined class.
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<int>, float>::value));
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<float>, int>::value));
        ASSERT(1 == (bslmf::IsConvertible<int,
                                          ConvertibleFrom<float> >::value));
        ASSERT(1 == (bslmf::IsConvertible<float,
                                          ConvertibleFrom<int> >::value));

        // Test const value conversions via a user-defined class.
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<int>,
                                          const float>::value));
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<float>,
                                          const int>::value));
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<const float>,
                                          int>::value));
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<const int>,
                                          float>::value));
        ASSERT(1 == (bslmf::IsConvertible<int,
                                      ConvertibleFrom<const float> >::value));
        ASSERT(1 == (bslmf::IsConvertible<float,
                                      ConvertibleFrom<const int> >::value));
        ASSERT(1 == (bslmf::IsConvertible<const float,
                                      ConvertibleFrom<int> >::value));
        ASSERT(1 == (bslmf::IsConvertible<const int,
                                      ConvertibleFrom<float> >::value));

        // Test volatile value conversions via a user-defined class.
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<int>,
                                          volatile float>::value));
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<float>,
                                          volatile int>::value));
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<volatile int>,
                                          float>::value));
        ASSERT(1 == (bslmf::IsConvertible<ConvertibleTo<volatile float>,
                                          int>::value));
        ASSERT(1 == (bslmf::IsConvertible<int,
                                  ConvertibleFrom<volatile float> >::value));
        ASSERT(1 == (bslmf::IsConvertible<float,
                                  ConvertibleFrom<volatile int> >::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile int,
                                  ConvertibleFrom<float> >::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile float,
                                  ConvertibleFrom<int> >::value));
      } break;
      case 2: {
        // --------------------------------------------------------------------
        // Concern:
        //   Gcc, for instance, generates a warning for integral to floating
        //   point conversions for implicit conversion.
        //
        // Test Plan:
        //   Instantiate 'bslmf::IsConvertible' with various fundamental type
        //   combinations and verify that the 'VALUE' member is initialized
        //   properly, and that no warning is generated for conversions between
        //   floating point types and integral types.
        // --------------------------------------------------------------------

        if (verbose)
            std::cout << "\nTesting gcc warnings for float to int conversions"
                      << "\n================================================="
                      << std::endl;

        // Test conversion of basic types.
        ASSERT(1 == (bslmf::IsConvertible<int, float>::value));
        ASSERT(1 == (bslmf::IsConvertible<float, int>::value));

        // Test const value conversions.
        ASSERT(1 == (bslmf::IsConvertible<int, const float>::value));
        ASSERT(1 == (bslmf::IsConvertible<float, const int>::value));
        ASSERT(1 == (bslmf::IsConvertible<const float, int>::value));
        ASSERT(1 == (bslmf::IsConvertible<const int, float>::value));

        // Test volatile value conversions.
        ASSERT(1 == (bslmf::IsConvertible<int, volatile float>::value));
        ASSERT(1 == (bslmf::IsConvertible<float, volatile int>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile int, float>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile float, int>::value));

        // Test volatile pointer and reference conversions
        // from integral to floating point.
        ASSERT(0 == (bslmf::IsConvertible<int*, volatile float*>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int*, float*>::value));
        ASSERT(0 == (bslmf::IsConvertible<int&, volatile float&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int&, float&>::value));
        ASSERT(0 == (bslmf::IsConvertible<int,  volatile float&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int,  float&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int&,  volatile float>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile int&,  float>::value));

        ASSERT(0 == (bslmf::IsConvertible<int*,
                                          const volatile float*>::value));
        ASSERT(0 == (bslmf::IsConvertible<const volatile int*,
                                          float*>::value));
#if !defined(BSLMF_ODD_COMPILER_CONST_OR_VOLATILE_CONVERSION_RULES)
       ASSERT(0 == (bslmf::IsConvertible<int&, const volatile float&>::value));
#endif
       ASSERT(0 == (bslmf::IsConvertible<const volatile int&, float&>::value));
#if !defined(BSLMF_ODD_COMPILER_CONST_OR_VOLATILE_CONVERSION_RULES)
       ASSERT(0 == (bslmf::IsConvertible<int,  const volatile float&>::value));
#endif
       ASSERT(0 == (bslmf::IsConvertible<const volatile int, float&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int&, const volatile float>::value));
        ASSERT(1 == (bslmf::IsConvertible<const volatile int&, float>::value));

        ASSERT(0 == (bslmf::IsConvertible<const int*,
                                          volatile float*>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int*,
                                          const float*>::value));
        ASSERT(1 == (bslmf::IsConvertible<const int&, volatile float>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile int&, const float>::value));
#if !defined(BSLMF_ODD_COMPILER_CONST_OR_VOLATILE_CONVERSION_RULES)
        ASSERT(0 == (bslmf::IsConvertible<const int&,
                                          const volatile float&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int&,
                                          const volatile float&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const int,
                                          const volatile float&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int,
                                          const volatile float&>::value));
#endif
        ASSERT(0 == (bslmf::IsConvertible<const int,
                                          volatile float&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const volatile int,
                                          volatile float&>::value));

        // Sun 5.2 and 5.5 get this right if the cv-unqualified types differ.
        ASSERT(1 == (bslmf::IsConvertible<volatile int,
                                          const float&>::value));
        ASSERT(1 == (bslmf::IsConvertible<const volatile int,
                                          const float&>::value));

        ASSERT(0 == (bslmf::IsConvertible<int*, float>::value));
        ASSERT(0 == (bslmf::IsConvertible<int, float*>::value));

        // Test volatile pointer and reference conversions
        // from floating point to integral.
        ASSERT(0 == (bslmf::IsConvertible<float*, volatile int*>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile float*, int*>::value));
        ASSERT(0 == (bslmf::IsConvertible<float&, volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile float&, int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<float,  volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile float,  int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<float&,  volatile int>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile float&,  int>::value));

        ASSERT(0 == (bslmf::IsConvertible<float*,
                                          const volatile int*>::value));
        ASSERT(0 == (bslmf::IsConvertible<const volatile float*,
                                          int*>::value));
#if !defined(BSLMF_ODD_COMPILER_CONST_OR_VOLATILE_CONVERSION_RULES)
       ASSERT(0 == (bslmf::IsConvertible<float&, const volatile int&>::value));
#endif
       ASSERT(0 == (bslmf::IsConvertible<const volatile float&, int&>::value));
#if !defined(BSLMF_ODD_COMPILER_CONST_OR_VOLATILE_CONVERSION_RULES)
       ASSERT(0 == (bslmf::IsConvertible<float,  const volatile int&>::value));
#endif
       ASSERT(0 == (bslmf::IsConvertible<const volatile float,  int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<float&, const volatile int>::value));
        ASSERT(1 == (bslmf::IsConvertible<const volatile float&, int>::value));

        ASSERT(0 == (bslmf::IsConvertible<const float*,
                                          volatile int*>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile float*,
                                          const int*>::value));
        ASSERT(1 == (bslmf::IsConvertible<const float&, volatile int>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile float&, const int>::value));
#if !defined(BSLMF_ODD_COMPILER_CONST_OR_VOLATILE_CONVERSION_RULES)
        ASSERT(0 == (bslmf::IsConvertible<const float&,
                                          const volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile float&,
                                          const volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const float,
                                          const volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile float,
                                          const volatile int&>::value));
#endif
        ASSERT(0 == (bslmf::IsConvertible<const float,
                                          volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const volatile float,
                                          volatile int&>::value));

// #ifndef BSLS_PLATFORM__CMP_SUN
        // Sun 5.2 and 5.5 both get this wrong
        ASSERT(1 == (bslmf::IsConvertible<volatile float,
                                          const int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<const volatile float,
                                          const int&>::value));
// #endif

        ASSERT(0 == (bslmf::IsConvertible<float*, int>::value));
        ASSERT(0 == (bslmf::IsConvertible<float, int*>::value));

      } break;
      case 1: {
        // --------------------------------------------------------------------
        // Test Plan:
        //   Instantiate 'bslmf::IsConvertible' with various type combinations
        //   and verify that the 'VALUE' member is initialized properly.
        // --------------------------------------------------------------------

        if (verbose) std::cout << "\nbslmf_IsConvertible"
                               << "\n===================" << std::endl;

        ASSERT(2 == sizeof(C0));
        ASSERT(2 == sizeof(C1));
        ASSERT(1 == sizeof(C2));
        ASSERT(1 == sizeof(C3));

        // Test conversion of basic types.
        ASSERT(1 == (bslmf::IsConvertible<int, int  >::value));
        ASSERT(1 == (bslmf::IsConvertible<int, char >::value));
        ASSERT(0 == (bslmf::IsConvertible<int, char*>::value));
        ASSERT(0 == (bslmf::IsConvertible<void*, char*>::value));
        ASSERT(1 == (bslmf::IsConvertible<char*, void*>::value));

        // Test const value conversions.
        ASSERT(1 == (bslmf::IsConvertible<int, const int>::value));
        ASSERT(1 == (bslmf::IsConvertible<const int, int>::value));

        // Test const pointer and reference conversions.
        ASSERT(1 == (bslmf::IsConvertible<int*, const int*>::value));
        ASSERT(0 == (bslmf::IsConvertible<const int*, int*>::value));
        ASSERT(1 == (bslmf::IsConvertible<int&, const int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const int&, int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int,  int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int,  const int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const int,  int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int&,  const int>::value));
        ASSERT(1 == (bslmf::IsConvertible<const int&, int>::value));
        ASSERT(0 == (bslmf::IsConvertible<const int,  int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<const int,  const int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<const int&, const int>::value));

        // Test volatile value conversions.
        ASSERT(1 == (bslmf::IsConvertible<int, volatile int>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile int, int>::value));

        // Test volatile pointer and reference conversions
        ASSERT(1 == (bslmf::IsConvertible<int*, volatile int*>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int*, int*>::value));
        ASSERT(1 == (bslmf::IsConvertible<int&, volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int&, int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int,  volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int,  int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int&,  volatile int>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile int&,  int>::value));

        ASSERT(1 == (bslmf::IsConvertible<int*, const volatile int*>::value));
        ASSERT(0 == (bslmf::IsConvertible<const volatile int*, int*>::value));
        ASSERT(1 == (bslmf::IsConvertible<int&, const volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const volatile int&, int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int,  const volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const volatile int,  int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int&,  const volatile int>::value));
        ASSERT(1 == (bslmf::IsConvertible<const volatile int&,  int>::value));

        ASSERT(0 == (bslmf::IsConvertible<const int*, volatile int*>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile int*, const int*>::value));
        ASSERT(1 == (bslmf::IsConvertible<const int&, volatile int>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile int&, const int>::value));
        ASSERT(1 == (bslmf::IsConvertible<const int&,
                                          const volatile int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile int&,
                                          const volatile int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<const int,
                                          const volatile int&>::value));
        ASSERT(1 == (bslmf::IsConvertible<volatile int,
                                          const volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const int,
                                          volatile int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const volatile int,
                                          volatile int&>::value));

#if !defined(BSLS_PLATFORM__CMP_SUN)
        // Sun 5.2 and 5.5 both get this wrong when the cv-unqualified types
        // are the same.
        ASSERT(0 == (bslmf::IsConvertible<volatile int,
                                          const int&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const volatile int,
                                          const int&>::value));
#endif

        ASSERT(0 == (bslmf::IsConvertible<int*, int>::value));
        ASSERT(0 == (bslmf::IsConvertible<int, int*>::value));

        ASSERT(1 == (bslmf::IsConvertible<MyAllocator*, MyAllocator*>::value));
        ASSERT(0 == (bslmf::IsConvertible<MyAllocator*, int*>::value));

        ASSERT(1 == (bslmf::IsConvertible<my_Class, my_Class>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_Class, const my_Class>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_Class, const my_Class&>::value));
        ASSERT(0 == (bslmf::IsConvertible<const my_Class, my_Class&>::value));
        ASSERT(1 == (bslmf::IsConvertible<const my_Class,
                                          const my_Class&>::value));
        ASSERT(1 == (bslmf::IsConvertible<const my_Class&,
                                          const my_Class>::value));

        ASSERT(0 == (bslmf::IsConvertible<const my_Class, my_Class&>::value));
        ASSERT(1 == (bslmf::IsConvertible<int, my_Class>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_Class, int>::value));
        ASSERT(0 == (bslmf::IsConvertible<int*, my_Class*>::value));

        ASSERT(1 == (bslmf::IsConvertible<my_OtherClass, my_Class>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_OtherClass,
                                          const my_Class>::value));

        ASSERT(0 == (bslmf::IsConvertible<const my_OtherClass,
                                          const my_Class>::value));

        ASSERT(1 == (bslmf::IsConvertible<my_Class, my_ThirdClass>::value));
        ASSERT(1 == (bslmf::IsConvertible<const my_Class,
                                          my_ThirdClass>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_Class,
                                          const my_ThirdClass>::value));
        ASSERT(1 == (bslmf::IsConvertible<const my_Class,
                                          const my_ThirdClass>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile my_Class,
                                          my_ThirdClass>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_Class&,
                                          const my_ThirdClass>::value));
        ASSERT(1 == (bslmf::IsConvertible<const my_Class&,
                                          const my_ThirdClass>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile my_Class&,
                                          my_ThirdClass>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_Class&,
                                          const my_ThirdClass&>::value));
        ASSERT(1 == (bslmf::IsConvertible<const my_Class&,
                                          const my_ThirdClass&>::value));
        ASSERT(0 == (bslmf::IsConvertible<volatile my_Class&,
                                          my_ThirdClass&>::value));

        ASSERT(0 == (bslmf::IsConvertible<my_OtherClass, int>::value));

        ASSERT(1 == (bslmf::IsConvertible<my_Enum, my_Enum >::value));
        ASSERT(1 == (bslmf::IsConvertible<my_Enum, int     >::value));
        ASSERT(0 == (bslmf::IsConvertible<int, my_Enum     >::value));

        ASSERT(1 == (bslmf::IsConvertible<my_Enum,  my_Class>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_Class,  my_Enum>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_Enum, my_Class*>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_Class, my_Enum*>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_Enum*, my_Class>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_Class*, my_Enum>::value));

        ASSERT(1 == (bslmf::IsConvertible<my_EnumClass::Type,
                                          my_Class>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_Class,
                                          my_EnumClass::Type>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_EnumClass::Type,
                                          my_Class*>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_Class,
                                          my_EnumClass::Type*>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_EnumClass::Type*,
                                          my_Class>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_Class*,
                                          my_EnumClass::Type>::value));

        ASSERT(1 == (bslmf::IsConvertible<my_BslmaAllocator*,
                                          my_StlAllocator<int> >::value));
        ASSERT(1 == (bslmf::IsConvertible<my_BslmaAllocator*,
                                          my_StlAllocator<void> >::value));
        ASSERT(1 == (bslmf::IsConvertible<my_BslmaAllocator*,
                                          my_StlAllocator<void*> >::value));
        ASSERT(1 == (bslmf::IsConvertible<my_BslmaAllocator*,
                                          my_StlAllocator<my_Enum> >::value));
        ASSERT(0 == (bslmf::IsConvertible<void*,
                                          my_StlAllocator<my_Enum> >::value));
        ASSERT(1 == (bslmf::IsConvertible<my_BslmaAllocator*,
                                          my_StlAllocator<my_EnumClass::Type>
                                         >::value));
        ASSERT(0 == (bslmf::IsConvertible<void*,
                                          my_StlAllocator<my_EnumClass::Type>
                                         >::value));

        ASSERT(1 == (bslmf::IsConvertible<void, void>::value));
        ASSERT(0 == (bslmf::IsConvertible<void, int>::value));
        ASSERT(0 == (bslmf::IsConvertible<int, void>::value));

        // An incomplete class can only be tested as the FROM parameter.
        ASSERT(1 == (bslmf::IsConvertible<my_IncompleteClass,
                                          my_IncompleteClass>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_IncompleteClass, int>::value));

        // Test references with incomplete types as both TO and FROM parameters
        ASSERT(1 == (bslmf::IsConvertible<my_IncompleteClass,
                                          my_IncompleteClass&>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_IncompleteClass&, int>::value));
        ASSERT(0 == (bslmf::IsConvertible<int, my_IncompleteClass&>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_IncompleteClass,
                                          my_IncompleteClass2&>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_IncompleteClass&,
                                          my_IncompleteClass2&>::value));

        // Test abstract class.  Mainly just testing that it compiles at all.
        ASSERT(1 == (bslmf::IsConvertible<my_AbstractClass,
                                          my_AbstractClass>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_AbstractClass,
                                          my_AbstractClass&>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_AbstractClass&,
                                          my_AbstractClass>::value));
        ASSERT(1 == (bslmf::IsConvertible<const my_AbstractClass,
                                          my_AbstractClass>::value));
        ASSERT(0 == (bslmf::IsConvertible<int, my_AbstractClass >::value));
        ASSERT(0 == (bslmf::IsConvertible<int, my_AbstractClass&>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_AbstractClass,  int>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_AbstractClass&, int>::value));

        // Test derived-to-base convertibility
        ASSERT(1 == (bslmf::IsConvertible<my_DerivedClass,
                                          my_AbstractClass>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_AbstractClass,
                                          my_DerivedClass>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_DerivedClass&,
                                          my_AbstractClass&>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_AbstractClass&,
                                          my_DerivedClass&>::value));
        ASSERT(1 == (bslmf::IsConvertible<my_DerivedClass*,
                                          my_AbstractClass*>::value));
        ASSERT(0 == (bslmf::IsConvertible<my_AbstractClass*,
                                          my_DerivedClass*>::value));

        // Test pointer-to-member variable convertibility
        ASSERT(1 == (bslmf::IsConvertible<int my_AbstractClass::*,
                                          int my_DerivedClass::*>::value));
        ASSERT(0 == (bslmf::IsConvertible<int my_DerivedClass::*,
                                          int my_AbstractClass::*>::value));
        ASSERT(0 == (bslmf::IsConvertible<int my_AbstractClass::*,
                                          void*>::value));
        ASSERT(0 == (bslmf::IsConvertible<void*,
                                          int my_AbstractClass::*>::value));

        // Test pointer-to-member function convertibility
        ASSERT(1 == (bslmf::IsConvertible<int (my_AbstractClass::*)(int),
                                          int (my_DerivedClass::*)(int)
                                         >::value));
        ASSERT(0 == (bslmf::IsConvertible<int (my_DerivedClass::*)(int),
                                          int (my_AbstractClass::*)(int)
                                         >::value));
        ASSERT(0 == (bslmf::IsConvertible<int (my_AbstractClass::*)(int),
                                          void*>::value));
        ASSERT(0 == (bslmf::IsConvertible<void*,
                                          int (my_AbstractClass::*)(int)
                                         >::value));
      } break;

      default: {
        std::cerr << "WARNING: CASE `" << test << "' NOT FOUND." << std::endl;
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        std::cerr << "Error, non-zero test status = "
                  << testStatus << "." << std::endl;
    }
    return testStatus;
}

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2002
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
