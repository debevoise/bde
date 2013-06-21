// bdesu_osutil.h                                                     -*-C++-*-
#ifndef INCLUDED_BDESU_OSUTIL
#define INCLUDED_BDESU_OSUTIL

#ifndef INCLUDED_BDES_IDENT
#include <bdes_ident.h>
#endif
BDES_IDENT("$Id: $")

//@PURPOSE: Provide utilities related to the operating system.
//
//@CLASSES:
//  bdesu_OsUtil: namespace for operating system information utilities
//
//@SEE ALSO:
//
//@DESCRIPTION: This component provides a namespace, 'bdesu_OsUtil',
// containing utility functions for retrieving information at runtime about the
// operating system in which this task is running.
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Display OS Name, Version and Patch Level
///- - - - - - - - - - - - - - - - - - - - - - - - - -
// The following example demonstrates using 'getOsInfo' to obtain information
// about the operating system at runtime and writing it to the console.
//
// First, we create strings for the operating system name ('osName'), version
// ('osVersion'), and patch ('osPatch'), and then call 'getOsInfo' to load
// these strings with values for the operating system the task is executing in:
//..
//  bsl::string name;
//  bsl::string version;
//  bsl::string patch;
//..
// Then, we use the standard streams to write the operating system version
// information to the console, or an error message of failure:
//..
//  int rc = bdesu_OsUtil::getOsInfo(&name, &version, &patch);
//  if (0 == rc) {
//      bsl::cout << "OS Name: " << name << "\n"
//                << "Version: " << version << "\n"
//                << "Patch:   " << patch << "\n";
//  } else {
//      bsl::cout << "Cannot determine OS name and version\n";
//  }
//..
// Finally, the resulting console output on the Red Hat Enterprise Linux Server
// 5.5 would be
//..
//  OS Name: Linux
//  Version: 2.6.18-194.32.1.el5
//  Patch:   #1 SMP Mon Dec 20 10:52:42 EST 2010
//..
// On Windows 7 SP1, the display would be
//..
//  OS Name: Windows
//  Version: 6.1
//  Patch:   Service Pack 1
//..

#ifndef INCLUDED_BSL_STRING
#include <bsl_string.h>
#endif

namespace BloombergLP {

                        // ===================
                        // struct bdesu_OsUtil
                        // ===================

struct bdesu_OsUtil {
    // This 'struct' provides a namespace for utility functions retrieving
    // information about the operating system.

    // CLASS METHODS
    static int getOsInfo(bsl::string *osName,
                         bsl::string *osVersion,
                         bsl::string *osPatch);
    // Load the operating system name, version name and patch name into the
    // specified 'osName', 'osVersion' and 'osPatch' respectively. Return 0 on
    // success and a non-zero value otherwise. On MS Windows systems 'osName'
    // will be 'Windows', with 'osVersion' set to 'dwMajorVersion' concatenated
    // with a dot and 'dwMinorVersion', and 'osPatch' will be set to
    // 'szCSDVresion' (Service Pack name), as returned by !GetVersionEx!. On
    // Posix systems, 'osName', 'osVersion' and 'osPatch' will be set to
    // 'sysname', 'release', and 'version', respectively, as returned by
    // !uname(2)!.
};

#ifndef BDE_OMIT_INTERNAL_DEPRECATED
namespace bdesu {
typedef bdesu_OsUtil OsUtil;
}
#endif // BDE_OMIT_INTERNAL_DEPRECATED

}  // close enterprise namespace

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2013
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
