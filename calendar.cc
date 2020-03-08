#include <fmt/core.h>
#include <date/date.h>
#include <exception>
#include <iostream>
#include <experimental/coroutine>

using fmt::format;

using date::Sunday;
using date::jan;
using date::days;
using date::dec;
using date::months;
using date::year_month;
using date::year_month_weekday;
using namespace date::literals;

using std::cout;

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

class resumable {
 public:
  struct promise_type;

 private:
  using handle = std::experimental::coroutine_handle<promise_type>;
  handle handle_;

 public:
  struct promise_type {
    auto get_return_object() { return handle::from_promise(*this); }
    auto initial_suspend() { return std::experimental::suspend_always(); }
    auto final_suspend() { return std::experimental::suspend_always(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };

  resumable(handle h) : handle_(h) {}
  bool resume();
};

resumable foo(){
  std::cout << "Hello" << std::endl;
  co_await std::experimental::suspend_always();
  std::cout << "Coroutine" << std::endl;
}

void print_month(year_month ym) {
  // Print the month name centered.
  auto day_field_width = 2;
  cout << format("{:^20}\n",  month_names[unsigned(ym.month()) - 1]);

  // Print spaces until the first weekday.
  auto weekday_index = year_month_weekday(ym/1).weekday().iso_encoding() - 1;
  cout << format("{0:{1}}", "", weekday_index * (day_field_width + 1));

  // Print days.
  auto last_day = unsigned((ym/last).day());
  for (auto day = 1u; day <= last_day; ++day) {
    auto weekday = year_month_weekday(ym/day).weekday();
    cout << format("{:2}{}", day, weekday != Sunday ? ' ' : '\n');
  }
  if (year_month_weekday(ym/last).weekday() != Sunday) cout << '\n';
}

int main() {
  for (auto start = 2020_y/jan; start <= 2020_y/dec; start += months(3)) {
    for (int i = 0; i < 3; ++i) {
      auto date = date::year_month_weekday((start + months(i))/1);
      print_month(start + months(i));
      cout << "\n";
    }
    // TODO: format 3 months side by side
  }
}
