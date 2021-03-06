// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/properties/longhands/FontVariantNumeric.h"

#include "core/css/parser/CSSPropertyParserHelpers.h"
#include "core/css/parser/FontVariantNumericParser.h"

namespace blink {
namespace CSSLonghand {

const CSSValue* FontVariantNumeric::ParseSingleValue(
    CSSParserTokenRange& range,
    const CSSParserContext& context,
    const CSSParserLocalContext&) const {
  if (range.Peek().Id() == CSSValueNormal)
    return CSSPropertyParserHelpers::ConsumeIdent(range);

  FontVariantNumericParser numeric_parser;
  do {
    if (numeric_parser.ConsumeNumeric(range) !=
        FontVariantNumericParser::ParseResult::kConsumedValue)
      return nullptr;
  } while (!range.AtEnd());

  return numeric_parser.FinalizeValue();
}

}  // namespace CSSLonghand
}  // namespace blink
