echo "Limpiando archivos anteriores..."
del *.exe
echo "Compilando archivos..."
gcc -o main.exe main.c -lpthread
cls
call main.exe