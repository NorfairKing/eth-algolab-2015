double pow(double a, int n) {
  double res = 1;
  while (n > 0) {
    if (odd(n)) { result *= a; }
    result *= a;
    n /= 2;
  }
  return result;
}
