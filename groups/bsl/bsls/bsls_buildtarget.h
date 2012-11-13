// bsls_buildtarget.h                                                 -*-C++-*-
#ifndef INCLUDED_BSLS_BUILDTARGET
#define INCLUDED_BSLS_BUILDTARGET

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide build-target information in the object file.
//
//@CLASSES:
//  bsls::BuildTargetExc: type name for identifying exception builds
//  bsls::BuildTargetMt: type name for identifying multi-threaded builds
//
//@MACROS:
//  BDE_BUILD_TARGET_EXC: flag for exception-enabled builds
//  BDE_BUILD_TARGET_MT:  flag for multi-threaded builds
//
//@AUTHOR: Pablo Halpern (phalpern)
//
//@DESCRIPTION: The purpose of this component is to cause a link-time error
// when trying to link an executable with incompatible libraries.  This
// component defines type names that indicate the build target parameters.
// These parameters include whether this build was exception-enabled (which is
// the case unless overridden by defining the 'BDE_BUILD_TARGET_NO_EXC' macro),
// and whether it was multi-threaded or not (it is enabled unless overridden by
// defining the 'BDE_BUILD_TARGET_NO_MT' macro).  The types defined by this
// component should not be used directly.

#ifndef INCLUDED_BSLS_PLATFORM
#include <bsls_platform.h>
#endif

namespace BloombergLP {

// Default to an exception-enabled build unless 'BDE_BUILD_TARGET_NO_EXC' is
// defined.
#ifndef BDE_BUILD_TARGET_NO_EXC

#ifndef BDE_BUILD_TARGET_EXC
#define BDE_BUILD_TARGET_EXC
#endif

namespace bsls {

struct BuildTargetYesExc {
    static const int s_isBuildTargetExc;
};
typedef BuildTargetYesExc BuildTargetExc;

}  // close package namespace

#ifndef BDE_OMIT_TRANSITIONAL  // BACKWARD_COMPATIBILITY
typedef bsls::BuildTargetYesExc bsls_YesExcBuildTarget;
    // This alias is defined for backward compatibility.
#endif  // BDE_OMIT_TRANSITIONAL -- BACKWARD_COMPATIBILITY

#else

#ifdef BDE_BUILD_TARGET_EXC
#error Do not define both BDE_BUILD_TARGET_EXC and BDE_BUILD_TARGET_NO_EXC
#endif

namespace bsls {

struct BuildTargetNoExc {
    static const int s_isBuildTargetExc;
};
typedef BuildTargetNoExc BuildTargetExc;

}  // close package namespace

#ifndef BDE_OMIT_TRANSITIONAL  // BACKWARD_COMPATIBILITY
typedef bsls::BuildTargetNoExc bsls_NoExcBuildTarget;
    // This alias is defined for backward compatibility.
#endif  // BDE_OMIT_TRANSITIONAL -- BACKWARD_COMPATIBILITY

#endif

// Default to a threaded (MT) build unless 'BDE_BUILD_TARGET_NO_MT' is defined.
#ifndef BDE_BUILD_TARGET_NO_MT

#ifndef BDE_BUILD_TARGET_MT
#define BDE_BUILD_TARGET_MT
#endif

namespace bsls {

struct BuildTargetYesMt {
    static const int s_isBuildTargetMt;
};
typedef BuildTargetYesMt BuildTargetMt;

}  // close package namespace

#ifndef BDE_OMIT_TRANSITIONAL  // BACKWARD_COMPATIBILITY
typedef bsls::BuildTargetYesMt bsls_YesMtBuildTarget;
    // This alias is defined for backward compatibility.
#endif  // BDE_OMIT_TRANSITIONAL -- BACKWARD_COMPATIBILITY

#else

#ifdef BDE_BUILD_TARGET_MT
#error Do not define both BDE_BUILD_TARGET_MT and BDE_BUILD_TARGET_NO_MT
#endif

namespace bsls {

struct BuildTargetNoMt {
    static const int s_isBuildTargetMt;
};
typedef BuildTargetNoMt BuildTargetMt;

}  // close package namespace

#ifndef BDE_OMIT_TRANSITIONAL  // BACKWARD_COMPATIBILITY
typedef bsls::BuildTargetNoMt bsls_NoMtBuildTarget;
    // This alias is defined for backward compatibility.
#endif  // BDE_OMIT_TRANSITIONAL -- BACKWARD_COMPATIBILITY

#endif

#ifdef BSLS_PLATFORM_CPU_64_BIT

#ifdef BSLS_PLATFORM_CPU_32_BIT
#error Do not define both BSLS_PLATFORM_CPU_64_BIT and BSLS_PLATFORM_CPU_32_BIT
#endif

namespace bsls {

struct BuildTargetYes64Bit {
    static const int s_isBuildTarget64Bit;
};
typedef BuildTargetYes64Bit BuildTarget64Bit;

}  // close package namespace

#ifndef BDE_OMIT_TRANSITIONAL  // BACKWARD_COMPATIBILITY
typedef bsls::BuildTargetYes64Bit bsls_Yes64BitBuildTarget;
    // This alias is defined for backward compatibility.
#endif  // BDE_OMIT_TRANSITIONAL -- BACKWARD_COMPATIBILITY

#else

namespace bsls {

struct BuildTargetNo64Bit {
    static const int s_isBuildTarget64Bit;
};
typedef BuildTargetNo64Bit BuildTarget64Bit;

}  // close package namespace

#ifndef BDE_OMIT_TRANSITIONAL  // BACKWARD_COMPATIBILITY
typedef bsls::BuildTargetNo64Bit bsls_No64BitBuildTarget;
    // This alias is defined for backward compatibility.
#endif  // BDE_OMIT_TRANSITIONAL -- BACKWARD_COMPATIBILITY

#endif

// Force linker to pull in this component's object file.

#if defined(BSLS_PLATFORM_CMP_IBM)
static const int *bsls_buildtarget_assertion1 =
                                     &bsls::BuildTargetExc::s_isBuildTargetExc;
static const int *bsls_buildtarget_assertion2 =
                                       &bsls::BuildTargetMt::s_isBuildTargetMt;
static const int *bsls_buildtarget_assertion3 =
                                 &bsls::BuildTarget64Bit::s_isBuildTarget64Bit;
#else
namespace {
    extern const int *const bsls_buildtarget_assertion1 =
                                     &bsls::BuildTargetExc::s_isBuildTargetExc;
    extern const int *const bsls_buildtarget_assertion2 =
                                       &bsls::BuildTargetMt::s_isBuildTargetMt;
    extern const int *const bsls_buildtarget_assertion3 =
                                 &bsls::BuildTarget64Bit::s_isBuildTarget64Bit;
}
#endif

#ifndef BDE_OMIT_TRANSITIONAL  // BACKWARD_COMPATIBILITY
// ===========================================================================
//                           BACKWARD COMPATIBILITY
// ===========================================================================

#ifdef bsls_ExcBuildTarget
#undef bsls_ExcBuildTarget
#endif
#define bsls_ExcBuildTarget bsls::BuildTargetExc
    // This alias is defined for backward compatibility.

#ifdef bsls_MtBuildTarget
#undef bsls_MtBuildTarget
#endif
#define bsls_MtBuildTarget bsls::BuildTargetMt
    // This alias is defined for backward compatibility.

#ifdef bsls_64BitBuildTarget
#undef bsls_64BitBuildTarget
#endif
#define bsls_64BitBuildTarget bsls::BuildTarget64Bit
    // This alias is defined for backward compatibility.

#endif  // BDE_OMIT_TRANSITIONAL -- BACKWARD_COMPATIBILITY

}  // close enterprise namespace

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2012
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
