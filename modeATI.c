/*
 * This file is part of the V95bench distribution (https://github.com/viti95/V95bench).
 * Copyright (c) 2022 Víctor Nieto.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "modeATI.h"
#include "timer.h"

#include <dos.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <conio.h>

#define PREHEAT_LOOPS 25L
#define BENCH_TIME 5000L

unsigned long total_loops_modeATI;
unsigned long timespent_modeATI;

void init_modeATI(void)
{
    static int parms[16] = {0x70, 0x50, 0x58, 0x0a,
                            0x40, 0x06, 0x32, 0x38,
                            0x02, 0x03, 0x06, 0x07,
                            0x00, 0x00, 0x00, 0x00};
    int i;

    /* Set the Graphics Solution to 640 x 200 with 16 colors in
       Color Mode */
    outp(0x3D8, 0x2);

    /* Set extended graphics registers */
    outp(0x3DD, 0x80);

    outp(0x03D8, 0x2);
    /* Program 6845 crt controlller */
    for (i = 0; i < 16; ++i)
    {
        outp(0x3D4, i);
        outp(0x3D5, parms[i]);
    }
    outp(0x3D8, 0x0A);
    outp(0x3D9, 0x30);

    outp(0x3DD, 0x80);
}

void preheat_modeATI(unsigned long total_loops)
{
    unsigned int loops;

#ifdef __386__
    unsigned char *vram;
#else
    unsigned char far *vram;
#endif

    for (loops = 0; loops < total_loops; loops++)
    {

#ifdef __386__
        for (vram = (unsigned char *)0xB0000; vram < (unsigned char *)0xB1F40; vram++)
#else
        for (vram = MK_FP(0xB000, 0); vram < MK_FP(0xB000, 0x1F40); vram++)
#endif
        {
            *(vram) = 0x55;
            *(vram + 0x2000) = 0x55;
            *(vram + 0x4000) = 0x55;
            *(vram + 0x6000) = 0x55;
            *(vram + 0x8000) = 0x55;
            *(vram + 0xA000) = 0x55;
            *(vram + 0xC000) = 0x55;
            *(vram + 0xE000) = 0x55;
        }
    }
}

void bench_modeATI(void)
{
#ifdef __386__
    unsigned char *vram;
#else
    unsigned char far *vram;
#endif

    unsigned int loops;
    unsigned int num_loops = total_loops_modeATI;

    for (loops = 0; loops < num_loops; loops++)
    {

#ifdef __386__
        for (vram = (unsigned char *)0xB0000; vram < (unsigned char *)0xB1F40; vram++)
#else
        for (vram = MK_FP(0xB000, 0); vram < MK_FP(0xB000, 0x1F40); vram++)
#endif
        {
            *(vram) = 0x55;
            *(vram + 0x2000) = 0x55;
            *(vram + 0x4000) = 0x55;
            *(vram + 0x6000) = 0x55;
            *(vram + 0x8000) = 0x55;
            *(vram + 0xA000) = 0x55;
            *(vram + 0xC000) = 0x55;
            *(vram + 0xE000) = 0x55;
        }
    }
}

void execute_bench_modeATI(void)
{
    unsigned long preheat_loops = PREHEAT_LOOPS;

    // SET VIDEO MODE
    init_modeATI();

    // PRE-HEAT
    do
    {
        timespent_modeATI = profile_function_loops(preheat_modeATI, preheat_loops);
        preheat_loops *= 2;
    } while (timespent_modeATI == 0);
    preheat_loops /= 2;
    total_loops_modeATI = preheat_loops * BENCH_TIME / timespent_modeATI;

#ifndef __386__
    // Fix for 16-bit executables
    if (total_loops_modeATI > 65535)
        total_loops_modeATI = 65535;
#endif

    // BENCHMARK
    timespent_modeATI = profile_function(bench_modeATI);
}

void show_results_modeATI(void)
{
    double total_result;

    total_result = ((double)total_loops_modeATI * 62.5 * 1000.0) / ((double)timespent_modeATI);
    printf("ATI 640x200 16c: %.2lf kb/s\n", total_result);
}
