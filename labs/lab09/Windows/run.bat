echo "Limpiando archivos anteriores..."
del *.exe
echo "Compilando archivos..."
gcc main.c -o main
echo "Corriendo programa"
call main.exe