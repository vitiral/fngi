# Imported by wasm.py
from collections import OrderedDict
from dataclasses import dataclass
import copy
import ctypes
from ctypes import c_bool as Bool

# This is the only way to get the base class for ctypes (_CData) which is
# technically private.
DataTy = ctypes.c_uint8.__bases__[0].__bases__[0]


def WloadStorePtr(ds: "Stack", instr: any):
    """Used for load/store wasm instructions."""
    offset, align = 0, 0
    if not isinstance(instr, int):
        _, offset, align = instr
    ptr = ds.popv(U32) + offset
    if align and ptr % align != 0:
        ptr += align - (ptr % align)
    return ptr


def WstoreValue(env: "Env", instr: any, ty: DataTy):
    value = env.ds.pop(I32)
    ptr = loadStorePtr(env.ds, instr)
    env.memory.set(ptr, value)

