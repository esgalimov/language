;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var x - [0]
    ;var y - [1]
    ;var z - [2]
    ;var t - [3]



    push 0
    pop [0]
    push 1
    pop [1]
    push 2
    pop [2]
    push [0]
    push [1]
    add
    push [2]
    add
    pop [3]
    push [3]
    push 0
    je :if_0
    push [3]
    out

    :if_0

    hlt
