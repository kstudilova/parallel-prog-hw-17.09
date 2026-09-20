#include <iostream>
#include <cstddef>
#include <random>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <pthread.h>

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

struct Thread
{
  double r;
  size_t tests;
  size_t seed;
};

void* threadFunc(void* data)
{
  Thread* thread = static_cast< Thread* >(data);

  size_t res = calc(thread->r, thread->tests, thread->seed);

  return reinterpret_cast< void* >(res);
}

double area(double r, size_t threads, size_t tests)
{
  if (r <= 0.0)
  {
    throw std::invalid_argument("Circle radius must be positive");
  }

  if (threads == 0)
  {
    throw std::invalid_argument("Thread count must be positive");
  }

  if (tests == 0)
  {
    throw std::invalid_argument("Test count must be positive");
  }

  size_t partSize = tests / threads;

  std::vector< pthread_t > th(threads);
  std::vector< Thread > threadData(threads);

  for (size_t i = 0; i < threads; ++i)
  {
    threadData[i].seed = i + 1;
    threadData[i].r = r;
    threadData[i].tests = (i == threads - 1) ? partSize + (tests % threads) : partSize;

    int err = pthread_create(&th[i], nullptr, threadFunc, &threadData[i]);
    if (err)
    {
      std::cerr << strerror(err) << '\n';
    }
  }

  size_t total = 0;

  for (size_t i = 0; i < threads; ++i)
  {
    size_t result = 0;

    int err = pthread_join(th[i], reinterpret_cast< void** >(&result));
    if (err)
    {
      std::cerr << strerror(err) << '\n';
    }

    total += result;
  }

  return (4.0 * r * r * total) / tests;
}

int main()
{
  return 0;
}
