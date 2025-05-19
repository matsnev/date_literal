#include <compare>

namespace date_literal {

class Month {
public:
  constexpr explicit Month(int month) : month_(month) { Normalize(); }

  // Some syntactic sugar to retrieve the month value.
  constexpr int operator()() const { return month_; }

  // No implicit conversion to int to be able to do arithmetic with ints.
  constexpr explicit operator int() const { return month_; }

  // Next, previous, etc.
  constexpr Month &operator+=(int n) {
    month_ += n;
    Normalize();
    return *this;
  }

  constexpr Month &operator-=(int n) {
    month_ -= n;
    Normalize();
    return *this;
  }

  constexpr Month &operator++() {
    month_++;
    Normalize();
    return *this;
  }

  constexpr Month &operator--() {
    month_--;
    Normalize();
    return *this;
  }

  constexpr Month operator++(int) {
    Month copy = *this;
    ++(*this);
    return copy;
  }

  constexpr Month operator--(int) {
    Month copy = *this;
    --(*this);
    return copy;
  }

  constexpr Month operator+(int n) const { return Month(month_ + n); }
  // For subtracting int from month, use month + (-n) because the pure minus is
  // for composing the dates.

  friend constexpr auto operator<=>(Month lhs, Month rhs) = default;

private:
  constexpr void Normalize() {
    month_ = (month_ - 1) % 12 + 1;
    if (month_ < 1) {
      month_ += 12;
    }
  }

  int month_;
};

constexpr Month Jan = Month(1), January = Jan, jan = Jan, january = Jan;
constexpr Month Feb = Month(2), February = Feb, feb = Feb, february = Feb;
constexpr Month Mar = Month(3), March = Mar, mar = Mar, march = Mar;
constexpr Month Apr = Month(4), April = Apr, apr = Apr, april = Apr;
constexpr Month May = Month(5), may = May;
constexpr Month Jun = Month(6), June = Jun, jun = Jun, june = Jun;
constexpr Month Jul = Month(7), July = Jul, jul = Jul, july = Jul;
constexpr Month Aug = Month(8), August = Aug, aug = Aug, august = Aug;
constexpr Month Sep = Month(9), September = Sep, sep = Sep, september = Sep;
constexpr Month Oct = Month(10), October = Oct, oct = Oct, october = Oct;
constexpr Month Nov = Month(11), November = Nov, nov = Nov, november = Nov;
constexpr Month Dec = Month(12), December = Dec, dec = Dec, december = Dec;

// SlashMonth is an int/Month literal with a slash separator.
// Like 18/Nov doe the day and the month or 2025/Nov for the year and the month.
// It is internal to this library, and we are loose on the exact semantics of
// the int member, presumably if it is small, it is the day, otherwise it is the
// year.
class SlashMonth {

public:
  constexpr SlashMonth(int day_or_year, Month month)
      : month_(month), day_or_year_(day_or_year) {}

  constexpr Month month() const { return month_; }
  constexpr int day_or_year() const { return day_or_year_; }

private:
  Month month_;
  int day_or_year_;
};

inline constexpr SlashMonth operator/(int day_or_year, Month month) {
  return SlashMonth(day_or_year, month);
}

// MonthSlash is a Month/int literal with a slash separator.
class MonthSlash {
public:
  constexpr MonthSlash(Month month, int day_or_year)
      : month_(month), day_or_year_(day_or_year) {}

  constexpr Month month() const { return month_; }

  constexpr int day_or_year() const { return day_or_year_; }

private:
  Month month_;
  int day_or_year_;
};

inline constexpr MonthSlash operator/(Month month, int day_or_year) {
  return MonthSlash(month, day_or_year);
}

// And the same for a hyphenated date pairs.
class HyphenMonth {
public:
  constexpr HyphenMonth(int day_or_year, Month month)
      : month_(month), day_or_year_(day_or_year) {}

  constexpr Month month() const { return month_; }
  constexpr int day_or_year() const { return day_or_year_; }

private:
  Month month_;
  int day_or_year_;
};

inline constexpr HyphenMonth operator-(int day_or_year, Month month) {
  return HyphenMonth(day_or_year, month);
}

class MonthHyphen {
public:
  constexpr MonthHyphen(Month month, int day_or_year)
      : month_(month), day_or_year_(day_or_year) {}

  constexpr Month month() const { return month_; }
  constexpr int day_or_year() const { return day_or_year_; }

private:
  Month month_;
  int day_or_year_;
};

inline constexpr MonthHyphen operator-(Month month, int day_or_year) {
  return MonthHyphen(month, day_or_year);
}

inline constexpr bool IsLeapYear(int year) {
  return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

inline constexpr int kLeapNumbers[] = {31, 29, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31};

inline constexpr int kNonLeapNumbers[] = {31, 28, 31, 30, 31, 30,
                                          31, 31, 30, 31, 30, 31};

inline constexpr int DaysInMonth(Month month, int year) {
  return (IsLeapYear(year) ? kLeapNumbers : kNonLeapNumbers)[month() - 1];
}

class Date {
  // Always contain s a valid date or the sentinel invalid value.
public:
  constexpr Date(int year, Month m, int day)
      : year_(year), month_(m), day_(day) {
    if (!IsValid()) {
      year_ = 0;
      month_ = Month(1);
      day_ = 0;
    }
  }

  constexpr int year() const { return year_; }
  constexpr Month month() const { return month_; }
  constexpr int day() const { return day_; }

  constexpr bool IsValid() const {
    return day_ > 0 && day_ <= DaysInMonth(month_, year_);
  }

  // Standard sentinel value for an invalid date.
  static constexpr Date Invalid() { return Date(0, Month(1), 0); }

  constexpr Date &operator++() {
    if (day_ == DaysInMonth(month_, year_)) {
      day_ = 1;
      if (month_ == Month(12)) {
        month_ = Month(1);
        ++year_;
      } else {
        ++month_;
      }
    } else {
      ++day_;
    }
    return *this;
  }

  constexpr Date &operator--() {
    if (day_ == 1) {
      if (month_ == Month(1)) {
        month_ = Month(12);
        --year_;
      } else {
        --month_;
      }
      day_ = DaysInMonth(month_, year_);
    } else {
      --day_;
    }
    return *this;
  }

  constexpr Date operator++(int) {
    Date copy = *this;
    ++(*this);
    return copy;
  }

  constexpr Date operator--(int) {
    Date copy = *this;
    --(*this);
    return copy;
  }

  friend constexpr auto operator<=>(Date lhs, Date rhs) = default;

private:
  int year_;
  Month month_;
  int day_;
};

// An American version: Nov/11/2025.
inline constexpr Date operator/(MonthSlash month_slash, int year) {
  return Date(year, month_slash.month(), month_slash.day_or_year());
}

// A European version: 11/Nov/2025 or an Asian version: 2025/Nov/11.
inline constexpr Date operator/(SlashMonth slash_month, int day_or_year) {
  // Attempt an Asian version first.
  if (Date attempt(slash_month.day_or_year(), slash_month.month(), day_or_year);
      attempt.IsValid()) {
    return attempt;
  }
  return Date(day_or_year, slash_month.month(), slash_month.day_or_year());
}

// An American version: Nov-11-2025.
inline constexpr Date operator-(MonthHyphen month_hyphen, int year) {
  return Date(year, month_hyphen.month(), month_hyphen.day_or_year());
}

// A European version: 11-Nov-2025 or an Asian version: 2025-Nov-11.
inline constexpr Date operator-(HyphenMonth hyphen_month, int day_or_year) {
  // Attempt an Asian version first.
  if (Date attempt(hyphen_month.day_or_year(), hyphen_month.month(),
                   day_or_year);
      attempt.IsValid()) {
    return attempt;
  }
  return Date(day_or_year, hyphen_month.month(), hyphen_month.day_or_year());
}

} // namespace date_literal
