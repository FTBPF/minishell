export TESTVAR=hello
echo $TESTVAR
echo $TESTVAR | cat
cat << EOF > output.txt
$TESTVAR world
test line
