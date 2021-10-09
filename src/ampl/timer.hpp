#ifndef AMPL_TIMER_HPP
#define AMPL_TIMER_HPP

#include <chrono>

namespace ampl {

  using namespace std;
  
  struct Timer {
    using Clock = chrono::steady_clock;
    using Time = Clock::time_point;

    Timer() { start = Clock::now(); }
    
    auto ns() { return chrono::duration_cast<chrono::nanoseconds>(Clock::now() - start).count(); }

    auto us() { return chrono::duration_cast<chrono::microseconds>(Clock::now() - start).count(); }

    auto ms() { return chrono::duration_cast<chrono::milliseconds>(Clock::now() - start).count(); }

    Time start;
  };
    
}

#endif
