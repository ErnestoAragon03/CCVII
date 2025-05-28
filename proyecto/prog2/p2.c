// p2.c
extern void uart_send(unsigned char);

void main(void) {
    while(1){
        for(int i=0; i < 26; i++){
            uart_send('a' + i);  // ya no suma 17, imprime letras reales
            uart_send('\n');
        }
    }
}
