extern void PRINT(const char *s);
extern void uart_send(unsigned char);
extern void timer_setup(unsigned int);
extern void app_main(void);

void app_main(void) {
    while (1) {
        for (int i = 0; i < 10; i++) {
            uart_send('0' + i);
            uart_send('\n');
        }
    }

    PRINT("Hello World\n");
}
