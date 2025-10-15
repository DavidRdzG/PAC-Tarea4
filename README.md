# T4 - Evaluación de técnicas anti-VM en C++
Integrantes del equipo

    David Alejandro Rodriguez Garcia - 1814035

    Jair Eliud Flores Puente - 2076332
    
Proyecto en C++17 con 3 técnicas de detección de entornos virtuales.

## Compilación

### Linux
```bash
g++ -std=c++17 src/*.cpp -o antivm_detect
```

### Windows
```bat
cl /EHsc /std:c++17 src\*.cpp /Fe:antivm_detect.exe iphlpapi.lib
```

## Ejecución
```bash
./antivm_detect
./antivm_detect --technique 1  # CPUID
```

## Estructura
src/, Results/, README.md, .gitignore.
