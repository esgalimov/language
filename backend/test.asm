in
in
in

pop cx
pop bx
pop ax

push 0
push ax

je :label1

push bx
push bx
mul

push -4
push ax
push cx
mul
mul
add

pop dx

push 0
push dx

je :label2

push 0
push dx

ja :label3

push 0
push dx

jb :label4

hlt

    :label1
    push 0
    push bx

    je :label5

    push -1
    push cx
    mul

    push bx

    div

    out
    hlt

    :label5
    push 0
    push cx

    je :label6

    noroots
    hlt

    :label6
    allnum
    hlt

    :label2
    push -1
    push bx
    mul

    push 2
    push ax
    mul

    div

    out
    hlt

    :label3
    noroots
    hlt

    :label4
    push -1
    push bx
    mul

    push dx
    sqrt
    add

    push 2
    push ax
    mul

    div
    out


    push -1
    push bx
    mul

    push -1
    push dx
    sqrt
    mul
    add

    push 2
    push ax
    mul

    div
    out

    hlt

hlt
