#pragma once

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef long i32;
typedef unsigned long u32;
typedef long long i64;
typedef unsigned long long u64;

#define PACKED __attribute__((packed))
#define ALIGNED(x) __attribute__((aligned(x)))