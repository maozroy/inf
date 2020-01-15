#include <stdlib.h>

size_t no_signal_counter_g = 0;

void SetOffCounter(int nothing)
{
	no_signal_counter_g = 0;
}
