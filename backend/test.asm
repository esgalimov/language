    in
    push 0
    je :cock

    hlt


    :cock
    push 23
    push 35
    push 47

    push 5
    pop ax

    pop [0+ax]
    pop [1+ax]
    pop [2+ax]

    push [2+ax]
    push [1+ax]
    push [0+ax]

    out
    out
    out

    hlt
