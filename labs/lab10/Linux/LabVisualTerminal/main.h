#ifndef ZOMBIE_BRIDGE_H
#define ZOMBIE_BRIDGE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

// Constantes
#define MAX_BRIDGE_CAPACITY 4
#define MAX_STUDENTS 10
#define ANIMATION_DELAY 150000
#define MAX_NAME_LENGTH 20
#define MAX_WAIT_ALLOWED 10000 // Si se cambia eso puede pasar mas rapido a convertidos

// Colores y elementos visuales
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_RESET   "\033[0m"
#define ZOMBIE1       "🧟♂️"
#define ZOMBIE2       "🧟♀️"
#define STUDENT_RUN   "🏃"
#define STUDENT_WALK  "🚶"
#define BUILDING      "🏢"
#define BRIDGE_ICON   "🌉"
#define EXIT_SIGN     "🚪"
#define BOOK_ICON     "📚"
#define WINDOW_ICON   "🪟"
#define CAR_ICON      "🚗"
#define TREE_ICON     "🌳"
#define SUN_ICON      "☀️"
#define BRIDGE_SUPPORT "░░"

// Estructura de datos
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    int direction; // 0: Derecha, 1: Izquierda
    int position;
    long arrival_time;
    long start_cross_time;
    long end_cross_time;
} Student;

// Variables globales (extern)
extern pthread_mutex_t bridge_mutex;
extern pthread_cond_t bridge_cond;
extern int current_direction;
extern int students_on_bridge;
extern Student* bridge[MAX_BRIDGE_CAPACITY];
extern int waiting_students[2];
extern long oldest_waiting_time[2];
extern pthread_t students[MAX_STUDENTS];
extern Student student_data[MAX_STUDENTS];
extern bool simulation_running;
extern const char* student_names[];

// Prototipos de funciones
void clear_screen();
void move_cursor(int x, int y);
long get_current_time();
void draw_university();
void draw_safe_exit();
void draw_bridge();
void draw_zombies();
void draw_scene();
void log_activity(const char* message);
void check_starvation();
void access_bridge(Student* s);
void exit_bridge(Student* s);
int check_starvation_condition(Student* s);
void* student_thread(void* arg);
void show_summary();
void draw_intro_screen();

#endif // ZOMBIE_BRIDGE_H