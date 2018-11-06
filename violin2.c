/* Plugin for violin sound */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include <time.h>
#define PI 3.141592654
//#define RAND_MAX 2147483646
//#define rnd(min,max) (((double) rand() / (RAND_MAX+1)) * (max-min+1) + min)
#define BUFSZ 960000

short *pinknoise;
int samplerate;
double d;
double phases[40];
double amplitudes[40];
double phase(int i)
{
	double r;
	r = 0.04408411 * pow(i, 5) -
	    0.53792502 * pow(i, 4) +
	    2.14697315 * pow(i, 3) -
	    2.75812927 * pow(i, 2) - 1.21978153 * i + 2 * PI;
	return r;
}

short A(double f, int i)
{
	double t = (double)i / samplerate;
	return (short)(75 + 3000 * tanh(20 * t)) *	// Modulation
	    (sin(2 * PI * f * t) +	// fundamental tone
	     sin(2 * PI * 2 * f * t) * .97 +	// 1st overtone
	     sin(2 * PI * 3 * f * t) * .56 +	// 2nd overtone
	     sin(2 * PI * 4 * f * t) * .33 +	// 3rd overtone
	     sin(2 * PI * 5 * f * t) * .25 +	// 4th overtone
	     sin(2 * PI * 6 * f * t) * .17 +	// 5th overtone
	     sin(2 * PI * 7 * f * t) * .07 +	// 6th overtone
	     sin(2 * PI * 8 * f * t) * .07 +	// 7th overtone
	     sin(2 * PI * 9 * f * t) * .07 +	// 8th overtone
	     sin(2 * PI * 10 * f * t) * .07 +	// 9th overtone
	     sin(2 * PI * 11 * f * t) * .07 +	// 10th overtone
	     (double)pinknoise[i % BUFSZ] * 0.0004 * (0.1 + tanh(30 * t)));	// bow noise*/

}

void init()
{
	/*int i,j;
	   for(i=0;i<BUFSZ;i++)
	   whitenoise[i]=rnd(-300,300);
	   for(i=0;i<BUFSZ-1;i++)
	   {
	   pinknoise[i]=0;
	   for(j=0;j<BUFSZ-1;j++)
	   {
	   pinknoise[i]+=whitenoise[j]*exp(-abs(j-i)/100);
	   }
	   } */

	FILE *pn = fopen("./pinknoise.raw", "rb");
	pinknoise = malloc(BUFSZ * sizeof(short));
	fread(pinknoise, BUFSZ, sizeof(short), pn);
	fclose(pn);
	int i;
	for (i = 0; i < 40; i++) {
		phases[i] = phase(i);
		amplitudes[i] = 2 * pow(i + 1, -1.5) + 0.02;
	}
}

void gen_data(short *buff, double f, double time, int sr)
{
	samplerate = sr;
	d = time;
	int frames = (int)(time * (double)samplerate);
	int t;
	//printf("Frequency: %.5g\n",f);

	for (t = 0; t < frames; t++) {
		buff[t] = A(f, t);

	}
}
