# C++ Raytracer (Uni Project)

Ein C++ Raytracer Projekt mit Google Test Integration.

## Projekt aufsetzen (erstmalig)

```powershell
# Build-Verzeichnis erstellen
mkdir build
cd build

# CMake konfigurieren
cmake ..

# Projekt kompilieren
cmake --build .
```

## Programm ausführen

```powershell
# Von Projekt-Root aus:
.\build\Debug\raytracer.exe

# Oder im build-Ordner:
cd build
.\Debug\raytracer.exe
```

## Tests ausführen

```powershell
# Option 1: Mit ctest
cd build
ctest --output-on-failure -C Debug

# Option 2: Direkt
cd build
.\Debug\raytracer_tests.exe
```

## Nach Änderungen neu kompilieren

Wenn du **Main.cpp** oder andere Dateien änderst:

```powershell
cd build
cmake --build .
```

Das war's! CMake erkennt automatisch welche Dateien geändert wurden und kompiliert nur diese neu.

## Projekt komplett neu bauen

Falls etwas schiefgeht:

```powershell
# Build-Ordner löschen und neu anfangen
Remove-Item -Recurse -Force build
mkdir build
cd build
cmake ..
cmake --build .
```

## Projekt-Struktur

```
Cpp-Raytracer/
├── CMakeLists.txt          # CMake Konfiguration
├── README.md               # Diese Datei
├── src/
│   └── Main.cpp           # Haupt-Programm
├── tests/
│   └── TestMain.cpp      # Google Test Unit Tests
└── build/
    └── Debug/             # Kompilierte .exe Dateien hier
