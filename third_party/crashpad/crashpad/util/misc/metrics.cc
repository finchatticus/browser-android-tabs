// Copyright 2016 The Crashpad Authors. All rights reserved.
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

#include "util/misc/metrics.h"

#include "base/metrics/histogram_macros.h"
#include "base/metrics/sparse_histogram.h"
#include "build/build_config.h"

#if defined(OS_MACOSX)
#define METRICS_OS_NAME "Mac"
#elif defined(OS_WIN)
#define METRICS_OS_NAME "Win"
#elif defined(OS_ANDROID)
#define METRICS_OS_NAME "Android"
#elif defined(OS_LINUX)
#define METRICS_OS_NAME "Linux"
#elif defined(OS_FUCHSIA)
#define METRICS_OS_NAME "Fuchsia"
#endif

namespace crashpad {

namespace {

//! \brief Metrics values used to track the start and completion of a crash
//!     handling. These are used as metrics values directly, so
//!     enumeration values so new values should always be added at the end.
enum class ExceptionProcessingState {
  //! \brief Logged when exception processing is started.
  kStarted = 0,

  //! \brief Logged when exception processing completes.
  kFinished = 1,
};

void ExceptionProcessing(ExceptionProcessingState state) {
  UMA_HISTOGRAM_COUNTS("Crashpad.ExceptionEncountered",
                       static_cast<int32_t>(state));
}

}  // namespace

// static
void Metrics::CrashReportPending(PendingReportReason reason) {
  UMA_HISTOGRAM_ENUMERATION(
      "Crashpad.CrashReportPending",
      static_cast<int32_t>(reason),
      static_cast<int32_t>(PendingReportReason::kMaxValue));
}

// static
void Metrics::CrashReportSize(FileHandle file) {
  const FileOffset size = LoggingFileSizeByHandle(file);
  UMA_HISTOGRAM_CUSTOM_COUNTS(
      "Crashpad.CrashReportSize", size, 0, 20 * 1024 * 1024, 50);
}

// static
void Metrics::CrashUploadAttempted(bool successful) {
  UMA_HISTOGRAM_COUNTS("Crashpad.CrashUpload.AttemptSuccessful",
                       static_cast<int32_t>(successful));
}

// static
void Metrics::CrashUploadSkipped(CrashSkippedReason reason) {
  UMA_HISTOGRAM_ENUMERATION(
      "Crashpad.CrashUpload.Skipped",
      static_cast<int32_t>(reason),
      static_cast<int32_t>(CrashSkippedReason::kMaxValue));
}

// static
void Metrics::ExceptionCaptureResult(CaptureResult result) {
  ExceptionProcessing(ExceptionProcessingState::kFinished);
  UMA_HISTOGRAM_ENUMERATION("Crashpad.ExceptionCaptureResult",
                            static_cast<int32_t>(result),
                            static_cast<int32_t>(CaptureResult::kMaxValue));
}

// static
void Metrics::ExceptionCode(uint32_t exception_code) {
  UMA_HISTOGRAM_SPARSE_SLOWLY("Crashpad.ExceptionCode." METRICS_OS_NAME,
                              static_cast<int32_t>(exception_code));
}

// static
void Metrics::ExceptionEncountered() {
  ExceptionProcessing(ExceptionProcessingState::kStarted);
}

// static
void Metrics::HandlerLifetimeMilestone(LifetimeMilestone milestone) {
  UMA_HISTOGRAM_ENUMERATION("Crashpad.HandlerLifetimeMilestone",
                            static_cast<int32_t>(milestone),
                            static_cast<int32_t>(LifetimeMilestone::kMaxValue));
}

// static
void Metrics::HandlerCrashed(uint32_t exception_code) {
  UMA_HISTOGRAM_SPARSE_SLOWLY(
      "Crashpad.HandlerCrash.ExceptionCode." METRICS_OS_NAME,
      static_cast<int32_t>(exception_code));
}

}  // namespace crashpad
