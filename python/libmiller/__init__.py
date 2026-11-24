"""Typed Python bindings for the libmiller C library."""
import ctypes
import ctypes.util
import os
from dataclasses import dataclass
from enum import IntEnum
from functools import lru_cache
from typing import Optional

__all__ = ["Status", "Result", "check_prime"]

class Status(IntEnum):
    NOT_PRIME = 0
    PRIME = 1
    PRIME_GRH = 2
    INCOMPLETE = 3
    INVALID = 4

@dataclass(frozen=True)
class Result:
    status: Status
    bases_tested: int
    witness: int

@lru_cache(maxsize=8)
def _load(path: str):
    lib = ctypes.CDLL(path)
    lib.miller_abi_version.argtypes = []
    lib.miller_abi_version.restype = ctypes.c_uint
    if lib.miller_abi_version() != 1:
        raise RuntimeError("unsupported libmiller ABI (expected 1)")
    lib.miller_check_decimal.argtypes = [
        ctypes.c_char_p, ctypes.c_uint64, ctypes.POINTER(ctypes.c_uint64),
        ctypes.POINTER(ctypes.c_char), ctypes.c_size_t,
    ]
    lib.miller_check_decimal.restype = ctypes.c_int
    return lib

def check_prime(n: int, *, max_bases: int = 0,
                library: Optional[str] = None) -> Result:
    """Check an integer. Zero budget is unlimited; inspect status explicitly.

    The native library is resolved from library, LIBMILLER_PATH or the system.
    This binding supports base limits; cancellation callbacks use the C API.
    """
    if type(n) is not int:
        raise TypeError("n must be an int")
    if type(max_bases) is not int or not 0 <= max_bases <= (1 << 64) - 1:
        raise ValueError("max_bases must be an integer in [0, 2**64 - 1]")
    path = library or os.environ.get("LIBMILLER_PATH") or ctypes.util.find_library("miller")
    if not path:
        raise OSError("libmiller not found; build the shared library and set LIBMILLER_PATH")
    lib = _load(os.fspath(path))
    decimal = str(n).encode("ascii")
    witness = ctypes.create_string_buffer(len(decimal) + 1)
    tested = ctypes.c_uint64()
    status = Status(lib.miller_check_decimal(
        decimal, max_bases, ctypes.byref(tested), witness, len(witness)))
    if status == Status.INVALID:
        raise RuntimeError("libmiller rejected validated input")
    return Result(status, tested.value, int(witness.value))
