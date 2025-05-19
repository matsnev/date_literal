## Literal Dates
This is a simple C++ library to provide a machinery to use literal dates (like `15-Nov-2025`, `Oct/11/2025`, etc.) as date values in C++.

I wanted to have a tool to be able to write code like

```
if (today == 15-Nov-2025) { /* */ }
```

or

```
for (auto date = 2025/Sep/1; date < 2025/Nov/11; ++date) { /* */ }
```

(see more examples in the tests.)

## Implementation

The crucial point is to have the month as a `Month` class instance, then any date is a combination of the integer date, the month, and the integer year. By overloading
`operator-` and `operator/` for the pair `(Month, int)` and `(int, Month)` we can produce an instance of an intermediate class which can then be paired by the overloaded
operator with another integer. 

Note that `2025-11-15` cannot become anything but an int as we cannot overload `/` or '-' to operate on `int`s, hence we have to spell the month non-numerically.

There is one more nuance though. An expression like `2025-Feb-15` refers to a unique date, same as `Feb-15-2025` or `15-Feb-2025` (similarly for the version with the slashes).

An expression like `10-Feb-15` is tricky, so we have to make a choice what would be a year and what would be the day. Luckily, most our activities happen for the years which are
high enough in value, so I do not see this constraint as very limiting.
