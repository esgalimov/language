;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var no_roots - [0]
    ;var one_root - [1]
    ;var two_roots - [2]
    ;var all_nums - [3]
    ;var x1 - [4]
    ;var x2 - [5]
    ;var k_b - [6]
    ;var k_c - [7]
    ;var ko_a - [8]
    ;var ko_b - [9]
    ;var ko_c - [10]
    ;var a - [11]
    ;var b - [12]
    ;var c - [13]
    ;var lin_answer - [14]



    push 0
    pop [0]
    push 1
    pop [1]
    push 2
    pop [2]
    push 1
    pop [3]
    push 0
    pop [4]
    push 0
    pop [5]
    jmp :jmp_over_solvingLin
    :solvingLin
    pop [6]
    pop [7]
    push [6]
    push 0
    je :if_0
    push [7]
    push [6]
    div
    pop [4]
    push [1]
    pop ax
    ret
    :if_0
    push [6]
    push 0
    jne :else_0
    push [7]
    push 0
    je :if_1
    push [0]
    pop ax
    ret
    :if_1
    push [7]
    push 0
    jne :else_0
    push [3]
    pop ax
    ret
    :else_0
    :else_1
    ret
    :jmp_over_solvingLin
    jmp :jmp_over_solvingSq
    :solvingSq
    pop [8]
    pop [9]
    pop [10]
    push [8]
    pop [4]
    push [9]
    pop [5]
    push [2]
    pop ax
    ret
    ret
    :jmp_over_solvingSq
    push 0
    pop [11]
    push 0
    pop [12]
    push 0
    pop [13]
    in
    pop [11]
    in
    pop [12]
    in
    pop [13]
    push [11]
    out
    push [12]
    out
    push [13]
    out
    push [11]
    push 0
    je :if_2
    push [13]
    push [12]
    push [11]
    call :solvingSq
    push ax
    push [4]
    out
    push [5]
    out
    :if_2
    push [11]
    push 0
    jne :else_2
    push [13]
    push [12]
    call :solvingLin
    push ax
    pop [14]
    push [4]
    out
    :else_2
    hlt
