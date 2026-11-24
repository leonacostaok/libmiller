"""Compare Python/C and JavaScript status, witness and exact work counts."""
import json
from pathlib import Path
import random
import subprocess
from libmiller import check_prime, Status

root = Path(__file__).resolve().parents[2]
cases = json.loads((root / 'tests/vectors.json').read_text())
rng = random.Random(4464)
for _ in range(150):
    cases.append({'n': str(rng.getrandbits(rng.choice([16, 64, 128, 256]))),
                  'max_bases': str(rng.choice([1, 2, 10, 25]))})
output = subprocess.check_output(['node', str(root / 'javascript/vector-runner.mjs')],
                                 input=json.dumps(cases), text=True)
names = {Status.NOT_PRIME:'not-prime', Status.PRIME:'prime',
         Status.PRIME_GRH:'prime-grh', Status.INCOMPLETE:'incomplete'}
for v, js in zip(cases, json.loads(output)):
    c = check_prime(int(v['n']), max_bases=int(v['max_bases']))
    expected = {'status':names[c.status], 'basesTested':str(c.bases_tested), 'witness':str(c.witness)}
    if js != expected: raise AssertionError((v, js, expected))
print(f'{len(cases)} cross-language cases passed (status, base count, witness)')
