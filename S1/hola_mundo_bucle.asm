pila segment stack 'stack'
    dw 100h dup (?)
pila ends

datos segment 'data'
    msg db 'hola mundo$' 
datos ends

codigo segment 'code'
    assume cs:codigo, ds:datos, ss:pila

main PROC
    ; Configurar el segmento de datos
    mov ax, datos
    mov ds, ax

    ; Inicializar el contador en 0
    mov cx, 0

bucle:
    ; Mostrar el mensaje
    mov dx, OFFSET msg
    mov ah, 9
    int 21h

    ; Incrementar el contador
    inc cx

    ; Comprobar si se ha mostrado 7 veces
    cmp cx, 7
    jne bucle

    ; Terminar el programa
    mov ax, 4C00h
    int 21h

main ENDP
codigo ends

END main
