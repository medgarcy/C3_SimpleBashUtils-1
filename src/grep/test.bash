#test -e flag
./s21_grep -e 123 test.txt > s21_grep_out.txt
grep -e 123 test.txt > grep_out.txt
echo "test -e"
diff -q s21_grep_out.txt grep_out.txt

#test -i flag
./s21_grep -i new test.txt > s21_grep_out.txt
grep -i new test.txt > grep_out.txt
echo "test -i"
diff -q s21_grep_out.txt grep_out.txt

#test -v flag
./s21_grep -v 123 test.txt > s21_grep_out.txt
grep -v 123 test.txt > grep_out.txt
echo "test -v"
diff -q s21_grep_out.txt grep_out.txt

#test -c flag
./s21_grep -c 123 test.txt test2.txt > s21_grep_out.txt
grep -c 123 test.txt test2.txt > grep_out.txt
echo "test -c"
diff -q s21_grep_out.txt grep_out.txt

#test -l flag
./s21_grep -c -l 123 test.txt > s21_grep_out.txt
grep -c -l 123 test.txt > grep_out.txt
echo "test -l"
diff -q s21_grep_out.txt grep_out.txt

#test -h flag
./s21_grep -h 123 test.txt > s21_grep_out.txt
grep -h 123 test.txt > grep_out.txt
echo "test -h"
diff -q s21_grep_out.txt grep_out.txt

#test -o flag
./s21_grep -o 123 test.txt > s21_grep_out.txt
grep -o 123 test.txt > grep_out.txt
echo "test -o"
diff -q s21_grep_out.txt grep_out.txt

#test -f flag
./s21_grep -f test_pattern.txt test.txt > s21_grep_out.txt
grep -f test_pattern.txt test.txt > grep_out.txt
echo "test -f"
diff -q s21_grep_out.txt grep_out.txt