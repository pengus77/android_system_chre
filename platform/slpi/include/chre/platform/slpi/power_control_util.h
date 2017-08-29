/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CHRE_PLATFORM_POWER_CONTROL_UTIL_H
#define CHRE_PLATFORM_POWER_CONTROL_UTIL_H

extern "C" {

#include "qurt_island.h"

}  // extern "C"

#include "chre/core/event_loop_manager.h"

namespace chre {

/**
 * @return true if the vote succeeds.
 */
inline bool slpiForceBigImage() {
  return EventLoopManagerSingleton::get()->getEventLoop().
      getPowerControlManager().votePowerMode(SNS_IMG_MODE_BIG);
}

/**
 * @return true if we're currently running in micro-image, aka island mode.
 */
inline bool slpiInUImage() {
  return (qurt_island_get_status() == 1);
}

} // namespace chre

#endif // CHRE_PLATFORM_POWER_CONTROL_UTIL_H