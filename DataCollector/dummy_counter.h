#pragma once

class Counter {
private:
  long long cnt = 0;
public:
  long long get();
  String get_as_json();
};
