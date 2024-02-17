#pragma once

class Counter {
private:
  long long cnt = 0;
public:
  long long get();
};

long long Counter::get() {
  return cnt++;
}
