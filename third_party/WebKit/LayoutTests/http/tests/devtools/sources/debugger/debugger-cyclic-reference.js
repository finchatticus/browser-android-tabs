// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

(async function() {
  TestRunner.addResult(
      `Tests that debugging a page where Object prototype has a cyclic reference won't crash the browser.Bug 43558\n`);
  await TestRunner.loadModule('sources_test_runner');
  await TestRunner.showPanel('sources');
  await TestRunner.loadHTML(`
      <p>
      Tests that debugging a page where Object prototype has a cyclic reference won't
      crash the browser.<a href="https://bugs.webkit.org/show_bug.cgi?id=43558">Bug 43558</a>
      </p>
    `);
  await TestRunner.evaluateInPagePromise(`
      Object.prototype.cyclicRef = Object.prototype;

      function testFunction()
      {
          var o = new Object();
          debugger;
      }
  `);

  SourcesTestRunner.startDebuggerTest(step1);

  function step1() {
    SourcesTestRunner.runTestFunctionAndWaitUntilPaused(step2);
  }

  function step2() {
    SourcesTestRunner.completeDebuggerTest();
  }
})();
