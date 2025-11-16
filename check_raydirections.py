#!/usr/bin/env python3
"""
Analysiert das Strahlrichtungs-Bild
"""

def check_raydirections():
    filename = "test-sphere-raydirections.ppm"
    with open(filename, 'r') as f:
        # Lese Header
        magic = f.readline().strip()
        line = f.readline()
        while line.startswith('#'):
            line = f.readline()
        width, height = map(int, line.split())
        max_val = int(f.readline().strip())

        print(f"=== {filename} ===")
        print(f"Format: {magic}, Größe: {width}x{height}, Max: {max_val}\n")

        # Lese Pixel-Daten
        content = f.read()
        values = content.split()

        # Prüfe einige spezifische Pixel
        def get_pixel(x, y):
            idx = (y * width + x) * 3
            if idx + 2 < len(values):
                return (int(values[idx]), int(values[idx+1]), int(values[idx+2]))
            return None

        # Zentrum
        cx, cy = width // 2, height // 2
        center = get_pixel(cx, cy)
        print(f"Zentrum ({cx}, {cy}): RGB{center}")

        # Ecken
        tl = get_pixel(0, 0)
        tr = get_pixel(width-1, 0)
        bl = get_pixel(0, height-1)
        br = get_pixel(width-1, height-1)

        print(f"Ecke oben-links (0, 0): RGB{tl}")
        print(f"Ecke oben-rechts ({width-1}, 0): RGB{tr}")
        print(f"Ecke unten-links (0, {height-1}): RGB{bl}")
        print(f"Ecke unten-rechts ({width-1}, {height-1}): RGB{br}")

        # Ränder
        print(f"\nRechter Rand ({width-1}, {cy}): RGB{get_pixel(width-1, cy)}")
        print(f"Linker Rand (0, {cy}): RGB{get_pixel(0, cy)}")
        print(f"Oberer Rand ({cx}, 0): RGB{get_pixel(cx, 0)}")
        print(f"Unterer Rand ({cx}, {height-1}): RGB{get_pixel(cx, height-1)}")

if __name__ == "__main__":
    check_raydirections()
