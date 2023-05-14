call :bye

push [0+bx]
push 2
push 3
pop [0+ax]
out
out
out

:bye

push 7
out

hlt
