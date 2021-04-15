#! /bin/bash
../incompleted/scanner ../test/example1.kpl | diff ../test1/result1.txt -
../incompleted/scanner ../test/example2.kpl | diff ../test1/result2.txt -
../incompleted/scanner ../test/example3.kpl | diff ../test1/result3.txt -
../incompleted/scanner ../test/example4.kpl | diff ../test1/result4.txt -
