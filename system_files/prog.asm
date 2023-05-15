;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var x - [0]



    jmp :jmp_over_one
    :one
    push 0
    pop [0]
    push [0]
    out
    ret
    :jmp_over_one
    call :one
    push ax
    hlt
