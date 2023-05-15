;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var x - [0]
    ;var y - [1]
    ;var z - [2]
    ;var s - [3]



    jmp :jmp_over_one
    :one
    pop [0]
    pop [1]
    pop [2]
    push [0]
    push [1]
    add
    push [2]
    add
    push 5
    add
    pop ax
    ret
    ret
    :jmp_over_one
    jmp :jmp_over_two
    :two
    pop [3]
    push [3]
    push 1
    push 2
    push 5
    call :one
    push ax
    sub
    pop ax
    ret
    ret
    :jmp_over_two
    push 1
    call :two
    push ax
    pop [2]
    push [2]
    out
    hlt
