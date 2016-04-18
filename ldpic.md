# LdPic / SvPic File Format

## Introduction

SvPic and LdPic were two utilities for saving and loading image files to and
from disc on BBC Micro / Master computers, written by David Acton and Bruce
Smith.  The source code for these was originally included in issue 51 (October
1986) of the 'Acorn User' magazine (p 126-128).  (Thanks to Eelco Huininga for
letting me know the original issue that they featured in).

The information below was worked out from examining the (uncommented) LdPic
assembler and the files that it works on, so if you have any corrections
please feel free to make them.

## Summary

The LdPic format stores the contents of the BBC's graphics memory in a
run-length encoded format to save disc space.  This compression works most
effectively on images with areas of solid colour or regularly repeating
patterns.  The format also stores the mode and colour mappings required to load
and display the image correctly.

The LdPic format is bit rather than byte based.  Each of the values read from
the file is a specific number of bits in length.  When a value is more than 1
bit in length, the bits in the value are reversed compared to the input file.

## Header

The size of each value in the table below is in bits.

| Ref | Size | Description |
|-----|------|-------------|
| A   | 8    | The number of bits to load from the file and store in each byte of image memory. |
| B   | 8    | The mode to display the image in. |
| C   | 64   | The logical to physical colour mappings for the image as 16 values of 4 bits each.  Colour 15 is first, colour 0 last. |
| D   | 8    | The step size between bytes loaded to the image memory (see below). |
| E   | 8    | The number of bits to read from the file to give the repeat count for a repeated byte of graphics memory. |

## Image Data

Before reading any image data, the address to write to should be set to the top
of the screen memory plus the value of **D** - 1.

The first bit of a section of image data determines the data that follows.  If
this is **1** a repeat count of **E** bits should be read, followed by a value
of **A** bits, which should be then output the number of times indicated by the
repeat count.  If the first bit is **0** just a single value of size **A**
should be read and output.

After each value is output to a byte of screen memory, the address of the byte
to write to should be increased by the value **D**, unless this would make the
address fall outside of the screen memory.  In that case, **D** should be
decremented by one and the address to write to reset to the value of the top of
screen memory plus **D** - 1.

The above procedure should be repeated until the image has been loaded.
