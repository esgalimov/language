;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var x1 - [0] gl
    ;var x2 - [1] gl
    ;var all_nums - [2] gl
    ;var no_roots - [3] gl
    ;var one_root - [4] gl
    ;var two_roots - [5] gl
    ;var num - [6] loc
    ;var xn - [7] loc
    ;var i - [8] loc
    ;var b - [9] loc
    ;var c - [10] loc
    ;var a - [11] loc
    ;var D - [12] loc
    ;var a - [13] gl
    ;var b - [14] gl
    ;var c - [15] gl
    ;var roots - [16] gl



    push 0
    pop [0]
    push 0
    pop [1]
    push -1
    pop [2]
    push 0
    pop [3]
    push 1
    pop [4]
    push 2
    pop [5]
    jmp :jmp_over_Sqrt
:Sqrt
    pop [6+dx]
    push 1
    pop [7+dx]
    push 0
    pop [8+dx]
    :while_0
    push [8+dx]
    push 20
    sub
    push 0
    je :while_1
    push 0.5
    push [7+dx]
    push [6+dx]
    push [7+dx]
    div
    add
    mul
    pop [7+dx]
    push [8+dx]
    push 1
    add
    pop [8+dx]
    jmp :while_0
    :while_1
    push [7+dx]
    pop ax
    ret

    ret
:jmp_over_Sqrt
    jmp :jmp_over_SolveLinear
:SolveLinear
    pop [9+dx]
    pop [10+dx]
    push [9+dx]
    push 0
    jne :logic_jmp_0
    push 1
    jmp :logic_jmp_1
    :logic_jmp_0
    push 0
    :logic_jmp_1
    push 0
    je :false_1
    push [10+dx]
    push 0
    jne :logic_jmp_2
    push 1
    jmp :logic_jmp_3
    :logic_jmp_2
    push 0
    :logic_jmp_3
    push 0
    je :false_2
    push [2]
    pop ax
    ret

    jmp :done_2
:false_2
    push [3]
    pop ax
    ret

:done_2
    jmp :done_1
:false_1
    push -1
    push [10+dx]
    mul
    push [9+dx]
    div
    pop [0]
    push [4]
    pop ax
    ret

:done_1
    ret
:jmp_over_SolveLinear
    jmp :jmp_over_SolveSquare
:SolveSquare
    pop [11+dx]
    pop [9+dx]
    pop [10+dx]
    push [9+dx]
    push [9+dx]
    mul
    push 4
    push [11+dx]
    mul
    push [10+dx]
    mul
    sub
    pop [12+dx]
    push [12+dx]
    push 0
    jae :logic_jmp_4
    push 1
    jmp :logic_jmp_5
    :logic_jmp_4
    push 0
    :logic_jmp_5
    push 0
    je :false_3
    push [3]
    pop ax
    ret

    jmp :done_3
:false_3
    push [12+dx]
    push 0
    jne :logic_jmp_6
    push 1
    jmp :logic_jmp_7
    :logic_jmp_6
    push 0
    :logic_jmp_7
    push 0
    je :false_4
    push -1
    push [9+dx]
    mul
    push 2
    div
    push [11+dx]
    mul
    pop [0]
    push [4]
    pop ax
    ret

    jmp :done_4
:false_4
    push -1
    push [9+dx]
    mul
    push [12+dx]

    push dx
    push 17
    add
    pop dx

    call :Sqrt
    push ax

    push dx
    push 17
    sub
    pop dx

    add
    push 2
    push [11+dx]
    mul
    div
    pop [0]
    push -1
    push [9+dx]
    mul
    push [12+dx]

    push dx
    push 17
    add
    pop dx

    call :Sqrt
    push ax

    push dx
    push 17
    sub
    pop dx

    sub
    push 2
    push [11+dx]
    mul
    div
    pop [1]
    push [5]
    pop ax
    ret

:done_4
:done_3
    ret
:jmp_over_SolveSquare
    push 0
    pop [13]
    push 0
    pop [14]
    push 0
    pop [15]
    push 0
    pop [16]
    in
    pop [13]
    in
    pop [14]
    in
    pop [15]
    push [13]
    push 0
    jne :logic_jmp_8
    push 1
    jmp :logic_jmp_9
    :logic_jmp_8
    push 0
    :logic_jmp_9
    push 0
    je :false_5
    push [15]
    push [14]

    push dx
    push 17
    add
    pop dx

    call :SolveLinear
    push ax

    push dx
    push 17
    sub
    pop dx

    pop [16]
    push [16]
    push [2]
    jne :logic_jmp_10
    push 1
    jmp :logic_jmp_11
    :logic_jmp_10
    push 0
    :logic_jmp_11
    push 0
    je :false_6
    allnum
    out
    jmp :done_6
:false_6
    push [16]
    push [3]
    jne :logic_jmp_12
    push 1
    jmp :logic_jmp_13
    :logic_jmp_12
    push 0
    :logic_jmp_13
    push 0
    je :false_7
    noroots
    out
    jmp :done_7
:false_7
    push [0]
    out
:done_7
:done_6
    jmp :done_5
:false_5
    push [15]
    push [14]
    push [13]

    push dx
    push 17
    add
    pop dx

    call :SolveSquare
    push ax

    push dx
    push 17
    sub
    pop dx

    pop [16]
    push [16]
    push [3]
    jne :logic_jmp_14
    push 1
    jmp :logic_jmp_15
    :logic_jmp_14
    push 0
    :logic_jmp_15
    push 0
    je :false_8
    noroots
    out
    jmp :done_8
:false_8
    push [16]
    push [4]
    jne :logic_jmp_16
    push 1
    jmp :logic_jmp_17
    :logic_jmp_16
    push 0
    :logic_jmp_17
    push 0
    je :false_9
    push [0]
    out
    jmp :done_9
:false_9
    push [0]
    out
    push [1]
    out
:done_9
:done_8
:done_5
    hlt
