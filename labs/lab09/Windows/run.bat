echo "Limpiando archivos anteriores..."
del *.exe
echo "Compilando archivos..."
gcc -o main.exe main.c -lpthread
echo "Corriendo programa"
call main.exe