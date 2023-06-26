#
# Copyright (c) 2023 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import sys
import unittest
import pybddisasm

class TestNdDecode(unittest.TestCase):
    def test_nd_decode_bytes(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = b"\x48\x8b\x05\xb8\x13\x00\x00"
        ret, instrux = pybddisasm.nd_decode(instr_bytes, pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

        for idx in range(0, len(instr_bytes)):
            self.assertEqual(pybddisasm.nd_get_instrux_bytes(instrux)[idx], instr_bytes[idx])

    def test_nd_decode_bytes_array(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = bytearray(b'\x48\x8b\x05\xb8\x13\x00\x00')
        ret, instrux = pybddisasm.nd_decode(instr_bytes, pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

        for idx in range(0, instrux.Length):
            self.assertEqual(pybddisasm.nd_get_instrux_bytes(instrux)[idx], instr_bytes[idx])


    def test_nd_decode_memory_view(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = memoryview(bytearray(b'\x48\x8b\x05\xb8\x13\x00\x00'))
        ret, instrux = pybddisasm.nd_decode(instr_bytes, pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

        for idx in range(0, instrux.Length):
            self.assertEqual(pybddisasm.nd_get_instrux_bytes(instrux)[idx], instr_bytes[idx])

    def test_nd_decode_ex_bytes(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = b"\x48\x8b\x05\xb8\x13\x00\x00"
        ret, instrux = pybddisasm.nd_decode_ex(instr_bytes, len(instr_bytes), pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

        for idx in range(0, instrux.Length):
            self.assertEqual(pybddisasm.nd_get_instrux_bytes(instrux)[idx], instr_bytes[idx])

    def test_nd_decode_ex_bytes_array(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = bytearray(b'\x48\x8b\x05\xb8\x13\x00\x00')
        ret, instrux = pybddisasm.nd_decode_ex(instr_bytes, len(instr_bytes), pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

        for idx in range(0, instrux.Length):
            self.assertEqual(pybddisasm.nd_get_instrux_bytes(instrux)[idx], instr_bytes[idx])


    def test_nd_decode_ex_memory_view(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = memoryview(bytearray(b'\x48\x8b\x05\xb8\x13\x00\x00'))
        ret, instrux = pybddisasm.nd_decode_ex(instr_bytes, len(instr_bytes), pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

        for idx in range(0, instrux.Length):
            self.assertEqual(pybddisasm.nd_get_instrux_bytes(instrux)[idx], instr_bytes[idx])

    def test_nd_decode_ex2_bytes(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = b"\x48\x8b\x05\xb8\x13\x00\x00"
        ret, instrux = pybddisasm.nd_decode_ex2(instr_bytes, len(instr_bytes), pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64, 
                pybddisasm.ND_STACK_64, pybddisasm.ND_VEND_INTEL);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

        for idx in range(0, instrux.Length):
            self.assertEqual(pybddisasm.nd_get_instrux_bytes(instrux)[idx], instr_bytes[idx])

    def test_nd_decode_ex2_bytes_array(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = bytearray(b'\x48\x8b\x05\xb8\x13\x00\x00')
        ret, instrux = pybddisasm.nd_decode_ex2(instr_bytes, len(instr_bytes), pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64,
                pybddisasm.ND_STACK_64,pybddisasm.ND_VEND_INTEL);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

        for idx in range(0, instrux.Length):
            self.assertEqual(pybddisasm.nd_get_instrux_bytes(instrux)[idx], instr_bytes[idx])


    def test_nd_decode_ex2_memory_view(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = memoryview(bytearray(b'\x48\x8b\x05\xb8\x13\x00\x00'))
        ret, instrux = pybddisasm.nd_decode_ex2(instr_bytes, len(instr_bytes), pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64,
                pybddisasm.ND_STACK_64,pybddisasm.ND_VEND_INTEL);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

        for idx in range(0, instrux.Length):
            self.assertEqual(pybddisasm.nd_get_instrux_bytes(instrux)[idx], instr_bytes[idx])

    def test_nd_rip_relative(self):
        # mov    rax,QWORD PTR [rip+0x13b8]
        instr_bytes = memoryview(bytearray(b'\x48\x8b\x05\xb8\x13\x00\x00'))
        ret, instrux = pybddisasm.nd_decode_ex2(instr_bytes, len(instr_bytes), pybddisasm.ND_CODE_64, pybddisasm.ND_DATA_64,
                pybddisasm.ND_STACK_64,pybddisasm.ND_VEND_INTEL);

        self.assertIsInstance(ret, int, "'return-value' is not instance of 'int'")
        self.assertEqual(ret, 0, "'return-value' is not equal to 0")

if __name__ == '__main__':
    unittest.main()
