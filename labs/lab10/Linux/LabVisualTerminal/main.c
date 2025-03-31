#include "main.h"

// Variables globales
pthread_mutex_t bridge_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bridge_cond = PTHREAD_COND_INITIALIZER;
int current_direction = -1;
int students_on_bridge = 0;
Student* bridge[MAX_BRIDGE_CAPACITY] = {NULL};
int waiting_students[2] = {0, 0};
long oldest_waiting_time[2] = {0, 0};
pthread_t students[MAX_STUDENTS];
Student student_data[MAX_STUDENTS];
bool simulation_running = true;

const char* student_names[] = {
    "Inge 01", "Inge 02", "Inge 03", "Inge 04", "Inge 05",
    "Inge 06", "Inge 07", "Inge 08", "Inge 09", "Inge 10"
};

void clear_screen() {
    printf("\033[H\033[J");
}

void move_cursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

long get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void draw_university() {
    move_cursor(5, 3);
    printf(COLOR_BLUE "╔════════════════════════╗\n");
    move_cursor(5, 4);
    printf(COLOR_BLUE "║" COLOR_CYAN "   UNIVERSIDAD Galileo   " COLOR_BLUE "║\n");
    move_cursor(5, 5);
    printf(COLOR_BLUE "║  🏫 " BUILDING "  " BOOK_ICON "  ║\n");
    move_cursor(5, 6);
    printf(COLOR_BLUE "║                      ║\n");
    move_cursor(5, 7);
    printf(COLOR_BLUE "║  " WINDOW_ICON "  " WINDOW_ICON "  " WINDOW_ICON "  " WINDOW_ICON "  ║\n");
    move_cursor(5, 8);
    printf(COLOR_BLUE "╚════════════════════════╝" COLOR_RESET);
}

void draw_safe_exit() {
    move_cursor(70, 3);
    printf(COLOR_GREEN "╔══════════════════════╗\n");
    move_cursor(70, 4);
    printf(COLOR_GREEN "║" COLOR_YELLOW "   ZONA SEGURA    " COLOR_GREEN "║\n");
    move_cursor(70, 5);
    printf(COLOR_GREEN "║  🏠 " EXIT_SIGN " 🛡️  ║\n");
    move_cursor(70, 6);
    printf(COLOR_GREEN "║                      ║\n");
    move_cursor(70, 7);
    printf(COLOR_GREEN "║  " SUN_ICON "   " TREE_ICON "   " CAR_ICON "   ║\n");
    move_cursor(70, 8);
    printf(COLOR_GREEN "╚══════════════════════╝" COLOR_RESET);
}

void draw_bridge() {
    static int animation_offset = 0;
    animation_offset = (animation_offset + 1) % 4;
    
    move_cursor(25, 10);
    printf(COLOR_YELLOW "┌────────────────────────────────────────────┐\n");
    
    move_cursor(25, 11);
    printf(COLOR_YELLOW "│" COLOR_RESET);
    for(int i = 0; i < MAX_BRIDGE_CAPACITY; i++) {
        if(bridge[i]) {
            if(animation_offset < 2) {
                printf(" %s%-6s", STUDENT_RUN, bridge[i]->name);
            } else {
                printf(" %s%-6s", STUDENT_WALK, bridge[i]->name);
            }
            printf(bridge[i]->direction == 0 ? "→" : "←");
        } else {
            printf("       ");
        }
    }
    printf(COLOR_YELLOW "│\n");
    
    move_cursor(25, 12);
    printf(COLOR_YELLOW "│                                            │\n");
    move_cursor(25, 13);
    printf(COLOR_YELLOW "└────────────────────────────────────────────┘" COLOR_RESET);
    
    move_cursor(25, 14);
    printf(COLOR_YELLOW "  " BRIDGE_SUPPORT "  " BRIDGE_SUPPORT "  " BRIDGE_SUPPORT "  " BRIDGE_SUPPORT "  " BRIDGE_SUPPORT COLOR_RESET);
}

void draw_zombies() {
    static bool zombie_frame = false;
    static int zombie_pos = 20;
    
    zombie_pos = 20 + (rand() % 5);
    
    move_cursor(zombie_pos, 15);
    printf(COLOR_RED);
    for(int i = 0; i < 12; i++) {
        printf(zombie_frame ? ZOMBIE1 : ZOMBIE2);
    }
    printf(COLOR_RESET);
    
    if(students_on_bridge > MAX_BRIDGE_CAPACITY) {
        move_cursor(30, 16);
        printf(COLOR_RED "¡¡¡OH NO! ¡Demasiados estudiantes! ¡Zombies atrapan a alguien! " ZOMBIE1 COLOR_RESET);
    }
    
    zombie_frame = !zombie_frame;
}

void draw_scene() {
    clear_screen();
    
    move_cursor(30, 1);
    printf(COLOR_RED "▓▓▓ ¡PUENTE ZOMBIE! ▓▓▓" COLOR_RESET);
    
    draw_university();
    draw_safe_exit();
    draw_bridge();
    draw_zombies();
    
    move_cursor(5, 17);
    printf(COLOR_CYAN "🏗️  Capacidad: %d/%d ", students_on_bridge, MAX_BRIDGE_CAPACITY);
    move_cursor(5, 18);
    printf("🧭 Dirección: ");
    if(current_direction == 0) printf(COLOR_BLUE "UNIVERSIDAD → SALIDA " BUILDING);
    else if(current_direction == 1) printf(COLOR_GREEN "SALIDA ← UNIVERSIDAD " EXIT_SIGN);
    else printf("SIN DIRECCIÓN");
    
    move_cursor(5, 19);
    printf(COLOR_YELLOW "🕴️  Esperando UMB: %d ", waiting_students[0]);
    move_cursor(5, 20);
    printf(COLOR_YELLOW "🕴️  Esperando Salida: %d ", waiting_students[1]);
    
    move_cursor(5, 22);
    printf(COLOR_MAGENTA "📜 Eventos recientes:");
    fflush(stdout);
}

void log_activity(const char* message) {
    static int log_line = 23;
    static int log_count = 0;
    
    if(log_count > 5) {
        for(int i = 23; i <= 28; i++) {
            move_cursor(5, i);
            printf("                                             ");
        }
        log_count = 0;
        log_line = 23;
    }
    
    move_cursor(5, log_line++);
    printf(COLOR_RESET "• %s", message);
    log_count++;
    fflush(stdout);
}

void check_starvation() {
    long current_time = get_current_time();
    for (int dir = 0; dir < 2; dir++) {
        if (waiting_students[dir] > 0 && oldest_waiting_time[dir] != 0 && 
            (current_time - oldest_waiting_time[dir]) > MAX_WAIT_ALLOWED) {
            if (students_on_bridge == 0) {
                current_direction = dir;
                oldest_waiting_time[dir] = 0;
                
                int opposite_dir = 1 - dir;
                if (waiting_students[opposite_dir] > 0 && oldest_waiting_time[opposite_dir] == 0) {
                    oldest_waiting_time[opposite_dir] = current_time;
                }
                
                pthread_cond_broadcast(&bridge_cond);
                char log_msg[100];
                snprintf(log_msg, sizeof(log_msg), "¡Prevención de inanición! Cambiando dirección a %s", 
                         dir == 0 ? "Derecha" : "Izquierda");
                log_activity(log_msg);
            }
            break;
        }
    }
}

int check_starvation_condition(Student* s) {
    long current_time = get_current_time();
    int opposite_dir = 1 - s->direction;
    
    return (waiting_students[opposite_dir] > 0 && 
            oldest_waiting_time[opposite_dir] != 0 && 
            (current_time - oldest_waiting_time[opposite_dir]) > MAX_WAIT_ALLOWED);
}

void access_bridge(Student* s) {
    pthread_mutex_lock(&bridge_mutex);
    
    waiting_students[s->direction]++;
    if (oldest_waiting_time[s->direction] == 0) {
        oldest_waiting_time[s->direction] = get_current_time();
    }
    
    char log_msg[100];
    snprintf(log_msg, sizeof(log_msg), "%s llega queriendo ir a la %s (Esperando: Izq=%d, Der=%d)", 
             s->name, s->direction == 0 ? "Derecha" : "Izquierda", 
             waiting_students[1], waiting_students[0]);
    log_activity(log_msg);
    
    draw_scene();
    
    while (students_on_bridge >= MAX_BRIDGE_CAPACITY || 
          (students_on_bridge > 0 && current_direction != s->direction) ||
          (students_on_bridge == 0 && check_starvation_condition(s))) {
        check_starvation();
        pthread_cond_wait(&bridge_cond, &bridge_mutex);
    }

    if (students_on_bridge == 0) {
        current_direction = s->direction;
        oldest_waiting_time[s->direction] = 0;
        
        int opposite_dir = 1 - s->direction;
        if (waiting_students[opposite_dir] > 0 && oldest_waiting_time[opposite_dir] == 0) {
            oldest_waiting_time[opposite_dir] = get_current_time();
        }
    }
    
    for(int i = 0; i < MAX_BRIDGE_CAPACITY; i++) {
        if(bridge[i] == NULL) {
            bridge[i] = s;
            s->position = i;
            break;
        }
    }
    
    students_on_bridge++;
    waiting_students[s->direction]--;
    s->start_cross_time = get_current_time();
    
    snprintf(log_msg, sizeof(log_msg), "%s cruza hacia la %s (en puente: %d)", 
             s->name, s->direction == 0 ? "Derecha" : "Izquierda", students_on_bridge);
    log_activity(log_msg);
    
    draw_scene();
    pthread_mutex_unlock(&bridge_mutex);
    
    usleep(1000000 + (rand() % 2000000));
}

void exit_bridge(Student* s) {
    pthread_mutex_lock(&bridge_mutex);
    
    for(int i = 0; i < MAX_BRIDGE_CAPACITY; i++) {
        if(bridge[i] == s) {
            bridge[i] = NULL;
            break;
        }
    }
    
    s->position = -1;
    students_on_bridge--;
    s->end_cross_time = get_current_time();
    
    char log_msg[100];
    snprintf(log_msg, sizeof(log_msg), "%s sale del puente (en puente: %d, Esperando: Izq=%d, Der=%d)", 
             s->name, students_on_bridge, waiting_students[1], waiting_students[0]);
    log_activity(log_msg);
    
    pthread_cond_broadcast(&bridge_cond);
    
    draw_scene();
    pthread_mutex_unlock(&bridge_mutex);
}

void* student_thread(void* arg) {
    Student* s = (Student*)arg;
    
    usleep(rand() % 5000000);
    s->arrival_time = get_current_time();
    
    access_bridge(s);
    exit_bridge(s);
    
    return NULL;
}

void show_summary() {
    clear_screen();
    long total_wait = 0;
    long max_wait = 0;
    int starved_students = 0;
    
    printf(COLOR_CYAN "\n╔══════════════════════════════════════════════════════╗\n");
    printf(COLOR_CYAN "║           RESUMEN FINAL DE LA SIMULACIÓN            ║\n");
    printf(COLOR_CYAN "╚══════════════════════════════════════════════════════╝\n" COLOR_RESET);
    
    for(int i = 0; i < MAX_STUDENTS; i++) {
        Student* s = &student_data[i];
        long wait = (s->start_cross_time - s->arrival_time)/1000;
        long cross = (s->end_cross_time - s->start_cross_time)/1000;
        total_wait += wait;
        
        if(wait > max_wait) max_wait = wait;
        if(wait > MAX_WAIT_ALLOWED/1000) starved_students++;
        
        printf(" %s\n", s->name);
        printf("   Dirección: %s\n", s->direction == 0 ? "Universidad → Salida" : "Salida ← Universidad");
        printf("   Tiempo espera: %lds | Tiempo cruce: %lds\n", wait, cross);
        printf(COLOR_YELLOW "──────────────────────────────────────────────────────\n" COLOR_RESET);
    }
    
    printf("\nESTADÍSTICAS:\n");
    printf("  Tiempo promedio de espera: %.2fs\n", (double)total_wait/MAX_STUDENTS);
    printf("  Tiempo máximo de espera: %lds\n", max_wait);
    printf("  Estudientes que sufrieron inanición: %d\n", starved_students);
    printf(COLOR_GREEN "\n¡Todos cruzaron seguros! 🎉 Los zombies siguen hambrientos 🧟\n" COLOR_RESET);
}

void draw_intro_screen() {
    clear_screen();
    printf(COLOR_RED "\n\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n");
    printf("    ▓▓                                                          ▓▓\n");
    printf("    ▓▓           " COLOR_RESET "SIMULACIÓN DEL PUENTE ZOMBIE" COLOR_RED "                   ▓▓\n");
    printf("    ▓▓                                                          ▓▓\n");
    printf("    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n\n" COLOR_RESET);
    
    printf("    " BUILDING " Universidad UMB                                    Zona Segura " EXIT_SIGN "\n");
    printf("    " BRIDGE_ICON " Puente de capacidad %d personas                     Zombies: " ZOMBIE1 ZOMBIE2 "\n\n", MAX_BRIDGE_CAPACITY);
    
    printf("    REGLAS DEL PUENTE:\n");
    printf("    - Solo %d personas pueden estar en el puente a la vez\n", MAX_BRIDGE_CAPACITY);
    printf("    - Todos deben ir en la misma dirección\n");
    printf("    - Si esperan más de %d segundos, se activa prevención de inanición\n\n", MAX_WAIT_ALLOWED/1000);
    
    printf("Iniciando simulación con %d estudiantes...\n\n", MAX_STUDENTS);
    printf("Preparando escenario...\n");
    sleep(3);
}

int main() {
    srand(time(NULL));
    draw_intro_screen();
    
    // Inicializar datos de estudiantes
    for(int i = 0; i < MAX_STUDENTS; i++) {
        strncpy(student_data[i].name, student_names[i], MAX_NAME_LENGTH);
        student_data[i].id = i + 1;
        student_data[i].direction = rand() % 2;
        student_data[i].position = -1;
        student_data[i].arrival_time = 0;
        student_data[i].start_cross_time = 0;
        student_data[i].end_cross_time = 0;
    }
    
    // Crear hilos de estudiantes
    for(int i = 0; i < MAX_STUDENTS; i++) {
        pthread_create(&students[i], NULL, student_thread, &student_data[i]);
    }
    
    // Bucle principal de visualización
    while(simulation_running) {
        draw_scene();
        
        // Verificar si todos han terminado
        bool all_done = true;
        for(int i = 0; i < MAX_STUDENTS; i++) {
            if(student_data[i].end_cross_time == 0) {
                all_done = false;
                break;
            }
        }
        
        if(all_done) simulation_running = false;
        
        usleep(ANIMATION_DELAY);
    }
    
    // Esperar a que todos los hilos terminen
    for(int i = 0; i < MAX_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }
    
    show_summary();
    
    return 0;
}