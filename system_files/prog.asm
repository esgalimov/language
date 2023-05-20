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
    ;var num - [6]
    ;var xn - [7]
    ;var i - [8]
    ;var k_b - [9]
    ;var k_c - [10]
    ;var ko_a - [11]
    ;var ko_b - [12]
    ;var ko_c - [13]
    ;var D - [14]
    ;var check - [15]
    ;var a - [16]
    ;var b - [17]
    ;var c - [18]
    ;var sqanswer - [19]
    ;var lin_answer - [20]



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
    jmp :jmp_over_Sqrt
    :Sqrt
    pop [6]
    push 1
    pop [7]
    push 0
    pop [8]
    :while_0
    push [8]
    push 20
    sub
    push 0
    je :while_1
    push 0.5
    push [7]
    push [6]
    push [7]
    div
    add
    mul
    pop [7]
    push [8]
    push 1
    add
    pop [8]
    jmp :while_0
    :while_1
    push [7]
    pop ax
    ret
    ret
    :jmp_over_Sqrt
    jmp :jmp_over_solvingLin
    :solvingLin
    pop [9]
    pop [10]
    push [9]
    push 0
    je :if_0
    push [10]
    push [9]
    div
    pop [4]
    push [1]
    pop ax
    ret
    :if_0
    push [9]
    push 0
    jne :else_0
    push [10]
    push 0
    je :if_1
    push [0]
    pop ax
    ret
    :if_1
    push [10]
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
    pop [11]
    pop [12]
    pop [13]
    push [12]
    push [12]
    mul
    push 4
    push [11]
    mul
    push [13]
    mul
    sub
    pop [14]
    push [14]
    push 0
    je :if_2
    push [14]
    push [14]
    call :Sqrt
    push ax
    push [14]
    call :Sqrt
    push ax
    mul
    sub
    pop [15]
    push [15]
    push 0
    je :if_2
    push [0]
    pop ax
    ret
    :if_2
    push [15]
    push 0
    jne :else_2
    push [14]
    call :Sqrt
    push ax
    add
    push 2
    push [11]
    mul
    div
    pop [4]
    push [14]
    call :Sqrt
    push ax
    sub
    push 2
    push [11]
    mul
    div
    pop [5]
    push [2]
    pop ax
    ret
    :else_2
    :if_3
    push [14]
    push 0
    jne :else_3
    push 2
    push [11]
    mul
    div
    pop [4]
    push [1]
    pop ax
    ret
    :else_3
    push [2]
    pop ax
    ret
    ret
    :jmp_over_solvingSq
    push 0
    pop [16]
    push 0
    pop [17]
    push 0
    pop [18]
    in
    pop [16]
    in
    pop [17]
    in
    pop [18]
    push [16]
    push 0
    je :if_4
    push [18]
    push [17]
    push [16]
    call :solvingSq
    push ax
    pop [19]
    push [4]
    out
    push [5]
    out
    :if_4
    push [16]
    push 0
    jne :else_4
    push [18]
    push [17]
    call :solvingLin
    push ax
    pop [20]
    push [4]
    out
    :else_4
    hlt
