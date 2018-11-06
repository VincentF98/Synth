/* Plugin for sinewave sound */
#include <math.h>
//#include <stdio.h>
#define PI 3.141592654

short A(double f, double t)
{
	return (short)(sin(2 * PI * f * t) * 10000);
}

void init()
{
}

void gen_data(short *buff, double f, double time, int samplerate)
{
	int frames = (int)(time * (double)samplerate);
	int t;

	for (t = 0; t < frames; t++) {
		buff[t] = A(f, (double)t / samplerate);

	}

}
