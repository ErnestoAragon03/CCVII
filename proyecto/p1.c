extern void uart_send(unsigned char);

void main_p1(void) {
    while(1){
        for(int i=0; i < 10; i++){
            uart_send('0' + i);
            uart_send('\n');

            for (int j=0; j < 100000; j++); // Esperar un poco
        }
    }
}
