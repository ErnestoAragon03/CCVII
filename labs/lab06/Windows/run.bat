echo "Limpiando archivos anteriores..."
del *.exe
echo "Compilando archivos..."
gcc main_windows.c -o main_windows
echo "Corriendo programa con los archivos file1 file2 y file3 ..."
call main_windows.exe file1.txt file2.txt file3.txt