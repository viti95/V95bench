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

#include "modeY.h"
#include "timer.h"

#include <dos.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <conio.h>

#define SC_INDEX 0x3C4
#define SC_DATA 0x3C5
#define SC_MEMMODE 4

#define CRTC_INDEX 0x3D4
#define CRTC_DATA 0x3D5
#define CRTC_UNDERLINE 20
#define CRTC_MODE 23

#define GC_INDEX 0x3CE
#define GC_DATA 0x3CF
#define GC_READMAP 4
#define GC_MODE 5
#define GC_MISCELLANEOUS 6
#define GC_BITMASK 8

#define PREHEAT_LOOPS 25
#define BENCH_TIME 5000

unsigned int total_loops_modeY;
unsigned long timespent_modeY;

void init_modeY(void)
{
    union REGS regs;

    // Set video mode 13
    regs.w.ax = 0x13;
    int386(0x10, (union REGS *)&regs, &regs);

    outp(SC_INDEX, SC_MEMMODE);
    outp(SC_DATA, (inp(SC_DATA) & ~8) | 4);
    outp(GC_INDEX, GC_MODE);
    outp(GC_DATA, inp(GC_DATA) & ~0x13);
    outp(GC_INDEX, GC_MISCELLANEOUS);
    outp(GC_DATA, inp(GC_DATA) & ~2);
    outpw(SC_INDEX, 0xf02);
    
    outp(CRTC_INDEX, CRTC_UNDERLINE);
    outp(CRTC_DATA, inp(CRTC_DATA) & ~0x40);
    outp(CRTC_INDEX, CRTC_MODE);
    outp(CRTC_DATA, inp(CRTC_DATA) | 0x40);
    outp(GC_INDEX, GC_READMAP);
}

void preheat_modeY(void)
{
    unsigned int loops;
    unsigned char *vram;

    for (loops = 0; loops < PREHEAT_LOOPS; loops++)
    {
        // PLANE 0
        outp(SC_DATA, 1 << (0 & 3));

        for (vram = (unsigned char *)0xA0000; vram < (unsigned char *)0xA3E80; vram += 8)
        {
            *(vram) = 0x46;
            *(vram + 1) = 0x46;
            *(vram + 2) = 0x46;
            *(vram + 3) = 0x46;
            *(vram + 4) = 0x46;
            *(vram + 5) = 0x46;
            *(vram + 6) = 0x46;
            *(vram + 7) = 0x46;
        }

        // PLANE 1
        outp(SC_DATA, 1 << (1 & 3));

        for (vram = (unsigned char *)0xA0000; vram < (unsigned char *)0xA3E80; vram += 8)
        {
            *(vram) = 0x03;
            *(vram + 1) = 0x03;
            *(vram + 2) = 0x03;
            *(vram + 3) = 0x03;
            *(vram + 4) = 0x03;
            *(vram + 5) = 0x03;
            *(vram + 6) = 0x03;
            *(vram + 7) = 0x03;
        }

        // PLANE 2
        outp(SC_DATA, 1 << (2 & 3));

        for (vram = (unsigned char *)0xA0000; vram < (unsigned char *)0xA3E80; vram += 8)
        {
            *(vram) = 0xCC;
            *(vram + 1) = 0xCC;
            *(vram + 2) = 0xCC;
            *(vram + 3) = 0xCC;
            *(vram + 4) = 0xCC;
            *(vram + 5) = 0xCC;
            *(vram + 6) = 0xCC;
            *(vram + 7) = 0xCC;
        }

        // PLANE 3
        outp(SC_DATA, 1 << (3 & 3));

        for (vram = (unsigned char *)0xA0000; vram < (unsigned char *)0xA3E80; vram += 8)
        {
            *(vram) = 0x92;
            *(vram + 1) = 0x92;
            *(vram + 2) = 0x92;
            *(vram + 3) = 0x92;
            *(vram + 4) = 0x92;
            *(vram + 5) = 0x92;
            *(vram + 6) = 0x92;
            *(vram + 7) = 0x92;
        }
    }
}

void bench_modeY(void)
{
    unsigned char *vram;

    unsigned int loops;
    unsigned int num_loops = total_loops_modeY;

    for (loops = 0; loops < num_loops; loops++)
    {
        // PLANE 0
        outp(SC_DATA, 1 << (0 & 3));

        for (vram = (unsigned char *)0xA0000; vram < (unsigned char *)0xA3E80; vram += 8)
        {
            *(vram) = 0x46;
            *(vram + 1) = 0x46;
            *(vram + 2) = 0x46;
            *(vram + 3) = 0x46;
            *(vram + 4) = 0x46;
            *(vram + 5) = 0x46;
            *(vram + 6) = 0x46;
            *(vram + 7) = 0x46;
        }

        // PLANE 1
        outp(SC_DATA, 1 << (1 & 3));

        for (vram = (unsigned char *)0xA0000; vram < (unsigned char *)0xA3E80; vram += 8)
        {
            *(vram) = 0x03;
            *(vram + 1) = 0x03;
            *(vram + 2) = 0x03;
            *(vram + 3) = 0x03;
            *(vram + 4) = 0x03;
            *(vram + 5) = 0x03;
            *(vram + 6) = 0x03;
            *(vram + 7) = 0x03;
        }

        // PLANE 2
        outp(SC_DATA, 1 << (2 & 3));

        for (vram = (unsigned char *)0xA0000; vram < (unsigned char *)0xA3E80; vram += 8)
        {
            *(vram) = 0xCC;
            *(vram + 1) = 0xCC;
            *(vram + 2) = 0xCC;
            *(vram + 3) = 0xCC;
            *(vram + 4) = 0xCC;
            *(vram + 5) = 0xCC;
            *(vram + 6) = 0xCC;
            *(vram + 7) = 0xCC;
        }

        // PLANE 3
        outp(SC_DATA, 1 << (3 & 3));

        for (vram = (unsigned char *)0xA0000; vram < (unsigned char *)0xA3E80; vram += 8)
        {
            *(vram) = 0x92;
            *(vram + 1) = 0x92;
            *(vram + 2) = 0x92;
            *(vram + 3) = 0x92;
            *(vram + 4) = 0x92;
            *(vram + 5) = 0x92;
            *(vram + 6) = 0x92;
            *(vram + 7) = 0x92;
        }
    }
}

void execute_bench_modeY(void)
{
    // SET VIDEO MODE
    init_modeY();

    // PRE-HEAT
    timespent_modeY = profile_function(preheat_modeY);
    total_loops_modeY = PREHEAT_LOOPS * BENCH_TIME / timespent_modeY;

    // BENCHMARK
    timespent_modeY = profile_function(bench_modeY);
}

void show_results_modeY(void)
{
    double total_result;

    total_result = ((double)total_loops_modeY * 62.5 * 1000) / ((double)timespent_modeY);
    printf("VGA 320x200 256c (Y): %.2lf kb/s\n", total_result);
}