#include <iostream>
#include <cstddef>
#include <random>

bool isInside(double x, double y, double r)
{
  double a = x - r;
  double b = y - r;
  return a * a + b * b <= r * r;
}

size_t calc(double r, size_t tests, size_t seed)
{
  size_t count = 0;

  std::default_random_engine engine(seed);
  std::uniform_real_distribution< double > dist(0.0, 2.0 * r);

  for (size_t i = 0; i < tests; ++i)
  {
    double x = dist(engine);
    double y = dist(engine);

    if (isInside(x, y, r))
    {
      ++count;
    }
  }
  return count;
}

int main()
{
  return 0;
}
