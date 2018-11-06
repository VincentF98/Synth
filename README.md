# Synth
This is mmy attempt at simulating the sound of various instruments.
## Usage
```
$ ./synth instrument song
```

Output is a file named song-instrument.wav
Available instruments are:
	- violin
	- guitar
	- recorder
	- flute
	- sine

## Song format
Songs are saved in plain text files; each line must contain one note in the following format:
```
Notename Octave Duration
```

"Twinkle twinkle little star" is included as a sample song
