#test -b flag
./s21_cat -b test.txt > s21_cat_out.txt
cat -b test.txt > cat_out.txt
echo "test -b"
diff -q s21_cat_out.txt cat_out.txt

#test -e flag
./s21_cat -e test.txt > s21_cat_out.txt
cat -e test.txt > cat_out.txt
echo "test -e"
diff -q s21_cat_out.txt cat_out.txt

#test -n flag
./s21_cat -n test.txt > s21_cat_out.txt
cat -n test.txt > cat_out.txt
echo "test -n"
diff -q s21_cat_out.txt cat_out.txt

#test -s flag
./s21_cat -s test.txt > s21_cat_out.txt
cat -s test.txt > cat_out.txt
echo "test -s"
diff -q s21_cat_out.txt cat_out.txt

#test -t flag
./s21_cat -t test.txt > s21_cat_out.txt
cat -t test.txt > cat_out.txt
echo "test -t"
diff -q s21_cat_out.txt cat_out.txt