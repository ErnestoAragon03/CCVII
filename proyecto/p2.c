extern void uart_send(unsigned char);

void main_p2(void) {
    while(1){
        for(int i=0; i < 26; i++){
            uart_send('0' + i + 17);
            uart_send('\n');

            for(int j=0; j < 100000; j++); // Esperar un poco
        }
    }
}
