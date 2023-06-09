// DO NOT EDIT MANUALLY! GENERATED BY etc/gen.py
// See docs at: src/comp.fn
#include "civ.h"

#define CSZ_CATCH        0xff
#define T_NUM            0x0
#define T_HEX            0x1
#define T_ALPHA          0x2
#define T_SINGLE         0x3
#define T_SYMBOL         0x4
#define T_WHITE          0x5
#define C_UNTY           0x8
#define C_FN_STATE       0x7
#define FN_STATE_NO      0x0
#define FN_STATE_BODY    0x1
#define FN_STATE_STK     0x2
#define FN_STATE_INP     0x3
#define FN_STATE_OUT     0x4
#define TY_UNSIZED       0xffff
#define TY_MASK          0xc0
#define TY_VAR           0x40
#define TY_FN            0x80
#define TY_DICT          0xc0
#define TY_FN_NATIVE     0x20
#define TY_FN_TY_MASK    0xf
#define TY_FN_NORMAL     0x0
#define TY_FN_IMM        0x1
#define TY_FN_SYN        0x2
#define TY_FN_SYNTY      0x3
#define TY_FN_INLINE     0x4
#define TY_FN_COMMENT    0x5
#define TY_FN_METH       0x6
#define TY_FN_ABSMETH    0x7
#define TY_FN_SIG        0xf
#define TY_VAR_MSK       0x30
#define TY_VAR_LOCAL     0x0
#define TY_VAR_GLOBAL    0x10
#define TY_VAR_CONST     0x20
#define TY_VAR_ALIAS     0x30
#define TY_DICT_MSK      0x7
#define TY_DICT_NATIVE   0x0
#define TY_DICT_MOD      0x1
#define TY_DICT_BITMAP   0x2
#define TY_DICT_STRUCT   0x3
#define TY_DICT_ENUM     0x4
#define TY_DICT_ROLE     0x5
#define TY_NATIVE_SIGNED 0x8
#define TY_REFS          0x3

struct _TyDict;
struct _TyFn;

typedef struct {
  struct _TyFn*  drop;     // this:&This -> ()
  struct _TyFn*  alloc;    // this:&This sz:S alignment:U2 -> Ref
  struct _TyFn*  free;     // this:&This dat:Ref sz:S alignment:U2 -> ()
  struct _TyFn*  maxAlloc; // this:&This -> S
} MSpArena;
typedef struct { MSpArena* m; void* d; } SpArena;

typedef struct {
  struct _TyFn*  read;   // this:&This -> ()
  struct _TyFn*  asBase; // this:&This -> &BaseFile
} MSpReader;
typedef struct { MSpReader* m; void* d; } SpReader;

typedef struct _FileInfo {
  CStr* path;
  U2 line;
} FileInfo;

typedef struct _TyBase {
  struct _TyBase* l;
  struct _TyBase* r;
  U2 meta;
} TyBase;

typedef struct _TyI {
  struct _TyI* next;
  U2 meta;
  U2 arrLen;
  CStr* name;
  TyBase* ty;
} TyI;

typedef struct _TyIBst {
  struct _TyIBst* l;
  struct _TyIBst* r;
  TyI tyI;
} TyIBst;

typedef struct _Key {
  Slc name;
  TyI* tyI;
} Key;

typedef struct _Ty {
  struct _Ty* l;
  struct _Ty* r;
  U2 meta;
  U2 line;
  CStr* name;
  TyI* tyKey;
  struct _TyDict* container;
  FileInfo* file;
} Ty;

typedef struct _TyDict {
  struct _TyDict* l;
  struct _TyDict* r;
  U2 meta;
  U2 line;
  CStr* name;
  TyI* tyKey;
  struct _TyDict* container;
  FileInfo* file;
  Ty* children;
  TyI* fields;
  U2 sz;
} TyDict;

typedef struct _DictStk {
  TyDict** dat;
  U2 sp;
  U2 cap;
} DictStk;

typedef struct _TyVar {
  struct _TyVar* l;
  struct _TyVar* r;
  U2 meta;
  U2 line;
  CStr* name;
  TyI* tyKey;
  TyDict* container;
  FileInfo* file;
  S v;
  TyI* tyI;
} TyVar;

typedef struct _InOut {
  TyI* inp;
  TyI* out;
} InOut;

typedef struct _FnSig {
  struct _FnSig* l;
  struct _FnSig* r;
  U2 meta;
  InOut io;
} FnSig;

typedef struct _TyFn {
  struct _TyFn* l;
  struct _TyFn* r;
  U2 meta;
  U2 line;
  CStr* name;
  TyI* tyKey;
  TyDict* container;
  FileInfo* file;
  Ty* locals;
  U1* code;
  TyI* inp;
  TyI* out;
  U2 len;
  U1 lSlots;
} TyFn;

typedef struct _TyDb {
  BBA* bba;
  Stk tyIs;
  Stk done;
} TyDb;

typedef struct _Blk {
  struct _Blk* next;
  S start;
  SllS* breaks;
  TyI* startTyI;
  TyI* endTyI;
} Blk;

typedef struct _GlobalsCode {
  SpReader src;
  FileInfo* srcInfo;
  Buf token;
  Buf code;
  U2 metaNext;
  U2 cstate;
  U2 fnLocals;
  U1 fnState;
  Blk* blk_;
  TyFn* compFn;
} GlobalsCode;

typedef struct _Globals {
  GlobalsCode c;
  BBA* bbaDict;
  TyDict rootDict;
  DictStk dictStk;
  DictStk implStk;
  CBst* cBst;
  TyIBst* tyIBst;
  FnSig* fnSigBst;
  TyDb tyDb;
  TyDb tyDbImm;
  BBA bbaTyImm;
} Globals;
