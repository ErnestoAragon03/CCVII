echo "🧹 Limpiando..."

# Limpiar archivos de nivel raíz (si los hubiera)
rm -f *.o *.elf *.bin *.list

# Limpiar por carpeta
rm -f os/*.o os/*.elf os/*.bin os/*.list
rm -f prog1/*.o prog1/*.elf prog1/*.bin prog1/*.list
rm -f prog2/*.o prog2/*.elf prog2/*.bin prog2/*.list
