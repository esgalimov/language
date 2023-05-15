;Created by Emil Galimov
;2023
;Tatar programming language version 1.0

;VARS TABLE:
    ;var x - [0]
    ;var y - [1]
    ;var z - [2]



    push 0
    pop [0]
    push 12
    pop [1]
    push 47
    pop [2]
    in
    pop [0]
    push [0]
    push 0
    je :if_0
    push [1]
    out
    :if_0
    push [0]
    push 0
    jne :else_0
    push [2]
    out
    :else_0
    hlt
