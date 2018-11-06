/* Plugin for recorder sound */
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
	return (short)(75 + 300 * tanh(20 * t)) *	// Modulation
	    (sin(2 * PI * f * t) * 13.69 +	// fundamental tone
	     sin(2 * PI * 2 * f * t + 1.86396092) * 5.722 +	// 1st overtone
	     sin(2 * PI * 3 * f * t + 0.14129215) * 9.0712 +	// 2nd overtone
	     sin(2 * PI * 4 * f * t + 2.01) * 0.45 +	// 3rd overtone
	     sin(2 * PI * 50 * t + 2.7) * 0.31 + sin(2 * PI * 100 * t + 2.7) * 0.31);	/* +
											   (double)pinknoise[i%BUFSZ]*0.1*(0.1+tanh(30*t));                   // bow noise */

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
	//srand(time(NULL));

	FILE *pn = fopen("./pinknoise.raw", "rb");
	pinknoise = malloc(BUFSZ * sizeof(short));
	fread(pinknoise, BUFSZ, sizeof(short), pn);
	fclose(pn);
	/*int i;
	   for(i=0;i<40;i++)
	   {
	   phases[i]=phase(i);
	   amplitudes[i]=2*pow(i+1,-1.5)+0.02;
	   } */
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
