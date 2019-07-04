/*
 * OpenAL Recording Example
 *
 * Copyright (c) 2017 by Chris Robinson <chris.kcat@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* This file contains a relatively simple recorder. */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include <common/alhelpers.h>


#if defined(_WIN64)
#define SZFMT "%I64u"
#elif defined(_WIN32)
#define SZFMT "%u"
#else
#define SZFMT "%zu"
#endif


#if defined(_MSC_VER) && (_MSC_VER < 1900)
static float msvc_strtof(const char *str, char **end)
{ return (float)strtod(str, end); }
#define strtof msvc_strtof
#endif


static void fwrite16le(ALushort val, FILE *f)
{
    ALubyte data[2] = { val&0xff, (val>>8)&0xff };
    fwrite(data, 1, 2, f);
}

static void fwrite32le(ALuint val, FILE *f)
{
    ALubyte data[4] = { val&0xff, (val>>8)&0xff, (val>>16)&0xff, (val>>24)&0xff };
    fwrite(data, 1, 4, f);
}


typedef struct Recorder {
    ALCdevice *mDevice;

    FILE *mFile;
    long mDataSizeOffset;
    ALuint mDataSize;
    float mRecTime;

    int mChannels;
    int mBits;
    int mSampleRate;
    ALuint mFrameSize;
    ALbyte *mBuffer;
    ALsizei mBufferSize;
} Recorder;

int main(int argc, char **argv)
{
    printf("main\n");
    if(InitAL(&argv, &argc) != 0)
        return 1;
ALuint res =  LoadSound(argv[0]);

    return 0;
}
