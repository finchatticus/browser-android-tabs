// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/editing/SetSelectionOptions.h"

#include "core/editing/testing/EditingTestBase.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace blink {

class SetSelectionOptionsTest : public EditingTestBase {};

TEST_F(SetSelectionOptionsTest, DefaultValues) {
  SetSelectionOptions options = SetSelectionOptions::Builder().Build();

  EXPECT_EQ(CursorAlignOnScroll::kIfNeeded, options.GetCursorAlignOnScroll());
  EXPECT_FALSE(options.DoNotClearStrategy());
  EXPECT_FALSE(options.DoNotSetFocus());
  EXPECT_EQ(TextGranularity::kCharacter, options.Granularity());
  EXPECT_EQ(SetSelectionBy::kSystem, options.GetSetSelectionBy());
  EXPECT_FALSE(options.ShouldClearTypingStyle());
  EXPECT_FALSE(options.ShouldCloseTyping());
  EXPECT_FALSE(options.ShouldShowHandle());
  EXPECT_FALSE(options.ShouldShrinkNextTap());
}

TEST_F(SetSelectionOptionsTest, Setter) {
  SetSelectionOptions::Builder builder;

  builder.SetCursorAlignOnScroll(CursorAlignOnScroll::kAlways)
      .SetDoNotClearStrategy(true)
      .SetDoNotSetFocus(true)
      .SetGranularity(TextGranularity::kDocumentBoundary)
      .SetSetSelectionBy(SetSelectionBy::kUser)
      .SetShouldClearTypingStyle(true)
      .SetShouldCloseTyping(true)
      .SetShouldShowHandle(true)
      .SetShouldShrinkNextTap(true);

  SetSelectionOptions options = builder.Build();

  EXPECT_EQ(CursorAlignOnScroll::kAlways, options.GetCursorAlignOnScroll());
  EXPECT_TRUE(options.DoNotClearStrategy());
  EXPECT_TRUE(options.DoNotSetFocus());
  EXPECT_EQ(TextGranularity::kDocumentBoundary, options.Granularity());
  EXPECT_EQ(SetSelectionBy::kUser, options.GetSetSelectionBy());
  EXPECT_TRUE(options.ShouldClearTypingStyle());
  EXPECT_TRUE(options.ShouldCloseTyping());
  EXPECT_TRUE(options.ShouldShowHandle());
  EXPECT_TRUE(options.ShouldShrinkNextTap());
}

}  // namespace blink
