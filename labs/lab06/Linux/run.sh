#!/bin/bash
echo "Elimina file_processor"
rm -rf file_processor
#Usamos el make para hacer la compilacion de los archvios
make


# verificamos si se cumlio
if [ $? -eq 0 ]; then
#-eq "es igula a"
# si es 0 va ser exitosa

    # Run the program with provided arguments
    ./file_processor "$@"
    #$@ esto es para saber que va a recibir los resultados
else
    echo "Compilation failed."
fi


