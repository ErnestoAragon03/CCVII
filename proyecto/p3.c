extern void uart_send(unsigned char);

void main_p3(void) {
    while(1){
        for(int i=0; i < 26; i++){
            uart_send('0' + i + 49);
            uart_send('\n');

            for(int j=0; j < 10000000; j++); // Esperar un poco
        }
    }
}
