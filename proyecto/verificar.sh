#!/bin/bash

BIN1="Prog1.bin"
BIN2="Prog2.bin"

if [[ ! -f "$BIN1" || ! -f "$BIN2" ]]; then
    echo "❌ Uno o ambos binarios no existen: $BIN1 / $BIN2"
    exit 1
fi

echo "🔍 Comparando $BIN1 y $BIN2..."

if cmp -s "$BIN1" "$BIN2"; then
    echo "⚠️  ¡Son idénticos! Ambos binarios contienen lo mismo."
    echo "❗ Asegúrate de que p2.c tiene código distinto al de p1.c y fue recompilado correctamente."
else
    echo "✅ Son diferentes. Todo bien para cargar como procesos independientes."
fi
