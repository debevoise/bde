// baejsn_parserutil.h                                                -*-C++-*-
#ifndef INCLUDED_BAEJSN_PARSERUTIL
#define INCLUDED_BAEJSN_PARSERUTIL

#ifndef INCLUDED_BDES_IDENT
#include <bdes_ident.h>
#endif
BDES_IDENT("$Id: $")

//@PURPOSE: Provide functions to encode and decode simple types in JSON format.
//
//@CLASSES:
//
//@SEE_ALSO:
//
//@AUTHOR: Raymond Chiu (schiu49)
//
//@DESCRIPTION: This component provides utility functions for encoding and
// decoding of primitive JSON constructs fundamental C++ types.

#ifndef INCLUDED_BDESCM_VERSION
#include <bdescm_version.h>
#endif

#ifndef INCLUDED_BDEPU_ISO8601
#include <bdepu_iso8601.h>
#endif

#ifndef INCLUDED_BSL_STREAMBUF
#include <bsl_streambuf.h>
#endif

#ifndef INCLUDED_BSLS_TYPES
#include <bsls_types.h>
#endif

namespace BloombergLP {


                            // ========================
                            // struct baejsn_ParserUtil
                            // ========================

struct baejsn_ParserUtil
{
    static int skipSpaces(bsl::streambuf *streamBuf);

    static int getInteger(bsl::streambuf      *streamBuf,
                          bsls::Types::Uint64 *value);

    static int getInteger(bsl::streambuf      *streamBuf,
                          bsls::Types::Int64  *value);

    static int getDouble(bsl::streambuf *streamBuf,
                         double         *value);

    //static int getNumber(bsl::streambuf *streamBuf,
    //                     double         *value);
    template <class TYPE>
    static int getNumber(bsl::streambuf *streamBuf,
                         TYPE           *value);

    static int getString(bsl::streambuf *streamBuf, bsl::string *value);

    static int eatToken(bsl::streambuf *streamBuf, const char *token);
        // If the get pointer of specified 'streamBuf' refers to a string that
        // matches the specified 'token', advance the get pointer to the
        // charater just after the matched string, with no effect otherwise.
        // Return 0 if a match is found, and a non-zero value otherwise.

    static int advancePastTokenAndWhitespace(bsl::streambuf *streamBuf,
                                             char            token);
        // TBD

    static int putString(bsl::streambuf *streamBuf, const bsl::string& value);
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

//inline
//int baejsn_ParserUtil::getNumber(bsl::streambuf *streamBuf,
//                                 double         *value)
//{
//    return getDouble(streamBuf, value);
//}

template <class TYPE>
inline
int baejsn_ParserUtil::getNumber(bsl::streambuf *streamBuf,
                                 TYPE           *value)
{
    // TBD: We lose accuraccy for int64.  Does it matter?

    double temp;
    int rc = baejsn_ParserUtil::getDouble(streamBuf, &temp);
    *value = static_cast<TYPE>(temp);
    return rc;
}

}  // close namespace BloombergLP

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2012
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
