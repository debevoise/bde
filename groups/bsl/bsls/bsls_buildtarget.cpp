// bsls_buildtarget.cpp                                               -*-C++-*-
#include <bsls_buildtarget.h>

#include <bsls_ident.h>
BSLS_IDENT("$Id$ $CSID$")

namespace BloombergLP {

extern const int BSLS_BUILDTARGET_IS_UNIQUE = 1;

#ifdef BDE_BUILD_TARGET_EXC

const int bsls::BuildTargetYesExc::s_isBuildTargetExc = 1;

#else

const int bsls::BuildTargetNoExc::s_isBuildTargetExc = 0;

#endif

#ifdef BDE_BUILD_TARGET_MT

const int bsls::BuildTargetYesMt::s_isBuildTargetMt = 1;

#else

const int bsls::BuildTargetNoMt::s_isBuildTargetMt = 0;

#endif

#ifdef BSLS_PLATFORM_CPU_64_BIT

const int bsls::BuildTargetYes64Bit::s_isBuildTarget64Bit = 1;

#else

const int bsls::BuildTargetNo64Bit::s_isBuildTarget64Bit = 0;

#endif

}  // close enterprise namespace

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2004
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
