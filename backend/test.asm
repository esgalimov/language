in
in
in

pop cx
pop bx
pop ax

push 0
push ax

je :0

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

je :1

push 0
push dx

ja :2

push 0
push dx

jb :3

hlt

    :0
    push 0
    push bx

    je :4

    push -1
    push cx
    mul

    push bx

    div

    out
    hlt

    :4
    push 0
    push cx

    je :5

    noroots
    hlt

    :5
    allnum
    hlt

    :1
    push -1
    push bx
    mul

    push 2
    push ax
    mul

    div

    out
    hlt

    :2
    noroots
    hlt

    :3
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
