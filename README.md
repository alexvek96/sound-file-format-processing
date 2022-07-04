This is a project of 1st/8 semester in NKUA IT Department (Sep 2020 - Feb 2021).
Some introductive info:

1)	Sound is an analog signal and from a mathematical point of view, 
	it can be considered as a function of one size related to time. 
	This size can be the density of the air that excites one's eardrum, 
	or the electrical signal produced by the head of a record player when
	it runs through a groove of a record vinyl, etc. However, to store the 
	sound in a computer, we must first digitize it from it analog nature. 
	This digitization is done by recording the value of the function
	of the signal at distinct equidistant instants of time. 
	The values of the function can then be considered to correspond to the
	intensity of the sound at the corresponding times.
	
2)	Provided that the record of successive values of the function is dense enough,
	even though we no longer have the original analog signal at our disposal, 
	the reproduction of sound from its digitized form can be reasonably faithful 
	to its original analog version.
	
3)	There are various standards for storing digitized sounds in files, such as, for
	example, .wav, .mp3, .aac, .m4a, .wma, etc. The object of this work is the processing
	of .wav files. Before moving on to the requirements of the work, a detailed description of
	the format of the .wav files that the code handles oughts to be presented. 
	The simplest possible version of a .wav file format  will be used in this context of work,
	and it is described below. A wav file contains in order:
	
	
	• 4 bytes with the characters “RIFF” (initials of Resource Interchange File Format, which is
	the standard used to structure .wav files).
	
	• 4 bytes, named “SizeOfFile”, indicating the size of the file, which is actually the
	number of bytes that follow until the end of the file. Essentially, it's the size
	of the file on disk minus 8 bytes, i.e. those occupying the current field and the
	previous (“RIFF”). It is noted that the representation of unsigned integers is by
	order from least significant byte to most significant (little-endian standard).
	That is, the integer 0xA3870FB9 is represented in order by the bytes 0xB9, 0x0F, 0x87, 0xA3.
	
	• 4 bytes with the characters “WAVE”, indicating the specific file subcategory
	which follows the “RIFF” standard.
	
	• 4 bytes with the characters “fmt” (pay attention to the last space character),
	which indicate the start of the format chunk of the file.
	
	• 4 bytes for the space occupied by the data of the format section that will follow
	In the files processed by the current code, the value of this field will be
	always 16 (0x00000010). And here the little-endian standard is applied, i.e. the hex-number
	stored with the sequence of bytes 0x10, 0x00, 0x00, 0x00.
	
	• 2 bytes for the .wav format type. For the present work, this will be always equal to 1 (0x0001)
	and will be stored in little-endian order, i.e. with bytes 0x01, 0x00.
	
	• 2 bytes, lets name them “MonoStereo”, for whether the sound is monophonic (value 0x0001) or
	stereophonic with two channels (value 0x0002). Beware of little-endian, with regard to all
	integer values that follow.
	
	• 4 bytes, “SampleRate”, for the sampling rate, i.e. how many values per second
	of the analog audio function have been recorded in the digital file.
	
	• 4 bytes, “BytesPerSec”, for the number of bytes per second of audio, that have been
	registered into the sound file.
	
	• 2 bytes, “BlockAlign”, for the number of bytes required for the entry
	of the audio information at one time, for all channels together. Note that always
	is [BytesPerSec = SampleRate × BlockAlign].
	
	• 2 bytes, “BitsPerSample”, for the number of bits required for the registring
	of the sound information at one distinct moment in time, for a single channel. Within the current
	project, this value will be either 8 (0x0008), or 16 (0x0010). For BitsPerSample=8, the sound
	volume is an unsigned integer in the interval [0,255], while for BitsPerSample=16, loudness 
	is a signed integer in the interval [-32768,32767].
	Note that [BlockAlign = BitsPerSample/8 × MonoStereo] should always be true.
	
	• 4 bytes with the characters “data”, indicating the start of the data section (data
	chunk) of the file.
	
	• 4 bytes, “SizeOfData”, for the space occupied by the segment data that follows.
	
	• Digitized audio data in chronological order. In the case of stereo sound
	with two channels, for each time moment, there is a pair of data, with the first element
	corresponding to the left channel and the second element corresponding to the right channel.
	
	• After the end of the audio data, there may be extra data info regarding the specific .wav file.
	We will not stick to them.
	
	
4)	Example of data in a .wav file:
	
	$ od -tx1 good.wav
	0000000 52 49 46 46 85 00 00 00 57 41 56 45 66 6d 74 20
	0000020 10 00 00 00 01 00 01 00 44 ac 00 00 88 58 01 00
	0000040 02 00 10 00 64 61 74 61 58 00 00 00 00 00 d4 07
	0000060 a1 0f 5e 17 04 1f 8a 26 ea 2d 1c 35 18 3c d7 42
	0000100 54 49 86 4f 69 55 f6 5a 27 60 f8 64 63 69 64 6d
	0000120 f7 70 18 74 c5 76 fa 78 b6 7a f6 7b b9 7c ff 7c
	0000140 c8 7c 12 7c e0 7a 33 79 0b 77 6c 74 58 71 d1 6d
	0000160 dd 69 7e 65 b8 60 92 5b 0f 56 36 50 0c 4a 97 43
	0000200 df 3c ea 35 45 78 74 72 61 44 61 74 61
	0000215
	$
	
Objectives:

The code must read the data from a .wav file using getchar() and decide if the file represents a
.wav file according to the anticipated format. Results of bad or good format shall be printed
using fprint. Bad format must end the procedure instantly.

- Example of good format:

$ ./wavproc < 5MB.wav
size of file: 5226758
size of format chunck: 16
WAVE type format: 1
mono/stereo: 2
sample rate: 44100
bytes/sec: 176400
block alignment: 4
bits/sample: 16
size of data chunk: 5226496
$

- Example of bad format:

$ ./wavproc < bad_sfc.wav
size of file: 133
size of format chunck: 18
Error! size of format chunck should be 16
$
