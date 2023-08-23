;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var x - [0]
    ;var ans - [1]



    push 3
    pop [0+dx]
    jmp :jmp_over_fact
    :fact
    pop [0+dx]
    push [0+dx]
    out
    push [0+dx]
    push 1
    ja  :logic_jmp_0
    push 1
    jmp :logic_jmp_1
    :logic_jmp_0
    push 0
    :logic_jmp_1
    push 0
    je :if_0
    push 1
    pop ax
    ret
    :if_0
    push [0+dx]
    push 1
    ja  :logic_jmp_2
    push 1
    jmp :logic_jmp_3
    :logic_jmp_2
    push 0
    :logic_jmp_3
    push 0
    jne :else_0
    push [0+dx]
    push [0+dx]
    push 1
    sub

    push dx
    push 2
    add
    pop dx

    call :fact
    push ax

    push dx
    push 2
    sub
    pop dx

    mul
    pop ax
    ret
    :else_0
    ret
    :jmp_over_fact
    push 7
    push 1
    sub

    push dx
    push 2
    add
    pop dx

    call :fact
    push ax

    push dx
    push 2
    sub
    pop dx

    pop [1+dx]
    push [1+dx]
    out
    hlt
