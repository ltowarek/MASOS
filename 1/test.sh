#!/bin/sh
ls
ls -la test/sample_input
python3.4 test/exercise_1_tester.py "build/exercise_1.exe 5" "test/sample_input/test0" "test/sample_output/wzor0"
python3.4 test/exercise_1_tester.py "build/exercise_1.exe 5" "test/sample_input/test1" "test/sample_output/wzor1"
python3.4 test/exercise_1_tester.py "build/exercise_1.exe 5" "test/sample_input/test2" "test/sample_output/wzor2"
python3.4 test/exercise_1_tester.py "build/exercise_1.exe 5" "test/sample_input/test3" "test/sample_output/wzor3"
python3.4 test/exercise_1_tester.py "build/exercise_1.exe 5" "test/sample_input/test4" "test/sample_output/wzor4"
python3.4 test/exercise_1_tester.py "build/exercise_1.exe 5" "test/sample_input/test5" "test/sample_output/wzor5"
