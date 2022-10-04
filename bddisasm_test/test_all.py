#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import os
import sys
import glob

total_tests = 0
failed_tests = 0

def get_dict(ins):
    d = {}
    prefix = ""
    
    # Remove lines that don't contain tokens.
    lines = ins.split("\n")
    
    # Main disassembly.
    d["disasm"] = lines[0]
    
    for line in lines[1:]:
        tokens = line.split(",")
        for t in tokens:
            val = t.split(":")
            if len(val) != 2:
                continue
            a = val[0].strip(" ")
            b = val[1].strip(" ")
            if a == "Operand":
                prefix = "op%s_" % b
            a = prefix + a
            d[a] = b
    return d

def compare_instruction(i1, i2):
    x1 = get_dict(i1)
    x2 = get_dict(i2)
    for t in x1:
        if not t in x2:
            print("    ! ERROR: Token '%s', value '%s' missing from output!" % (t, x1[t]))
            return False
        if x1[t] != x2[t]:
            print("    ! ERROR: Token '%s' value mismatch: expected '%s', got '%s'!" % (t, x1[t], x2[t]))
            return False
    for t in x2:
        if not t in x1:
            print("    ! WARNING: Extra token in result: '%s', value '%s'!" % (t, x2[t]))
            
    return True

def compare_results(data1, data2):
    ins1 = data1.split("\n\n")
    ins2 = data2.split("\n\n")
    if len(ins1) != len(ins2):
        print("    ! Different number of instructions in output: expected %d, got %d!" % (len(ins1), len(ins2)))
        return False
    for i in range(0, len(ins1)):
        if not compare_instruction(ins1[i], ins2[i]):
            print("    ! ERROR: Instruction mismatch at %d!" % (i))
            return False
    return True

def test_dir(dir):
    global total_tests
    global failed_tests
    
    for f in glob.glob('%s\\*.test' % dir):
        base, _ = os.path.splitext(f)
        
        tst_file = f
        res_file = base + '.result'
        tmp_file = base + '.temp'
        
        if 0 < f.find('_16'):
            mod = '-b16'
        elif 0 < f.find('_32'):
            mod = '-b32'
        else:
            mod = '-b64'
        if 0 < f.find('_r0'):
            mod += ' -k'
            
        if 0 < f.find('_skip'):
            mod += ' -skip16'
            
        print('    * Running test case %s...' % f)
        os.system('disasm -exi %s -f %s >%s' % (mod, tst_file, tmp_file))
        try:
            res = open(res_file).read()
        except:
            print('    ! No result file provided for test %s!' % tst_file)
            
        try:
            tmp = open(tmp_file).read()
        except:
            print('    ! No result produced by test %s!' % tst_file)
        
        total_tests += 1
        if not compare_results(res, tmp):
            print('    **** FAILED! ****')
            failed_tests += 1
        else:
            print('    * Passed.')

        # Cleanup.
        os.remove(tmp_file)
        
def regenerate(dir): 
    for f in glob.glob('%s\\*.test' % dir):
        base, _ = os.path.splitext(f)
        
        tst_file = f
        res_file = base + '.result'
        
        if 0 < f.find('_16'):
            mod = '-b16'
        elif 0 < f.find('_32'):
            mod = '-b32'
        else:
            mod = '-b64'
        if 0 < f.find('_r0'):
            mod += ' -k'
        if 0 < f.find('_skip'):
            mod += ' -skip16'
                
        print('    * Regenerating test case %s...' % tst_file)
        os.system('disasm -exi %s -f %s >%s' % (mod, tst_file, res_file))

if __name__ == "__main__":
    for dn in glob.glob("x86\\*"):
        if not os.path.isdir(dn):
            continue
        if "regenerate" in sys.argv:
            print('Regenerating %s...' % dn)
            regenerate(dn)
        else:
            print('Testing %s...' % dn)
            test_dir(dn)
            
    print("Ran %d tests, %d failed" % (total_tests, failed_tests))
