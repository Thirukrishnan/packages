
.model tiny  

org 100h


.data
posX      db 1 dup(0)        ; dh = posX -> controls row
posY      db 1 dup(0)        ; dl = posY -> controls column
matrix    db 26*50 dup(' ')  ; 50 lines of 26 chars each.
curr_line dw ?               ;pointer to current line
curr_char dw ?               ;pointer to current character
color     db 0*16+15         ;colors to be used in the console(background and text)



.code
program:    

    mov  curr_line, offset matrix        ;starting line of matrix
    mov  curr_char, 0  
    

start:
;CAPTURE KEY.
    mov  ah, 0
    int  16h  

;EVALUATE KEY.    
    cmp  al, 27          ; ESC
    je   fin
    cmp  ax, 4800h       ; UP.
    je   moveUp
    cmp  al,08h          ; backspace.
    je   moveLeft
    cmp  ax, 20H       ; RIGHT.
    je   moveRight
    cmp  al,0dh          ;ENTER
    je   Enter  
    
    jae  any_char        ;other characters
    jmp  start   

;DISPLAY LETTER, DIGIT OR ANY OTHER ACCEPTABLE CHAR.
any_char:
    mov  ah, 9
    mov  bh, 0
    mov  bl, color                            
    mov  cx, 1           ; how many times display char.
    int  10h             ; display char in al.

;UPDATE CHAR IN MATRIX.    
    mov  si, curr_line   ; si points to the beginning of the line.
    add  si, curr_char   ; si points to the char in the line.
    mov  [ si ], al      ; the char is in the matrix.   


;RIGHT.
moveRight:
    inc  curr_char       ; update current char.
    mov  dl, posX
    mov  dh, posY
    cmp  dl,25
    jge moveNewline
    inc  dl              ; posX ++
    mov  posX, dl
    jmp  prntCrs

;LEFT.                   ;helper label for backspace
moveLeft:
    dec  curr_char       ; update current char.
    mov  dl, posX
    mov  dh, posY
    cmp  dl, 0
    jle prntCrs
    dec  dl              ; posX --
    mov  posX, dl 
    jmp  backspace

;UP.
moveUp: 
    sub  curr_line, 10   ; update current line.
    mov  dl, posX
    mov  dh, posY
    cmp  dh,0
    jle  prntCrs
    dec  dh              ; posY -- 
    mov  posY, dh
    jmp  prntCrs         ; print cursor

;ENTER.
Enter:   
    add  curr_line, 10   ;move to next line
    mov posX, 0          ;move to first index of next line
    mov  dl, posX        
    mov  dh, posY
    cmp  dh,50
    jge  prntCrs
    inc  dh              ; posY ++
    mov  posY, dh
    jmp  prntCrs        

   
;Newline
moveNewline:
    add curr_line,10
    mov posX,0
    mov dl,posX
    mov dh,posY
    inc dh
    mov posY,dh
    jmp prntCrs

;backspace
backspace: 
    mov  ah, 2h
    int  10h
    mov dl, 20h         ; A space to clear old character 
    int 21h             ; Display it  
    mov dl, 08h         ; Another backspace character to move cursor back again
    int 21h        
    jmp start
        

prntCrs:                 ; print cursor
    mov  ah, 2h
    int  10h
    jmp  start           


fin:
    int  20h             ;to close/halt the console


end program                                                                                                 ;TEAM MEMBERS
                                                                                                            
                                                                                                            ;Rambabu S               (19pd26)
                                                                                                            ;Thiruvenkata krishnan B (19pd39)  