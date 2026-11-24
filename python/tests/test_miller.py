import ctypes
import json
import os
from pathlib import Path
import unittest
from libmiller import check_prime, Status
from libmiller import _load

ROOT = Path(__file__).resolve().parents[2]
NAMES = {Status.NOT_PRIME: 'not-prime', Status.PRIME: 'prime',
         Status.PRIME_GRH: 'prime-grh', Status.INCOMPLETE: 'incomplete'}

class MillerTests(unittest.TestCase):
    def test_vectors(self):
        for v in json.loads((ROOT / 'tests/vectors.json').read_text()):
            with self.subTest(v=v):
                r = check_prime(int(v['n']), max_bases=int(v['max_bases']))
                self.assertEqual(NAMES[r.status], v['status'])
                if r.status != Status.NOT_PRIME:
                    self.assertEqual(r.witness, 0)

    def test_sieve(self):
        sieve = [True] * 2000
        sieve[:2] = [False, False]
        for i in range(2, 45):
            if sieve[i]:
                for j in range(i*i, 2000, i): sieve[j] = False
        for n, expected in enumerate(sieve):
            r = check_prime(n)
            self.assertEqual(r.status in (Status.PRIME, Status.PRIME_GRH), expected)

    def test_validation(self):
        for n in (True, '17', 17.0, None):
            with self.assertRaises(TypeError): check_prime(n)
        for budget in (-1, 1 << 64, True, 1.0, None):
            with self.assertRaises(ValueError): check_prime(101, max_bases=budget)
        with self.assertRaises(OSError): check_prime(101, library='/missing/libmiller.so')
        self.assertEqual(check_prime(101, max_bases=(1 << 64)-1).status, Status.PRIME_GRH)

    def test_decimal_abi(self):
        lib = _load(os.environ['LIBMILLER_PATH'])
        for text in (b'', b'-', b'+17', b' 17', b'1 7', b'17\n', b'0x11', b'1.0', None):
            count = ctypes.c_uint64(99)
            w = ctypes.create_string_buffer(64)
            status = lib.miller_check_decimal(text, 0, ctypes.byref(count), w, len(w))
            self.assertEqual(status, Status.INVALID)
            self.assertEqual(count.value, 0)
        w = ctypes.create_string_buffer(b'XYZ', 4)
        self.assertEqual(lib.miller_check_decimal(b'101',0,None,w,3),Status.INVALID)
        self.assertEqual(w.raw[1:],b'YZ\0')
        self.assertEqual(lib.miller_check_decimal(b'101',0,None,None,1),Status.INVALID)
        self.assertEqual(lib.miller_check_decimal(b'00041',0,None,None,0),Status.PRIME_GRH)
        self.assertEqual(lib.miller_check_decimal(b'-0',0,None,None,0),Status.NOT_PRIME)

if __name__ == '__main__': unittest.main()
