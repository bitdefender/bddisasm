#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import os
import sys
import glob
from zipfile import ZipFile
from pathlib import Path

total_tests = 0
failed_tests = 0

def test_dir(dir):
    global total_tests
    global failed_tests
    
    for f in glob.glob('%s\\*' % dir):
        if -1 == f.find('.'):
            if 0 < f.find('_16'):
                mod = '-b16'
            elif 0 < f.find('_32'):
                mod = '-b32'
            else:
                mod = '-b64'
            if 0 < f.find('_r0'):
                mod += ' -k'
                
            print('    * Running test case %s...' % f)
            os.system('disasm shemu %s -f %s >%s.temp' % (mod, f, f))
            try:
                res = open('%s.result' % f).read()
            except:
                print('    ! No result file provided for test %s!' % f)
                
            try:
                tmp = open('%s.temp' % f).read()
            except:
                print('    ! No result produced by test %s!' % f)
            
            total_tests += 1
            if res != tmp:
                print('    **** FAILED! ****')
                failed_tests += 1
            else:
                print('    * Passed.')
                
    for f in glob.glob('%s\\*_decoded.bin' % dir):
        os.remove(f)
    for f in glob.glob('%s\\*.temp' % dir):
        os.remove(f)
        
def regenerate(dir): 
    for f in glob.glob('%s\\*' % dir):
        if -1 == f.find('.'):
            if 0 < f.find('_16'):
                mod = '-b16'
            elif 0 < f.find('_32'):
                mod = '-b32'
            else:
                mod = '-b64'
            if 0 < f.find('_r0'):
                mod += ' -k'
                
            print('    * Regenerating test case %s...' % f)
            os.system('disasm -exi shemu %s -f %s >%s.result' % (mod, f, f))
                            
    for f in glob.glob('%s\\*_decoded.bin' % dir):
        os.remove(f)    

cleanup_files = []

print("Extracting test archive...\n")
with ZipFile('bdshemu_test.zip') as zf:
    cleanup_files = zf.namelist()
    zf.extractall()
print("Done!\n")    

for dn in glob.glob("*"):
    if not os.path.isdir(dn):
        continue
    print('Testing %s...' % dn)
    test_dir(dn)
print("Ran %d tests, %d failed" % (total_tests, failed_tests))

print("Cleaning up test files...\n")
for f in cleanup_files:
    p = Path(os.getcwd()) / f
    if p.is_file():
        p.unlink()
print("Done!\n")
