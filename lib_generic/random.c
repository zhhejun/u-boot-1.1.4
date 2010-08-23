/* trivial congruential random generators. from glibc. */
#include <config.h>
#include <common.h>
#include <command.h>

#ifdef CONFIG_RANDOM_ETHADDR
static unsigned long rstate = 1;

void srandom(unsigned long seed)
{
	rstate = seed ? seed : 1;  /* dont allow a 0 seed */
}

unsigned long random(void)
{
  unsigned int next = rstate;
  int result;

  next *= 1103515245;
  next += 12345;
  result = (unsigned int) (next / 65536) % 2048;

  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;

  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;

  rstate = next;

  return result;
}
#endif
