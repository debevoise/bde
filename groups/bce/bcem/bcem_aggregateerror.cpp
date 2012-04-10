// bcem_aggregateerror.cpp                                            -*-C++-*-

#include <bcem_aggregateerror.h>
#include <bslim_printer.h>

#include <bdes_ident.h>
BDES_IDENT_RCSID(bcem_aggregateerror_cpp,"$Id$ $CSID$")

namespace BloombergLP {

int bcem_AggregateError::fromInt(bcem_AggregateError::Code *result,
                                 int                        number)
{
    switch (number) {
      case BCEM_SUCCESS:
      case BCEM_ERR_UNKNOWN_ERROR:
      case BCEM_ERR_NOT_A_RECORD:
      case BCEM_ERR_NOT_A_SEQUENCE:
      case BCEM_ERR_NOT_A_CHOICE:
      case BCEM_ERR_NOT_AN_ARRAY:
      case BCEM_ERR_BAD_FIELDNAME:
      case BCEM_ERR_BAD_FIELDID:
      case BCEM_ERR_BAD_FIELDINDEX:
      case BCEM_ERR_BAD_ARRAYINDEX:
      case BCEM_ERR_NOT_SELECTED:
      case BCEM_ERR_BAD_CONVERSION:
      case BCEM_ERR_BAD_ENUMVALUE:
      case BCEM_ERR_NON_CONFORMANT:
      case BCEM_ERR_AMBIGUOUS_ANON:
          *result = (bcem_AggregateError::Code)number;
          return 0;                                                   // RETURN
      default:
          return -1;                                                  // RETURN

    }
}

bsl::ostream& bcem_AggregateError::print(bsl::ostream& stream,
                                         int           level,
                                         int           spacesPerLevel) const
{
    if (stream.bad()) {
        return stream;                                                // RETURN
    }

    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("description", d_description.c_str());

    // TBD, erase these two lines when bslim_printer is up-to-date
#warning TEMPORARY BSLIM_PRINTER WORKAROUND
    printer.printAttribute("code", (int)d_code);

    // TBD, uncomment this line and erase the lines above when bslim_printer
    // is up-to-date in this branch
    //printer.printAttribute("code", d_code);

    printer.end();

    return stream;
}

}  // close namespace BloombergLP

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2011
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
