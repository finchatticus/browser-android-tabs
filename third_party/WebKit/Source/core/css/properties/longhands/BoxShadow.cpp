// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/properties/longhands/BoxShadow.h"

#include "core/css/parser/CSSParserContext.h"
#include "core/css/properties/CSSParsingUtils.h"

namespace blink {
namespace CSSLonghand {

const CSSValue* BoxShadow::ParseSingleValue(
    CSSParserTokenRange& range,
    const CSSParserContext& context,
    const CSSParserLocalContext&) const {
  return CSSParsingUtils::ConsumeShadow(
      range, context.Mode(), CSSParsingUtils::AllowInsetAndSpread::kAllow);
}

}  // namespace CSSLonghand
}  // namespace blink
