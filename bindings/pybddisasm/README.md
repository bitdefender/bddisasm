# The Bitdefender disassembler Python wrapper

## Build

Building and installing is easy:

```bash
python3 setup.py build
python3 setup.py install
```

## Usage

Use it by importing the pybddisasm module:

```python
import pybddisasm

ret, instr = pybddisasm.nd_decode_ex(code, def_code, def_data, def_stack)
```

## Example

```python
import pybddisasm

code = b"\x55\x48\x8b\x05\xb8\x13\x00\x00"
offset = 0

while offset < len(code):
    _, instr = pybddisasm.nd_decode_ex(code[offset:], len(code[offset:]), pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64)

    if instr is None:
        break

    _, text = pybddisasm.nd_to_text(instr, 0x0)
    print(text)

    offset += instr.Length
```

## Pip

Use pip to install the package:

```bash
pip install pybddisasm
```
