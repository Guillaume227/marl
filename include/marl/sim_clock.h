#pragma once

#include <chrono>

namespace aura {

  struct sim_clock {
    using duration = std::chrono::microseconds;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<std::chrono::system_clock>;

    static constexpr bool is_steady = false;

    static sim_clock*& instance_ptr() {
      static sim_clock _instance;
      static thread_local sim_clock* _instance_ptr = &_instance;
      return _instance_ptr;
    }

    static sim_clock& instance() {
      return *instance_ptr();
    }

    static void set_instance(sim_clock& instance) {
      instance_ptr() = &instance;
    }

    static time_point now() noexcept {
      return instance()._time_now;
    }

    template<typename Duration = duration>
    static Duration since_start() noexcept {
      return std::chrono::duration_cast<Duration>(instance()._time_now.time_since_epoch());
    }
    static void reset_clock() {
      instance().reset();
    }

    //////////

    time_point time_now() const noexcept {
      return _time_now;
    }

    template<typename Duration = duration>
    Duration time_since_start() const noexcept {
      return std::chrono::duration_cast<Duration>(_time_now.time_since_epoch());
    }

    template<typename Duration = duration>
    time_point to_time_point(Duration time_since_start) const noexcept {
      return time_point{} + time_since_start;
    }

    duration time_since_start(time_point const& timePoint) const noexcept {
      return std::chrono::duration_cast<duration>(timePoint.time_since_epoch());
    }

    template<typename Duration>
    time_point advance_time(Duration const& d) noexcept {
      return _time_now += d;
    }

    time_point set_time(time_point timePoint) noexcept {
      return _time_now = timePoint;
    }

    void reset() {
      _time_now = {};
    }

  private:
    time_point _time_now = {};
  };
}  // namespace marl
