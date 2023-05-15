;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var x - [0]
    ;var ans - [1]
    ;var i - [2]



    jmp :jmp_over_fact
    :fact
        pop [0]
    push 1
    pop [1]
    push 1
    pop [2]
    push [1]
    out

    :while_0
    push [2]
    push [0]
    sub
    push 0
    je :while_1
    push [2]
    push 1
    add
    pop [2]
    push [1]
    push [2]
    mul
    pop [1]
    push [1]
    out

    jmp :while_0
    :while_1

        ret

    :jmp_over_fact
    push 6
    call :fact

    hlt
