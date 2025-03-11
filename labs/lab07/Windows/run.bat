echo "Limpiando archivos anteriores..."
del *.exe
echo "Compilando archivos..."
gcc main_windows.c log_processor.c -o main_windows
echo "Corriendo programa..."
call main_windows.exe access.log