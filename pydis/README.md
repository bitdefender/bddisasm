# The Bitdefender disassembler Python wrapper

## Build

Building and installing is easy:

```bash
python3 setup.py build
python3 setup.py install
```

## Usage

Use it by importing the pydis.disasm module:

```python
from pydis.disasm import *

instr = nd_decode_ex2(buff, arch, arch, arch, vendor, current_rip)
```

