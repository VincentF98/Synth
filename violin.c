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
	double r = 0;
	int c;
	for (c = 0; c < 20; c++) {
		r += (3000.0 * sin(PI * t / d)) * sin(2 * PI * (c + 1) *
						     (f +
						      0.05 * (sin(12 * PI * t) -
							      0.1)) * t +
						     phases[c]) * (amplitudes[c]);

	}
	r += (double)pinknoise[i % BUFSZ] * 0.05 * (sin(PI * t / d));
	return (short)r;
	/*return (short)(75+300*tanh(20*t)) *                   // Modulation
	   (sin(2 * PI * f * t+phases[0]) +                     // fundamental tone
	   sin(2 * PI * 2 * f * t+phases[1]) * .97 +            // 1st overtone
	   sin(2 * PI * 3 * f * t+phases[2]) * .56 +            // 2nd overtone
	   sin(2 * PI * 4 * f * t+phases[3]) * .33 +            // 3rd overtone
	   sin(2 * PI * 5 * f * t+phases[4]) * .25 +            // 4th overtone
	   sin(2 * PI * 6 * f * t+phases[5]) * .17 +            // 5th overtone
	   sin(2 * PI * 7 * f * t+phases[6]) * .07 +            // 6th overtone
	   sin(2 * PI * 8 * f * t+phases[7]) * .07 +            // 7th overtone
	   sin(2 * PI * 9 * f * t+phases[8]) * .07 +            // 8th overtone
	   sin(2 * PI * 10 * f * t+phases[9]) * .07 +   // 9th overtone
	   sin(2 * PI * 11 * f * t+phases[10]) * .07 +  // 10th overtone
	   sin(2 * PI * 12 * f * t+phases[11]) * .07 +  // 11th overtone
	   sin(2 * PI * 13 * f * t+phases[12]) * .07 +  // 12th overtone
	   sin(2 * PI * 14 * f * t+phases[13]) * .07 +  // 13th overtone
	   sin(2 * PI * 15 * f * t+phases[14]) * .07 +  // 14th overtone
	   sin(2 * PI * 16 * f * t+phases[15]) * .07 +  // 15th overtone
	   sin(2 * PI * 17 * f * t+phases[16]) * .07 +  // 16th overtone
	   sin(2 * PI * 18 * f * t+phases[17]) * .07 +  // 17th overtone
	   sin(2 * PI * 19 * f * t+phases[18]) * .07 +  // 18th overtone
	   sin(2 * PI * 20 * f * t+phases[19]) * .07 +  // 19th overtone
	   sin(2 * PI * 21 * f * t+phases[20]) * .07+   // 20th overtone
	   sin(2 * PI * 22 * f * t+phases[21]) * .07 +  // 21st overtone
	   sin(2 * PI * 23 * f * t+phases[22]) * .07 +  // 22nd overtone
	   sin(2 * PI * 24 * f * t+phases[23]) * .07 +  // 23rd overtone
	   sin(2 * PI * 25 * f * t+phases[24]) * .07 +  // 24th overtone
	   sin(2 * PI * 26 * f * t+phases[25]) * .07 +  // 25th overtone
	   sin(2 * PI * 27 * f * t+phases[26]) * .07 +  // 26th overtone
	   sin(2 * PI * 28 * f * t+phases[27]) * .07 +  // 27th overtone
	   sin(2 * PI * 29 * f * t+phases[28]) * .07 +  // 28th overtone
	   sin(2 * PI * 30 * f * t+phases[29]) * .07 +  // 29th overtone
	   sin(2 * PI * 31 * f * t+phases[30]) * .07 +  // 30th overtone
	   sin(2 * PI * 32 * f * t+phases[31]) * .07 +  // 31st overtone
	   sin(2 * PI * 33 * f * t+phases[32]) * .07 +  // 32nd overtone
	   sin(2 * PI * 34 * f * t+phases[33]) * .07 +  // 33rd overtone
	   sin(2 * PI * 35 * f * t+phases[34]) * .07 +  // 34th overtone
	   sin(2 * PI * 36 * f * t+phases[35]) * .07 +  // 35th overtone
	   sin(2 * PI * 37 * f * t+phases[36]) * .07 +  // 36th overtone
	   sin(2 * PI * 38 * f * t+phases[37]) * .07 +  // 37th overtone
	   sin(2 * PI * 39 * f * t+phases[38]) * .07 +  // 38th overtone
	   sin(2 * PI * 40 * f * t+phases[39]) * .07) + // 39th overtone
	   (double)pinknoise[i%BUFSZ]*0.4*(0.1+tanh(30*t));                     // bow noise */

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
