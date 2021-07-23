# This execution engine is written from scratch but inspired by "A Talk Near
# the Future of Python" presented by David Beazley. You can watch the whole
# talk here: https://www.youtube.com/watch?v=r-A78RgMhZU
#
# In it we define a Machine that interacts with our Env class, specifically the
# memory inside it. This allows us to define and execute functions.

from pdb import set_trace as dbg
from .wasm_constants import *
from .wasm import wasmSubroutines
from .types import Env, ENV
from typing import List
from ctypes import sizeof

STACK_TYPES = {U32, I32, U64, I64, F32, F64}
WASM_TYPES = {U8, I8, U16, I16}
WASM_TYPES.update(STACK_TYPES)

def checkTy(value: any, ty: DataTy):
    if type(value) is not ty: raise TypeError(f"{value} is not of type {ty}")

def assertTyEq(v1: DataTy, v2: DataTy):
    assert type(v1) == type(v2)
    assert v1.value == v2.value

def needAlign(size: int) -> int:
    """Return padding bytes needed to align a value of size."""
    if size % 4 == 0:
        return 0
    return 4 - (size % 4)

def formatArgs(args):
    out = []
    for a in args:
        if isinstance(a, int): out.append(hex(a))
        elif isinstance(a, (tuple, list)): out.append("(block)")
        else: out.append("???")
    return "Args: " + ' '.join(out)


def runWasm(env: Env, code: List[any]):
    ds = env.ds
    index = 0
    while index < len(code):
        instr = code[index]
        brLevel = None
        # wi stands for "webassembly instr"
        if isinstance(instr, int):
            wi = instr
            args = ()
        else:
            wi, *args = instr
        print("\nSTART ", wasmName[wi], formatArgs(args), '', ds.debugStr())
        subroutine = wasmSubroutines.get(wi)
        if subroutine: subroutine(env, args)

        elif wi == w.br: brLevel = args[0]
        elif wi == w.br_if and ds.popv(U32):
            brLevel = args[0]
        elif wi == w.block:
            brLevel = runWasm(env, args[0])
            if brLevel == 0:
                # br has been handled by getting here
                brLevel = None
            elif brLevel > 0: return brLevel - 1
        elif wi == w.loop:
            while True:
                brLevel = runWasm(env, args[0])
                if brLevel is None: break # ended block w/out br
                if brLevel > 0: return brLevel - 1
                # else loop again

        print("END   ", wasmName[wi], ds.debugStr())

        if brLevel is not None: return brLevel
        index += 1


def testRunSimple():
    env = ENV.copyForTest()
    runWasm(env, [
        (w.i32.const, 10),
        (w.i32.const, 11),
        w.i32.add,
        (w.i32.const, 2),
        w.i32.mul,
    ])
    assert 42 == env.ds.popv(I32)

def testRunLoop():
    env = ENV.copyForTest()
    vPtr = env.heap.grow(4)
    runWasm(env, [
        # @v = 0;
        (w.i32.const, vPtr),
        (w.i32.const, 0),
        w.i32.store,
        # while True:
        #  out += 1
        #  if 10 > out: continue
        (w.loop, [
            (w.i32.const, vPtr),
            (w.i32.const, vPtr),
            w.i32.load,
            (w.i32.const, 1), w.i32.add,
            w.i32.store,
            (w.i32.const, vPtr), w.i32.load,
            (w.i32.const, 10),
            w.i32.lt_s,
            (w.br_if, 0), # continue if true
        ]),
        (w.i32.const, vPtr),
        w.i32.load,
    ])
    result = env.ds.popv(I32)
    assert 10 == result
