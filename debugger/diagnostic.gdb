set pagination off
set confirm off
set print pretty on
break main.cpp:23
run
next
printf "\n--- main: a initialized ---\n"
print a
next
printf "\n--- main: S initialized ---\n"
print s
step
printf "\n--- entered foo ---\n"
info args
backtrace
print a
print *b
print c
finish
printf "\n--- returned to main ---\n"
print a
print s
continue
quit
