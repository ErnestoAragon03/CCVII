#!/bin/bash

echo "🛠️  Compilando..."
make

if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa. Ejecutando programa..."
    echo "---------------------------------------------"
    ./scheduling
else
    echo "❌ Falló la compilación."
fi
