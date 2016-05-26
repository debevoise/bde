// bdlt_fixutilconfiguration.cpp                                      -*-C++-*-
#include <bdlt_fixutilconfiguration.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdlt_fixutilconfiguration_cpp,"$Id$ $CSID$")

#include <bslim_printer.h>

#include <bsl_ostream.h>

namespace BloombergLP {
namespace bdlt {

                        // --------------------------
                        // class FixUtilConfiguration
                        // --------------------------

// CLASS DATA
bsls::AtomicOperations::AtomicTypes::Int
FixUtilConfiguration::s_defaultConfiguration = { 3 };

// MANIPULATORS
void FixUtilConfiguration::setOmitColonInZoneDesignator(bool value)
{
    if (value) {
        d_configurationMask |= k_OMIT_COLON_IN_ZONE_DESIGNATOR_BIT;
    }
    else {
        d_configurationMask &= ~k_OMIT_COLON_IN_ZONE_DESIGNATOR_BIT;
    }
}

void FixUtilConfiguration::setFractionalSecondPrecision(int value)
{
    BSLS_ASSERT(0 <= value);
    BSLS_ASSERT(6 >= value);

    d_configurationMask = (d_configurationMask
                              & (~k_FRACTIONAL_SECOND_PRECISION_MASK)) | value;
}

void FixUtilConfiguration::setUseCommaForDecimalSign(bool value)
{
    if (value) {
        d_configurationMask |= k_USE_COMMA_FOR_DECIMAL_SIGN_BIT;
    }
    else {
        d_configurationMask &= ~k_USE_COMMA_FOR_DECIMAL_SIGN_BIT;
    }
}

void FixUtilConfiguration::setUseZAbbreviationForUtc(bool value)
{
    if (value) {
        d_configurationMask |= k_USE_Z_ABBREVIATION_FOR_UTC_BIT;
    }
    else {
        d_configurationMask &= ~k_USE_Z_ABBREVIATION_FOR_UTC_BIT;
    }
}

// ACCESSORS

                                  // Aspects

bsl::ostream&
FixUtilConfiguration::print(bsl::ostream& stream,
                            int           level,
                            int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("fractionalSecondPrecision",
                                                  fractionalSecondPrecision());
    printer.printAttribute("omitColonInZoneDesignator",
                                                  omitColonInZoneDesignator());
    printer.printAttribute("useCommaForDecimalSign", useCommaForDecimalSign());
    printer.printAttribute("useZAbbreviationForUtc", useZAbbreviationForUtc());
    printer.end();

    return stream;
}

}  // close package namespace

// FREE OPERATORS
bsl::ostream& bdlt::operator<<(bsl::ostream&                   stream,
                               const FixUtilConfiguration& object)
{
    bslim::Printer printer(&stream, 0, -1);
    printer.start();
    printer.printValue(object.fractionalSecondPrecision());
    printer.printValue(object.omitColonInZoneDesignator());
    printer.printValue(object.useCommaForDecimalSign());
    printer.printValue(object.useZAbbreviationForUtc());
    printer.end();

    return stream;
}

}  // close enterprise namespace

// ----------------------------------------------------------------------------
// Copyright 2016 Bloomberg Finance L.P.
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
