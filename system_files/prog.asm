;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var x - [0]



    push 3
    push 2
    jb  :logic_jmp_0
    push 1
    jmp :logic_jmp_1
    :logic_jmp_0
    push 0
    :logic_jmp_1
    pop [0]
    push 4
    push 5
    add
    push 7
    push 8
    push 2
    add
    mul
    jae :logic_jmp_2
    push 1
    jmp :logic_jmp_3
    :logic_jmp_2
    push 0
    :logic_jmp_3
    push 0
    je :if_0
    in
    pop [0]
    push [0]
    out
    :if_0
    hlt
