#include "stdafx.h"



//int tns, thh, tmm, tss;
//tns = (pFormatCtx->duration) / 1000000;
//thh = tns / 3600;
//tmm = (tns % 3600) / 60;
//tss = (tns % 60);
//timelong.Format("%02d:%02d:%02d", thh, tmm, tss);

//常用格式转换
//MP3 - WAV

//ffmpeg - i input.mp3 - acodec pcm_s16le - ac 2 - ar 44100 output.wav

//M4A - WAV

//ffmpeg - iinput.m4a - acodec pcm_s16le - ac 2 - ar 44100 output.wav

//WAV - PCM
//ffmpeg -iinput.wav -f s16le -ar 44100 -acodec pcm_s16le output.raw

//PCM - WAV
//ffmpeg - f s16le - ar 44100 - ac 2 - acodec pcm_s16le - i input.raw output.wav

//FFPlay 播放PCM

//ffplay -f s16le -ar 44100 -ac 2 *.pcm