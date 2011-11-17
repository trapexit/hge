#include <math.h>
#include "HGE_math.h"

#define NUM_PRIMES (sizeof(prime_numbers)/sizeof(prime_numbers[0]))
static const unsigned int prime_numbers[] =
  { 
    11,19,37,73,109,163,251,367,557,823,1237,1861,
    2777,4177,6247,9371,14057,21089,31627,47431,
    71143,106721,160073,240101,360163,540217,810343,
    1215497,1823231,2734867,4102283,6153409,9230113,
    13845163
  };

unsigned int
HGE_math_nearest_prime(unsigned int num)
{
  int i;

  for(i = 0; i < NUM_PRIMES; i++)
    {
      if(prime_numbers[i] > num)
	return prime_numbers[i];
    }
  
  return prime_numbers[NUM_PRIMES-1];
}

unsigned int
HGE_math_nearest_mult(unsigned int num, unsigned int mult)
{
  int n = mult;

  while(num > n)
    n+=mult;

  return n;
}

unsigned int
HGE_math_nearest_power2(unsigned int num)
{
  num--;
  num |= num >> 1;
  num |= num >> 2;
  num |= num >> 4;
  num |= num >> 8;
  num |= num >> 16;

  return ++num;
}

unsigned int
HGE_math_shift_value(unsigned int num)
{
  unsigned int i;

  i = 0;
  while(num>>=1)
    i++;

  return i;
}
