echo "Limpiando archivos anteriores..."
del *.exe transactions.log
echo "Compilando archivos..."
gcc main.c atm.c -o main
echo "Corriendo programa"
call main.exe