
extern void uart_send(unsigned char);

void main_p3(void) {
    while(1){
        
        uart_send('a');
        uart_send('\n');
        uart_send('b');
        uart_send('\n');
        uart_send('c');
        uart_send('\n');
        uart_send('d');
        uart_send('\n');
        uart_send('e');
        uart_send('\n');
        uart_send('f');
        uart_send('\n');
        uart_send('g');
        uart_send('\n');
        uart_send('h');
        uart_send('\n');
        uart_send('i');
        uart_send('\n');
        uart_send('j');
        uart_send('\n');
        uart_send('k');
        uart_send('\n');
        uart_send('l');
        uart_send('\n');
        uart_send('m');
        uart_send('\n');
        uart_send('n');
        uart_send('\n');
        uart_send('o');
        uart_send('\n');
        uart_send('p');
        uart_send('\n');
        uart_send('q');
        uart_send('\n');
        uart_send('r');
        uart_send('\n');
        uart_send('s');
        uart_send('\n');
        uart_send('t');
        uart_send('\n');
        uart_send('u');
        uart_send('\n');
        uart_send('v');
        uart_send('\n');
        uart_send('w');
        uart_send('\n');
        uart_send('x');
        uart_send('\n');
        uart_send('y');
        uart_send('\n');
        uart_send('z');
        uart_send('\n');
        for (int j=0; j < 10000000; j++);
    }
}
