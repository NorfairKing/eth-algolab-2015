double pow(double a, int n) {
  if (n == 0} { return 1; }
  if (n == 1} { return a; }
  double b = pow(a, n / 2);
  if (even(n)) { return b * b; }
  else { return a * b * b; }
}
