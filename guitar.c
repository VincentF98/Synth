/* Plugin for Guitar sound */
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
	r = -0.5502732115e-1 * pow(i, 3) +
	   .8704655056 * pow(i, 2) -2.233186588 * i + 2 * PI;
	return r;
}

short A(double f, int i)
{
	double t = (double)i / samplerate;
	double r = 0;
	int c;
	for (c = 0; c < 20; c++) {
		r += (2500.0 * (sin(PI * t*5 )/(6*t+1)+2)*exp(-4*t)) * sin(2 * PI * (c + 1) *
						     f * t +
						     phases[c]) * amplitudes[c];

	}
	r += (double)pinknoise[i % BUFSZ] * 0.05 * (sin(PI * t / d));
	return (short)r;
	

}

void init()
{
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
