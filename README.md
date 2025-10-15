# T4 - Evaluación de técnicas anti-VM en C++
Integrantes del equipo

    David Alejandro Rodriguez Garcia - 1814035

    Jair Eliud Flores Puente - 2076332
    
Proyecto en C++17 con 3 técnicas de detección de entornos virtuales.

## Compilación

### Linux
```
Acceder a la carpeta src

cd T4/src/

g++ -std=c++17 *.cpp -o antivm_detect
```

### Windows
```
Acceder a la carpeta src

cd T4/src/

cl /EHsc /std:c++17 *.cpp /Fe:antivm_detect.exe iphlpapi.lib
```

## Ejecución
```
./antivm_detect
./antivm_detect --technique 1  # CPUID
```

## Estructura
src/, Results/, README.md, .gitignore.
