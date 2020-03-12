#include <fmt/core.h>
#include <date/date.h>
#include <exception>
#include <iostream>
#include <experimental/coroutine>

using fmt::format;

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

 //private:
  using handle = std::experimental::coroutine_handle<promise_type>;
  handle handle_;

 public:
  struct promise_type {
    auto get_return_object() { return handle::from_promise(*this); }
    auto initial_suspend() { return std::experimental::suspend_always(); }
    auto final_suspend() { return std::experimental::suspend_always(); }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}
  };

  resumable(handle h) : handle_(h) {}
  bool resume();
};

auto print_month(year_month ym) -> resumable {
  // Print the month name centered.
  auto field_width = 3;
  cout << format("{0:^{1}}", month_name(ym.month()), field_width * 7 + 1);
  co_await suspend_always();

  // Print spaces until the first weekday.
  auto offset = year_month_weekday(ym/1).weekday().iso_encoding() - 1;
  cout << format("{0:{1}}", "", offset * field_width);

  // Print days.
  for (auto day = 1u; day <= unsigned((ym/last).day()); ++day) {
    cout << format(" {:2}", day);
    if (year_month_weekday(ym/day).weekday() == Sunday)
      co_await suspend_always();
  }
  auto wd = year_month_weekday(ym/last).weekday();
  if (wd != Sunday) {
    cout << format("{0:{1}}", "", (7 - wd.iso_encoding()) * field_width);
    co_await suspend_always();
  }
}

int main() {
  for (auto start = 2020_y/jan; start <= 2020_y/dec; start += months(3)) {
    auto m0 = print_month(start + months(0));
    auto m1 = print_month(start + months(1));
    auto m2 = print_month(start + months(2));
    while (!m0.handle_.done() && !m1.handle_.done() && !m2.handle_.done()) {
      m0.handle_.resume();
      cout << " ";
      m1.handle_.resume();
      cout << " ";
      m2.handle_.resume();
      cout << "\n";
      // TODO: fix formatting of the last week and reduce copy-pasta
    }
    cout << "\n";
  }
}
