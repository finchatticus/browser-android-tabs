// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/download/internal/service_config_impl.h"

#include "base/time/time.h"
#include "components/download/internal/config.h"

namespace download {

ServiceConfigImpl::ServiceConfigImpl(Configuration* config) : config_(config) {}

ServiceConfigImpl::~ServiceConfigImpl() = default;

uint32_t ServiceConfigImpl::GetMaxScheduledDownloadsPerClient() const {
  return config_->max_scheduled_downloads;
}

const base::TimeDelta& ServiceConfigImpl::GetFileKeepAliveTime() const {
  return config_->file_keep_alive_time;
}

}  // namespace download
