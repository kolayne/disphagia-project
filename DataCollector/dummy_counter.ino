long long Counter::get() {
  return cnt++;
}

String Counter::get_as_json() {
  return String(this->get());
}
