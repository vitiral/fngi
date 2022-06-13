#ifndef __KERNEL_H
#define __KERNEL_H
#include <stdint.h>
#include "constants.h"
#include "../linux/types.h" // TODO: support more than just linux

typedef U2 FErr;

// If F_INDEX is set, fid is a file descriptor.
// Else it is a "mock" BufPlc
#define F_INDEX    (1 << (RSIZE * 4 - 1))
#define F_FD(F)    ((~F_INDEX) & (F).fid)
const U2 F_seeking  = 0x00;
const U2 F_reading  = 0x01;
const U2 F_writing  = 0x02;
const U2 F_stopping = 0x03;

const U2 F_done     = 0xD0;
const U2 F_stopped  = 0xD1;
const U2 F_eof      = 0xD2;

const U2 F_error    = 0xE0;
const U2 F_Eperm    = 0xE1;
const U2 F_Eio      = 0xE2;

#define F_plcBuf(F)  ((PlcBuf*) &(F).b)
typedef struct {
  U4 pos;   // current position in file. If seek: desired position.
  Ref fid;  // file id or reference
  Buf b;    // buffer for reading or writing data
  U2 plc;   // place, makes buf a PlcBuf. write: write pos.
  U2 code;  // status or error (F_*)
} File;

typedef void (*fileMethod)(File* f);

typedef struct {
  fileMethod open;    // open the file
  fileMethod close;   // immediately close the file
  fileMethod stop;    // stop current operation
  fileMethod seek;    // seek to pos
  fileMethod clear;   // clear all data after pos
  fileMethod read;    // read data from pos
  fileMethod insert;  // insert data at pos
} FileMethods;

typedef struct { FileMethods* m; File* f; } FileRole;

#define BLOCK_END  0xFF
#define BLOCK_PO2  12
#define BLOCK_SIZE (1<<BLOCK_PO2)

typedef U1 Instr;

typedef struct { Ref ref; U2 sp; U2 cap; }                  Stk;
typedef struct { U1 previ; U1 nexti; }                      BANode;
typedef struct { Ref nodes; Ref blocks; U1 rooti; U1 cap; } BA;
typedef struct { Ref ba; U1 rooti; U2 len; U2 cap; }        BBA;
typedef struct { Ref root; Ref free; }                      Dict;
typedef struct { Ref l; Ref r; Ref ckey; U1 m0; U1 m1; U4 v; } DNode;

typedef struct {
  U1 valTy;     U1 _align;  U2 _align2;
  U2 valueASz;
  U2 valueBSz;
  U4 valueA;
  U4 valueB;
  Ref msg;
} ErrData;

typedef struct {
  U4 _null;
  Ref memTop;
  BA ba;
  BBA bba;
  BBA bbaTmp;
  Ref dict;
} Kern;


typedef struct {
  U2 _null;
  U2 err;
  Stk ws;   // working stack
  Stk ls;   // locals stack
  Stk cs;   // call stack
  Stk csz;  // call stack size bytes
  Buf gbuf;   // global data buffer (for tracking growing globals)
  Ref curBBA; // current bba to use for storing code/dictionary
  File src;
  U1 buf0[TOKEN_SIZE];
  U1 logLvlSys;
  U1 logLvlUsr;
  U2 _unused;

  int syserr;
} Globals;

typedef struct { Ref ep; } VM;

#endif // __KERNEL_H
