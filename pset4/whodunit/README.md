# Questions

## What's `stdint.h`?

A header file for integer type declaration.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

Using integer types with different sizes.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

BYTE: 1 byte; DWORD: 4 bytes without a sign; LONG: 4 bytes with a sign; WORD 2 bytes;

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

Ascii: BM, Hex: 0x424d;

## What's the difference between `bfSize` and `biSize`?

bfSize is the size of the bitmap file in bytes; bfSize is the number of bytes required by the structure;

## What does it mean if `biHeight` is negative?

The origin is the upper-left corn;

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount;

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

The input file may not exists;

seet http://pubs.opengroup.org/onlinepubs/009695399/functions/fopen.html
for reasons that will cause fopen to fail;

## Why is the third argument to `fread` always `1` in our code?

We always read one BITMAPFILEHEADER or one BITMAPINFOHEADER at a time;

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

Forward the file read cursor to skip the Ox0000 padding bytes

## What is `SEEK_CUR`?
SEEK_CUR and SEEK_SET and SEEK_END are three integer flags to specify what
the file position indicator is moved relative to.

SEEK_CUR is the current position indicator;

