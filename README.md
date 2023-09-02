# Tatar programing language
As our final project in C course we had to create own programing language. I am Tatar, so I decided that tatar language will be interesting to write it.

This project use my assembler (hereinafter referred to as asm) and my virtual machine (hereinafter referred to as cpu) in backend,
my text funcs to work with it to process written in my language and asm files, my tree and funcs to dump it grafically to debug recursive descent easily.

## Some words about syntax and frontend
Obviously, syntax consist of tatar words

| Word |       Meaning      | Real translate (just for fun) |
|------|--------------------|-------------------------------|
|esh       |func definition |work                           |
|bir       |return          |give away                      | 
|tugyaryak |while           |circle                         |
|egyar     |if              |if                             | 
|bylmasa   |else            |if will not be                 | 
|yozyrga   |printf          |write                          | 
|ukyrga    |scanf           |read                           | 
|bashlau   |{               |begin                          | 
|egyar     |}               |end                            | 

Most of all math operations are the same (+, -, *, /, >=, >, <=, <) but equal like this: <>, not equal like this: ><.
Must be "bashlau" and "tuktau" in the beginning of block (func, condition or cycle) and in the end.
Every operator except the last one in block must end with ";", the last don't have to end with ";".

Some words about vars. My language have only number variables with fixed accuracy (two digits).
Global vars that will be used in function must be defined before function declaration and its name mustn't be the same to local vars (or undefined behavior).
Global vars that will not be used in functions can have any name.

For example, recursive factorial function:
```
esh fact(x)
    bashlau
        egyar (x <= 1)
            bashlau
                bir 1
            tuktau;
        bylmasa
            bashlau
                bir x * fact(x - 1)
            tuktau
    tuktau;

x = 0;

ukyrga(x);
ans = fact(x);
yozyrga(ans)
```
It translated to syntax tree with using recursive descent:

![tree_dump2](https://github.com/esgalimov/language/assets/74832034/408ef847-f4b6-4eff-acf4-5d04c8bde35d)

Then it converted to AST written preorder format:
```
(26(26(26(26(14:fact(13:x)(18(9(13:x)(0:1))(26(16(0:1))(19(16(3(13:x)(15:fact(2(13:x)(0:1)))))))))(11(13:x)(0:0)))(21(13:x)))(11(13:ans)(15:fact(13:x))))(20(13:ans)))
```

## Middleend
Middleend convert AST written preoreder to syntax tree, make some math simplifies
(like count, that could be count; +0, -0, *1, /1 anв ect) and create .asm file:
```
;Created by Emil Galimov
;2023
;Tatar programing language version 1.0

;VARS TABLE:
    ;var x - [0] loc
    ;var x - [1] gl
    ;var ans - [2] gl


   jmp :main
:fact
    pop [0+dx]
    push [0+dx]
    push 1
    ja  :logic_jmp_0
    push 1
    jmp :logic_jmp_1
    :logic_jmp_0
    push 0
    :logic_jmp_1
    push 0
    je :false_1
    push 1
    pop ax
    ret
    jmp :done_1
    :false_1
    push [0+dx]
    push [0+dx]
    push 1
    sub

    push dx
    push 3
    add
    pop dx

    call :fact
    push ax

    push dx
    push 3
    sub
    pop dx

    mul
    pop ax
    ret
    :done_1
    ret

:main
    push 0
    pop [1]
    in
    pop [1]
    push [1]

    push dx
    push 3
    add
    pop dx

    call :fact
    push ax

    push dx
    push 3
    sub
    pop dx

    pop [2]
    push [2]
    out

    hlt
```
To support local and global vars, dx stores start of variable space (increase by a number of vars when go to local space in function).
So, we can see it in asm before and after calling function. 
Local vars use this syntax - "push [num+dx]", global - push [num], where num is index in var table (we can see var table in the start of asm file like comments)

## Backend

CPU executes code generated from asm file. It have not big command set but it enough to learn:

- **hlt** - the end of asm program
- **push** - push something to stack, can be number (push 3), register (push bx) or RAM (push [2] or push [4+cx])
- **pop** - pop from stack, can be just pop, to register (pop ax) or RAM RAM (push [7] or push [2+dx])
- **add**, **sub**, **mul**, **div** - pop two nums from stack make corresponding operation and push result back
- **jmp** - use like "jmp :label" - jump into another part of program
- **je** (==), **jne** (!=), **jb** (<), **jbe** (<=), **ja** (>), **jae** (>=) - jmp if inequality or equality is true for two poped numbers (more deep in stack is left value)
- **ax**, **bx**, **cx**, **dx** - four registers
- **in** - read number from console
- **out** - pop and write poped number to console
- **call** - call func (call :label) and save next cmd index to return there (used second stack to keep ret values)
- **ret** - return to saved index
- **noroots** - write "no roots" to console (this and next are for square equation solution)
- **allnum** - write "all numbers are roots"to console
----

### Compile and run project

Use this console commands:
```
git clone https://github.com/esgalimov/language
cd language
make
./tatatlang *code path*
```
You can test language this ready examples in this repo (square equation solution - test.tatar, recursive factirial - fact.tatar):
```
./tatatlang test.tatar
```
