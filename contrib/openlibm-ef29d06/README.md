# OpenLibm

[![Build Status](https://travis-ci.org/JuliaLang/openlibm.svg?branch=master)](https://travis-ci.org/JuliaLang/openlibm)

[OpenLibm](http://www.openlibm.org) is an effort to have a high quality, portable, standalone
C mathematical library ([`libm`](http://en.wikipedia.org/wiki/libm)).
It can be used standalone in applications and programming language
implementations.

The project was born out of a need to have a good `libm` for the
[Julia programming langage](http://www.julialang.org) that worked
consistently across compilers and operating systems, and in 32-bit and
64-bit environments.

## Platform support

OpenLibm builds on Linux, Mac OS X, Windows, and FreeBSD. It builds
with both GCC and clang. Although largely tested on x86, it also
includes experimental support for ARM.

## Build instructions

1. Use `make` to build OpenLibm.
2. Use `make USEGCC=1` to build with GCC. This is the default on
   Linux and Windows.
3. Use `make USECLANG=1` to build with clang. This is the default on OS X
   and FreeBSD.
4. Use `make ARCH=i386` to build for i386. Other supported architectures are
   i486, i586, i686, x86_64, and various arm architectures.
