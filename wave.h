#ifndef WAVE_H
#define WAVE_H
void write_wav(char *filename, unsigned long num_samples, short *data,
	       int s_rate);
#endif
