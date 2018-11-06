/* Main programm for the synthesizer
 * Usage:
 * synth instrument song [samplerate] */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include "wave.h"
typedef struct {
	int n;
	int o;
	double d;
} note_t;
typedef struct {
	note_t *notes;
	int n;
	double length;
	int bpm;
} melody;
typedef struct {
	short *buffer;
	melody m;
	char *instrument;
	int bufsize;
} synthdata;

void *generate(void *v);
int save_data(synthdata sd);
int samplerate = 96000;
int bytes_per_second;
int initflag = 0;
char *outputfilename;
melody newmelody(int n)
{
	melody m;
	note_t *notes = malloc(sizeof(note_t) * n);
	m.notes = notes;
	m.n = 0;
	m.length = 0;
	return m;
}

void addnote(melody * m, note_t n)
{
	m->notes[m->n] = n;
	m->n++;
	m->length += n.d;

}

note_t newnote(int n, int o, double d)
{
	note_t r;
	r.n = n;
	r.o = o;
	r.d = d;
	return r;
}

synthdata create_data_struct(melody m, char *instrument)
{
	synthdata r;
	r.bufsize = (ceil(m.length)) * 60 * bytes_per_second / m.bpm;
	short *buff = malloc(r.bufsize);
	memset(buff,0,r.bufsize);
	r.buffer = buff;
	r.m = m;
	r.instrument = instrument;

	return r;

}

unsigned long count_lines_of_file(char *file_patch)
{
	FILE *fp = fopen(file_patch, "r");
	unsigned long line_count = 0;

	if (fp == NULL) {
		fclose(fp);
		return 0;
	}
	char *s = malloc(11);
	while (fgets(s, 10, fp))
		line_count++;

	fclose(fp);
	free(s);
	return line_count;
}

int getnote(char *line)
{
	int n;
	switch (line[0]) {
	case 'C':
		n = 1;
		break;
	case 'D':
		n = 3;
		break;
	case 'E':
		n = 5;
		break;
	case 'F':
		n = 6;
		break;
	case 'G':
		n = 8;
		break;
	case 'A':
		n = 10;
		break;
	case 'B':
		n = 12;
		break;
	default:
		return -1;
	}
	switch (line[1]) {
	case '#':
		return n + 1;
	case 'b':
		return n - 1;
	default:
		return n;
	}
}

melody readmelodyold(char *file_name)
{
	melody m = newmelody(count_lines_of_file(file_name));
	FILE *fp = fopen(file_name, "r");
	char *line = malloc(11);
	fgets(line, 10, fp);

	int bpm = 0;
	int i = 0;
	while (line[i] != '\n') {
		bpm = 10 * bpm + line[i++] - '0';
	}
	m.bpm = bpm;
	while (fgets(line, 10, fp)) {
		int n, o, d;
		n = getnote(line);
		i = 1;
		if (line[1] == '#' || line[1] == 'b')
			i = 2;
		o = 0;
		while (line[i] != ';') {
			o = 10 * o + line[i++] - '0';
		}
		i++;
		d = 0;
		while (line[i] != '\n') {
			d = 10 * d + line[i++] - '0';
		}
		addnote(&m, newnote(n, o, d));
		//printf("%d %d %d\n",n,o,d);

	}
	free(line);
	return m;

}
melody readmelody(char *file_name)
{
	melody m = newmelody(count_lines_of_file(file_name));
	FILE *fp = fopen(file_name, "r");
	int bpm = 0;
	fscanf(fp,"%d\n",&bpm);
	m.bpm = bpm;
	char *note=malloc(3);
	note[1]=0;
	note[2]=0;
	int octave;
	double duration;
	while (fscanf(fp,"%[ABCDEFG#bp]%d;%lf\n",note,&octave,&duration)!=EOF) {
		addnote(&m, newnote(getnote(note), octave, duration));
		printf("%s %d %.2g\n",note,octave,duration);

	}
	free(note);
	return m;

}
int main(int argc, char **argv)
{

	char *instrument = argv[1];
	char *song = argv[2];
	outputfilename = malloc(strlen(instrument) + strlen(song) + 7);
	sprintf(outputfilename, "./%s-%s.wav", song, instrument);
	if (argc == 4)
		samplerate = atoi(argv[3]);
	bytes_per_second = sizeof(short) * samplerate;
	melody m = readmelody(song);

	synthdata sd = create_data_struct(m, instrument);
	pthread_t gendata_thread;
	pthread_create(&gendata_thread, NULL, &generate, &sd);
	printf("Synthesizing data...\n");
	pthread_join(gendata_thread, NULL);
	write_wav(outputfilename, sd.bufsize / sizeof(short), sd.buffer, samplerate);
}

void *generate(void *v)
{
	synthdata *sd = (synthdata *) v;
	char *file_name = malloc(strlen(sd->instrument) + 5);
	sprintf(file_name, "./%s.so", sd->instrument);
	void *plugin = dlopen(file_name, RTLD_NOW);
	if (!plugin) 
		printf("\nCannot find file\n");
	typedef void (*gen_f) (short *, double, double, int);
	typedef void (*init_f) ();
	gen_f gen_data;
	init_f init;
	gen_data = dlsym(plugin, "gen_data");
	if (dlerror()) {
		printf("ERROR: Could not load data generation function\n");
		exit(-1);
	}
	init = dlsym(plugin, "init");
	if (dlerror()) {
		printf("ERROR: Could not load init function\n");
		exit(-1);
	}
	init();

	int i;
	int p = 0;
	for (i = 0; i < sd->m.n; i++) {
	if(sd->m.notes[i].n!=-1)
	{
		double f =
		    55.0 * pow(2,
			       1.0 / 12.0 * (sd->m.notes[i].n +
					     12.0 * sd->m.notes[i].o - 22));
		//printf("%.5g\n",f);
		short *b = &(sd->buffer[p]);

		gen_data(b, f, sd->m.notes[i].d * 60 / sd->m.bpm, samplerate);
	}
		p += sd->m.notes[i].d * samplerate * 60 / sd->m.bpm;
		initflag = 1;
	}
	return NULL;
}

int save_data(synthdata sd)
{
	int ok = 0;
	FILE *out = fopen("./data.raw", "wb");

	if (out != NULL) {
		ok = fwrite(sd.buffer, sd.bufsize, 1, out) == 1;
		fclose(out);
	}
	return ok;
}
