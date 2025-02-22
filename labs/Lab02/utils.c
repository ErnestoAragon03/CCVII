// Convertir entero a cadena
void uart_itoa(int num, char *buffer) {
    int i = 0, is_negative = 0;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Invertir cadena
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }
}


// Función para convertir una cadena a un número flotante
float uart_atof(const char *s) {
    float num = 0.0;
    float fraction = 1.0;
    int sign = 1;
    int i = 0;

    // Manejar el signo opcional
    if (s[i] == '-') {
        sign = -1;
        i++;
    }

    // Parte entera
    for (; s[i] >= '0' && s[i] <= '9'; i++) {
        num = num * 10.0 + (s[i] - '0');
    }

    // Parte fraccionaria
    if (s[i] == '.') {
        i++;
        for (; s[i] >= '0' && s[i] <= '9'; i++) {
            num = num * 10.0 + (s[i] - '0');
            fraction *= 10.0;
        }
    }

    return sign * num / fraction;
}

// Función para convertir un número flotante a una cadena
void uart_ftoa(float num, char *buffer) {
    int i = 0;
    int is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Parte entera
    int int_part = (int)num;
    float frac_part = num - int_part;

    // Convertir la parte entera
    char int_buffer[16];
    uart_itoa(int_part, int_buffer);
    for (int j = 0; int_buffer[j] != '\0'; j++) {
        buffer[i++] = int_buffer[j];
    }

    // Punto decimal
    buffer[i++] = '.';

    // Parte fraccionaria (hasta 6 dígitos para mayor precisión)
    for (int j = 0; j < 6; j++) {
        frac_part *= 10.0;
        int digit = (int)frac_part;
        buffer[i++] = '0' + digit;
        frac_part -= digit;
    }

    buffer[i] = '\0';
}
// Convertir cadena a entero escalado  
// 12.34 --> 1234
int uart_scaled_atoi(const char *s, int scale) {
    int num = 0, frac = 0, sign = 1, i = 0, frac_digits = 0;
    int is_fraction = 0;

    if (s[i] == '-') {
        sign = -1;
        i++;
    }

    for (; s[i] != '\0'; i++) {
        if (s[i] == '.') {
            is_fraction = 1;
            continue;
        }

        if (s[i] >= '0' && s[i] <= '9') {
            if (is_fraction && frac_digits < scale) {
                frac = frac * 10 + (s[i] - '0');
                frac_digits++;
            } else if (!is_fraction) {
                num = num * 10 + (s[i] - '0');
            }
        }
    }

    while (frac_digits++ < scale) {
        frac *= 10;
    }

    return sign * (num * scale + frac);
}


// Convertir entero escalado a cadena con punto flotante 
//1234 --> 12.34
void uart_scaled_itoa(int num, char *buffer, int scale) {
    int is_negative = 0;
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    int int_part = num / scale;
    int frac_part = num % scale;

    char int_buffer[16];
    uart_itoa(int_part, int_buffer);

    int i = 0;
    if (is_negative) {
        buffer[i++] = '-';
    }

    // Copiar parte entera
    for (int j = 0; int_buffer[j] != '\0'; j++) {
        buffer[i++] = int_buffer[j];
    }

    buffer[i++] = '.';

    // Parte fraccionaria
    for (int j = 0; j < 2; j++) {
        frac_part *= 10;
        buffer[i++] = '0' + (frac_part / scale);
        frac_part %= scale;
    }

    buffer[i] = '\0';
}


// Convertir una cadena a un entero
int uart_atoi(const char *s) {
    int num = 0;
    int sign = 1;
    int i = 0;

    if (s[i] == '-') {
        sign = -1;
        i++;
    }

    for (; s[i] >= '0' && s[i] <= '9'; i++) {
        num = num * 10 + (s[i] - '0');
    }

    return sign * num;
}