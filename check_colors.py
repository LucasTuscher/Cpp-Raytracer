#!/usr/bin/env python3
"""
Prüft welche Farben in den PPM-Dateien vorkommen
"""
import sys

def check_ppm_colors(filename):
    print(f"\n=== {filename} ===")
    with open(filename, 'r') as f:
        # Lese Header
        magic = f.readline().strip()
        print(f"Format: {magic}")

        # Überspringe Kommentare
        line = f.readline()
        while line.startswith('#'):
            line = f.readline()

        # Lese Dimensionen
        width, height = map(int, line.split())
        print(f"Größe: {width}x{height}")

        # Lese max value
        max_val = int(f.readline().strip())
        print(f"Max Value: {max_val}")

        # Lese alle restlichen Zeilen und extrahiere RGB-Werte
        colors = {}
        pixels_read = 0

        # Lese den Rest der Datei
        content = f.read()
        values = content.split()

        # Extrahiere RGB-Tripel
        for i in range(0, len(values), 3):
            if i + 2 < len(values):
                r = int(values[i])
                g = int(values[i + 1])
                b = int(values[i + 2])

                color = (r, g, b)
                colors[color] = colors.get(color, 0) + 1
                pixels_read += 1

        print(f"Pixel gelesen: {pixels_read}")
        print(f"Verschiedene Farben: {len(colors)}")

        # Zeige häufigste Farben
        sorted_colors = sorted(colors.items(), key=lambda x: x[1], reverse=True)
        print(f"\nTop 5 häufigste Farben:")
        for i, (color, count) in enumerate(sorted_colors[:5]):
            percentage = 100.0 * count / pixels_read
            print(f"  {i+1}. RGB{color} - {count} Pixel ({percentage:.2f}%)")

        # Prüfe speziell auf Blau (0, 0, 255)
        blue_count = colors.get((0, 0, 255), 0)
        if blue_count > 0:
            print(f"\n✓ BLAU (0,0,255) gefunden: {blue_count} Pixel")
        else:
            print(f"\n✗ BLAU (0,0,255) NICHT gefunden!")

        # Prüfe auf Orange (255, 127/128, 0) für Hits
        orange1 = colors.get((255, 127, 0), 0)
        orange2 = colors.get((255, 128, 0), 0)
        if orange1 > 0 or orange2 > 0:
            print(f"✓ ORANGE gefunden: {orange1 + orange2} Pixel")

if __name__ == "__main__":
    files = [
        "test-sphere-hitmiss.ppm",
        "test-sphere-tvalue.ppm",
        "test-sphere-normals.ppm"
    ]

    for f in files:
        try:
            check_ppm_colors(f)
        except Exception as e:
            print(f"\nFehler bei {f}: {e}")
