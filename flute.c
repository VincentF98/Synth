/* Plugin for violin sound */
#include <math.h>
//#include <stdio.h>
#define PI 3.141592654

short A(double f, double t)
{
	return 500 *		// Modulation
	    (sin(2 * PI * f * t) +	// fundamental tone
	     sin(2 * PI * 2 * f * t) +	// 1st overtone
	     sin(2 * PI * 3 * f * t) * .15 +	// 2nd overtone
	     sin(2 * PI * 4 * f * t) * .21 +	// 3rd overtone
	     sin(2 * PI * 5 * f * t) * .19 +	// 4th overtone
	     sin(2 * PI * 6 * f * t) * .01 +	// 5th overtone
	     sin(2 * PI * 7 * f * t) * .01 +	// 6th overtone
	     sin(2 * PI * 8 * f * t) * .01 +	// 7th overtone
	     sin(2 * PI * 9 * f * t) * .01 +	// 8th overtone
	     sin(2 * PI * 10 * f * t) * .01);	// 9th overtone
}

void gen_data(short *buff, double f, double time, int samplerate)
{
	int frames = (int)(time * (double)samplerate);
	int t;
	//printf("Frequency: %.5g\n",f);
	for (t = 0; t < frames; t++) {
		buff[t] = A(f, (double)t / samplerate);

	}
}

//required to for loading plugin, but is not used here
void init() {

}
