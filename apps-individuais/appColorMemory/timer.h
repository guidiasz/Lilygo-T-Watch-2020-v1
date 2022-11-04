/*https://gist.github.com/zekroTJA/00317b41aa69f38090071b6c8065272b*/
/*
    COPYRIGHT © 2018 Ringo Hoffmann (zekro Development)
    READ BEFORE USING: https://zekro.de/policy
*/

#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

/**
 *  Create asynchronous timers which execute specified
 *  functions in set time interval.
 *
 *  @param func		Function which sould be executed
 *  @param interval	Interval of time in which function will be executed
 *					(in milliseconds)
 */
class Timer {
 public:
  Timer() {}

  Timer(std::function<void(void)> func, const long &interval) {
    m_func = func;
    m_interval = interval;
  }

  /**
   * Starting the timer.
   */
  void start() {
    m_running = true;
    m_thread = std::thread([&]() {
      while (m_running) {
        auto delta = std::chrono::steady_clock::now() + std::chrono::milliseconds(m_interval);
        m_func();
        std::this_thread::sleep_until(delta);
      }
    });
    m_thread.detach();
  }

  /*
   *  Stopping the timer and destroys the thread.
   */
  void stop() {
    m_running = false;
    m_thread.~thread();
  }

  /*
   *  Restarts the timer. Needed if you set a new
   *  timer interval for example.
   */
  void restart() {
    stop();
    start();
  }

  /*
   *  Check if timer is running.
   *
   *  @returns boolean is running
   */
  bool isRunning() {
    return m_running;
  }

  /*
   *  Set the method of the timer after
   *  initializing the timer instance.
   *
   *  @returns boolean is running
   *  @return  Timer reference of this
   */
  Timer *setFunc(std::function<void(void)> func) {
    m_func = func;
    return this;
  }

  /*
   *  Returns the current set interval in milliseconds.
   *
   *  @returns long interval
   */
  long getInterval() {
    return m_interval;
  }

  /*
   *  Set a new interval for the timer in milliseconds.
   *  This change will be valid only after restarting
   *  the timer.
   *
   *  @param interval new interval
   *  @return  Timer reference of this
   */
  Timer *setInterval(const long &interval) {
    m_interval = interval;
    return this;
  }

  ~Timer() {
    stop();
  }

 private:
  // Function to be executed fater interval
  std::function<void(void)> m_func;
  // Timer interval in milliseconds
  long m_interval;

  // Thread timer is running into
  std::thread m_thread;
  // Status if timer is running
  bool m_running = false;
};