/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef CHRE_CORE_EVENT_LOOP_H_
#define CHRE_CORE_EVENT_LOOP_H_

// TODO: using std lib for initial test... we can't do this in the real world
#include <atomic>

#include "chre/core/event.h"
#include "chre/core/nanoapp.h"
#include "chre/core/timer_pool.h"
#include "chre/util/dynamic_vector.h"
#include "chre/util/fixed_size_blocking_queue.h"
#include "chre/util/non_copyable.h"
#include "chre/util/synchronized_memory_pool.h"

namespace chre {

/**
 * TODO: document this better
 */
class EventLoop : public NonCopyable {
 public:
  /**
   * Setup the event loop.
   */
  EventLoop();

  // TODO: need a clear delineation for which methods are safe to call from
  // threads other than the one calling run()... should include stop() and
  // postEvent()... everything else would decidedly be not thread-safe

  /**
   * Starts a nanoapp by invoking the start entry point. If this is successful,
   * the app is managed by the event loop and the pointer passed in must remain
   * valid.
   *
   * @param A pointer to the nanoapp to start.
   * @return True if the app was started successfully.
   */
  bool startNanoapp(Nanoapp *nanoapp);

  /**
   * Stops a nanoapp by invoking the stop entry point. The nanoapp passed in
   * must have been previously started by the startNanoapp method.
   *
   * @param A pointer to the nanoapp to stop.
   */
  void stopNanoapp(Nanoapp *nanoapp);

  /**
   * Start the main task run loop. Only returns after stop() is called
   * (from another context).
   */
  void run();

  /**
   * Signals the event loop currently executing in run() to exit gracefully at
   * the next available opportunity.
   */
  void stop();

  /**
   * Posts an event to a nanoapp that is currently running (or all nanoapps if
   * the broadcast instance ID is used).
   *
   * @param The type of data being posted.
   * @param The data being posted.
   * @param The callback to invoke when the event is no longer needed.
   * @param The instance ID of the sender of this event.
   * @param The instance ID of the destination of this event.
   *
   * TODO: this is safe to call from other threads, and posts to the incoming
   * event queue, then it is doled out to interested nanoapp queues later
   */
  void postEvent(uint16_t eventType, void *eventData,
                 chreEventCompleteFunction *freeCallback,
                 uint32_t senderInstanceId = kSystemInstanceId,
                 uint32_t targetInstanceId = kBroadcastInstanceId);

  // TODO: static method that allocates + constructs + posts an Event to all
  // EventLoops (i.e. just the one for now), to simplify external code that will
  // call this (they don't need to worry about how events are allocated, etc)

  // TODO: do we need this? it might be a helpful convenience function that
  // handles creation of a timer and stuff
  void postEventDelayed(Event *event, uint64_t delayNs);

  /**
   * Returns a pointer to the currently executing Nanoapp, or nullptr if none is
   * currently executing.
   *
   * @return a pointer to the currently executing nanoapp and nullptr if not
   * currently in an app context.
   */
  Nanoapp *getCurrentNanoapp() const;

  /**
   * Returns a guaranteed unique instance ID that can be used to construct a
   * nanoapp.
   *
   * @return a unique instance ID to assign to a nanoapp.
   */
  uint32_t getNextInstanceId();

  /**
   * Obtains the TimerPool associated with this event loop.
   *
   * @return The timer pool owned by this event loop.
   */
  TimerPool& getTimerPool();

 private:
  //! The maximum number of events that can be active in the system.
  static constexpr size_t kMaxEventCount = 1024;

  //! The maximum number of events that are awaiting to be scheduled. These
  //! events are in a queue to be distributed to apps.
  static constexpr size_t kMaxUnscheduledEventCount = 1024;

  //! The instance ID that was previously generated by getNextInstanceId.
  uint32_t mLastInstanceId = kSystemInstanceId;

  //! The memory pool to allocate incoming events from.
  SynchronizedMemoryPool<Event, kMaxEventCount> mEventPool;

  //! The timer used schedule timed events for tasks running in this event loop.
  TimerPool mTimerPool;

  //! The list of nanoapps managed by this event loop.
  DynamicVector<Nanoapp *> mNanoapps;

  //! The blocking queue of incoming events from the system that have not been
  //!  distributed out to apps yet.
  FixedSizeBlockingQueue<Event *, kMaxUnscheduledEventCount> mEvents;

  // TODO: we probably want our own atomic platform abstraction too
  std::atomic<bool> mRunning{false};

  Nanoapp *mCurrentApp = nullptr;

  void freeEvent(Event *event);
  Nanoapp *lookupAppByInstanceId(uint32_t instanceId);

  // TODO: we probably want a to model kSystemInstanceId as an actual nanoapp
  // with entry points and all that to make it simpler to post events to ourself
  // and the like...
};

}  // namespace chre

#endif  // CHRE_CORE_EVENT_LOOP_H_
