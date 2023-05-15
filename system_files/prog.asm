;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var x - [0]



    push 0
    pop [0]
    jmp :jmp_over_one
    :one
        pop [0]
    push 1
    pop ax
    ret
        ret
    :jmp_over_one
    jmp :jmp_over_two
    :two
        pop [0]
    push 2
    pop ax
    ret
        ret
    :jmp_over_two
    push 1
    call :one
    push ax
    push 2
    call :two
    push ax
    add
    pop [0]
    push [0]
    out
    hlt
