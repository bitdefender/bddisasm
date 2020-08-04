# The Bitdefender disassembler Python wrapper

## Build

Building and installing is easy:

```bash
python3 setup.py build
python3 setup.py install
```

## Usage

Use it by importing the pybddisasm.disasm module:

```python
from pybddisasm.bddisasm import *

instr = nd_decode_ex2(buff, arch, arch, arch, vendor, current_rip)
```

## Pip

Use pip to install the package:

```bash
pip install pybddisasm
```
