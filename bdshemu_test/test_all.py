#
# Copyright (c) 2020 Bitdefender
# SPDX-License-Identifier: Apache-2.0
#
import os
import sys
import glob
import shutil
from zipfile import ZipFile
from pathlib import Path

TEMP_PATH = "!temp"

# A test file-name contains some indicators as to how the emulation should be done:
# - If '_16' is present in the name, emulation will be done on 16 bit
# - If '_32' is present in the name, emulation will be done on 32 bit
# - If '_64' is present in the name, emulation will be done on 64 bit
# - If '_r0' is present in the name, emulation will be done as kernel code
# A test case consists of minimum two files:
# - An '.test' file, containing the binary code to be emulated
# - An '.result' file, containing the emulation output
# A test is considered passed if the output produced during emulation is identical to the
# output containted in the '.result' file.

#
# test_dir
#
# Runs the tests inside the provided directory.
# Returns a tuplecontaining the total number of tests run and the number of failed tests.
#
def test_dir(dir, arch='x86'):
    total_tests = 0
    failed_tests = 0
    
    for f in glob.glob('%s\\*.test' % dir):
        base, _ = os.path.splitext(f)
        
        tst_file = f
        res_file = base + '.result'
        tmp_file = base + '.temp'
        
        if 0 < tst_file.find('_16'):
            mod = '-b16'
        elif 0 < tst_file.find('_32'):
            mod = '-b32'
        else:
            mod = '-b64'
            
        if 0 < tst_file.find('_r0'):
            mod += ' -k'
            
        print('    * Running test case %s...' % tst_file)
        os.system('disasm shemu %s -f %s >%s' % (mod, tst_file, tmp_file))
        try:
            res = open(res_file).read()
        except:
            print('    ! No result file provided for test %s!' % tst_file)
            
        try:
            tmp = open(tmp_file).read()
        except:
            print('    ! No result produced by test %s!' % tst_file)
        
        total_tests += 1
        if res != tmp:
            print('    **** FAILED! ****')
            failed_tests += 1
        else:
            print('    * Passed.')
            
        # Cleanup.
        os.remove(tmp_file)
        os.remove(tst_file + "_decoded.bin")

    return (total_tests, failed_tests)
        
#
# regenerate
#
def regenerate(dir, arch='x86'): 
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
                
        print('    * Regenerating test case %s...' % tst_file)
        os.system('disasm -exi shemu %s -f %s >%s' % (mod, tst_file, res_file))
        
        # Cleanup.
        os.remove(tst_file + "_decoded.bin")
       
#
# parse_dir_rec
#
def parse_dir_rec(dir, arch, handler):
    for f in glob.glob(dir + "\\*"):
        path, name = os.path.split(f)
        if name in ['.', '..']:
            continue
        if os.path.isdir(f):
            parse_dir_rec(f, arch, handler)
        handler(f, arch)


#
# test_archive
#
def test_archive(filename, arch='x86'):
    cleanup_files = []

    # Run the x86 test.
    print("Extracting test archive...")
    with ZipFile(filename) as zf:
        cleanup_files = zf.namelist()
        zf.extractall(path=TEMP_PATH)
        zf.close()

    print("Running tests...")
    total_tests, failed_tests = 0, 0
    for dn in glob.glob(os.path.join(TEMP_PATH, "*")):
        if not os.path.isdir(dn):
            continue
        print('Testing %s...' % dn)
        cnt_tests, cnt_failed = test_dir(dn, arch)
        
        total_tests += cnt_tests
        failed_tests += cnt_failed
    
    print("========================================================================")
    print("Summary:")
    print("Ran %d tests, %d failed" % (total_tests, failed_tests))
    print("========================================================================")
    print()
    
    shutil.rmtree(TEMP_PATH)


if __name__ == "__main__":
    # No arguments provided, auto-run all tests.
    if len(sys.argv) == 1:
        print("Running x86 tests...")
        test_archive("x86\\bdshemu_test_x86.zip")
        
        sys.exit(0)

    opt_dir = ""
    opt_regen = False
    opt_arch = 'x86'
    
    for arg in sys.argv[1:]:
        if os.path.isdir(arg):
            print("Will test directory", arg)
            opt_dir = arg
        if arg == "regenerate":
            print("Will regenerate tests...")
            opt_regen = True
    
    print("Identified architecture: ", opt_arch)
    
    # Single directory test.
    if not opt_dir:
        print("A directory containing tests must be supplied!")
        sys.exit(-1)

    if opt_regen:
        parse_dir_rec(opt_dir, opt_arch, regenerate)
    else:
        parse_dir_rec(opt_dir, opt_arch, test_dir)