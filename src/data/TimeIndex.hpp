#ifndef TIME_INDEX
#define TIME_INDEX

#include <iostream>

// https://en.wikipedia.org/wiki/Barton%E2%80%93Nackman_trick
// A class template to express an equality comparison interface.
template <typename T> class time_comparable {
  friend bool operator==(T const &a, T const &b) { return a.equal_to(b); }
  friend bool operator!=(T const &a, T const &b) { return !a.equal_to(b); }
  friend bool operator<(T const &a, T const &b) { return a.less_than(b); }
  friend bool operator>(T const &a, T const &b) {
    return !(a.equal_to(b) || a.less_than(b));
  }
};

// // Class value_type wants to have == and !=, so it derives from
// // equal_comparable with itself as argument (which is the CRTP).
// class TimeIndex : private time_comparable<TimeIndex> {
// public:
//   virtual bool equal_to(TimeIndex const &rhs) const = 0;  // to be defined
//   virtual bool less_than(TimeIndex const &rhs) const = 0; // to be defined
// };

#endif