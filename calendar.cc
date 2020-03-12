#include <fmt/core.h>
#include <date/date.h>
#include <exception>
#include <iostream>
#include <experimental/coroutine>

using fmt::format;

using date::Monday;
using date::Sunday;
using date::jan;
using date::dec;
using date::months;
using date::year_month;
using date::year_month_weekday;
using namespace date::literals;

using std::cout;
using std::experimental::suspend_always;

const char* const month_names[] = {
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"
};

auto month_name(date::month mon) {
  return month_names[unsigned(mon) - 1];
}

class resumable {
 public:
  struct promise_type;

 private:
  using handle = std::experimental::coroutine_handle<promise_type>;
  handle handle_;

 public:
  struct promise_type {
    bool value;

    auto get_return_object() { return handle::from_promise(*this); }
    auto initial_suspend() { return std::experimental::suspend_always(); }
    auto final_suspend() { return std::experimental::suspend_always(); }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
    auto yield_value(bool val) {
      value = val;
      return std::experimental::suspend_always();
    }
  };

  resumable(handle h) : handle_(h) {}

  bool next() {
    handle_.resume();
    return handle_.promise().value;
  }
};

auto print_month(year_month ym) -> resumable {
  // Print the month name centered.
  auto field_width = 3;
  cout << format("{0:^{1}}", month_name(ym.month()), field_width * 7 + 1);
  co_yield false;

  // Print spaces until the first weekday.
  auto wd_index = year_month_weekday(ym/1).weekday().iso_encoding() - 1;
  cout << format("{0:{1}}", "", wd_index * field_width);

  // Print days.
  for (auto day = 1u, end = unsigned((ym/last).day()); day <= end; ++day) {
    cout << format(" {:2}", day);
    if (year_month_weekday(ym/day).weekday() == Sunday) 
      co_yield day == end;
  }
  
  // Print spaces after the last weekday.
  wd_index = year_month_weekday(ym/last).weekday().iso_encoding();
  for (int i = 0; i < 2; ++i, wd_index = 0) {
    cout << format("{0:{1}}", "", (7 - wd_index) * field_width);
    co_yield true;
  }
}

int main() {
  for (auto start = 2020_y/jan; start <= 2020_y/dec; start += months(3)) {
    auto m0 = print_month(start + months(0));
    auto m1 = print_month(start + months(1));
    auto m2 = print_month(start + months(2));
    bool done = false;
    do {
      done = m0.next();
      cout << ' ';
      done &= m1.next();
      cout << ' ';
      done &= m2.next();
      cout << '\n';
    } while (!done);
    cout << "\n";
  }
}
