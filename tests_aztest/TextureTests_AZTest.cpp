/**
 * TextureTests_AZTest.cpp
 *
 * Unit-Tests für UV-Mapping und ImageTexture (PPM) mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include <cstdint>
#include <sstream>
#include <vector>

#include "../src/Matrix/Matrix.h"
#include "../src/Point/Point.h"
#include "../src/Shape/Plane.h"
#include "../src/Shape/Sphere.h"
#include "../src/Texture/Image.h"
#include "../src/Texture/ImageTexture.h"

// ============================================================================
// Feature: UV Mapping
// ============================================================================

TEST(UVMapping, SphereUVMappingAtKeyPoints) {
    Sphere sphere;

    UV uv1 = sphere.uvAt(Point(0, 1, 0));   // north pole
    EXPECT_TRUE(uv1.equals(UV(0.5, 0.0), 1e-6));

    UV uv2 = sphere.uvAt(Point(0, -1, 0));  // south pole
    EXPECT_TRUE(uv2.equals(UV(0.5, 1.0), 1e-6));

    UV uv3 = sphere.uvAt(Point(1, 0, 0));   // +x
    EXPECT_TRUE(uv3.equals(UV(0.5, 0.5), 1e-6));

    UV uv4 = sphere.uvAt(Point(0, 0, 1));   // +z
    EXPECT_TRUE(uv4.equals(UV(0.75, 0.5), 1e-6));

    UV uv5 = sphere.uvAt(Point(0, 0, -1));  // -z
    EXPECT_TRUE(uv5.equals(UV(0.25, 0.5), 1e-6));

    UV uv6 = sphere.uvAt(Point(-1, 0, 0));  // -x (seam)
    EXPECT_TRUE(uv6.equals(UV(0.0, 0.5), 1e-6));
}

TEST(UVMapping, SphereUVMappingRespectsTransform) {
    Sphere sphere;
    sphere.setTransform(Matrix::scale(2.0, 2.0, 2.0));

    // World point on the scaled sphere should map to the same UV as the corresponding local point.
    UV uv = sphere.uvAt(Point(2, 0, 0));
    EXPECT_TRUE(uv.equals(UV(0.5, 0.5), 1e-6));
}

TEST(UVMapping, PlaneUVMappingRepeats) {
    Plane plane;

    UV uv1 = plane.uvAt(Point(0.25, 0.0, 0.5));
    EXPECT_TRUE(uv1.equals(UV(0.25, 0.5), 1e-6));

    UV uv2 = plane.uvAt(Point(1.25, 0.0, 0.5)); // repeat in x
    EXPECT_TRUE(uv2.equals(UV(0.25, 0.5), 1e-6));

    UV uv3 = plane.uvAt(Point(0.25, 0.0, 1.5)); // repeat in z
    EXPECT_TRUE(uv3.equals(UV(0.25, 0.5), 1e-6));
}

// ============================================================================
// Feature: ImageTexture (PPM P3)
// ============================================================================

TEST(ImageTexture, LoadsPPMFromStream) {
    std::istringstream ppm(R"PPM(
P3
# a tiny 2x2 image
2 2
255
255 0 0   0 255 0
0 0 255   255 255 255
)PPM");

    Image img = Image::loadPPM(ppm);
    EXPECT_TRUE(img.getWidth() == 2);
    EXPECT_TRUE(img.getHeight() == 2);

    EXPECT_TRUE(img.getPixel(0, 0).equals(Color(1, 0, 0), 1e-6));
    EXPECT_TRUE(img.getPixel(1, 0).equals(Color(0, 1, 0), 1e-6));
    EXPECT_TRUE(img.getPixel(0, 1).equals(Color(0, 0, 1), 1e-6));
    EXPECT_TRUE(img.getPixel(1, 1).equals(Color(1, 1, 1), 1e-6));
}

TEST(ImageTexture, SamplesWithUV) {
    std::istringstream ppm(R"PPM(
P3
2 2
255
255 0 0   0 255 0
0 0 255   255 255 255
)PPM");

    ImageTexture texture(Image::loadPPM(ppm));

    EXPECT_TRUE(texture.sample(UV(0.25, 0.25), Point(0, 0, 0)).equals(Color(1, 0, 0), 1e-6));
    EXPECT_TRUE(texture.sample(UV(0.75, 0.25), Point(0, 0, 0)).equals(Color(0, 1, 0), 1e-6));
    EXPECT_TRUE(texture.sample(UV(0.25, 0.75), Point(0, 0, 0)).equals(Color(0, 0, 1), 1e-6));
    EXPECT_TRUE(texture.sample(UV(0.75, 0.75), Point(0, 0, 0)).equals(Color(1, 1, 1), 1e-6));
}

TEST(ImageTexture, LoadsBMPFromStream) {
    std::vector<unsigned char> bytes;
    bytes.reserve(128);

    auto push8 = [&](unsigned char b) { bytes.push_back(b); };
    auto pushLE16 = [&](uint16_t v) {
        push8(static_cast<unsigned char>(v & 0xFF));
        push8(static_cast<unsigned char>((v >> 8) & 0xFF));
    };
    auto pushLE32 = [&](uint32_t v) {
        push8(static_cast<unsigned char>(v & 0xFF));
        push8(static_cast<unsigned char>((v >> 8) & 0xFF));
        push8(static_cast<unsigned char>((v >> 16) & 0xFF));
        push8(static_cast<unsigned char>((v >> 24) & 0xFF));
    };

    const uint32_t pixelDataSize = 16; // 2 rows * 8 bytes (padded)
    const uint32_t fileSize = 54 + pixelDataSize;

    // BMP header
    push8('B');
    push8('M');
    pushLE32(fileSize);
    pushLE16(0);
    pushLE16(0);
    pushLE32(54); // pixel data offset

    // DIB header (BITMAPINFOHEADER)
    pushLE32(40); // header size
    pushLE32(2);  // width
    pushLE32(2);  // height (bottom-up)
    pushLE16(1);  // planes
    pushLE16(24); // bpp
    pushLE32(0);  // compression BI_RGB
    pushLE32(pixelDataSize);
    pushLE32(0); // x ppm
    pushLE32(0); // y ppm
    pushLE32(0); // colors used
    pushLE32(0); // important colors

    // Pixel data (bottom row first), BGR, row padded to 4-byte boundary (8 bytes per row)
    // Bottom row: blue, white
    push8(255); push8(0);   push8(0);   // blue
    push8(255); push8(255); push8(255); // white
    push8(0);   push8(0);               // padding
    // Top row: red, green
    push8(0);   push8(0);   push8(255); // red
    push8(0);   push8(255); push8(0);   // green
    push8(0);   push8(0);               // padding

    std::string blob(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    std::istringstream in(blob, std::ios::binary);

    Image img = Image::loadBMP(in);
    EXPECT_TRUE(img.getWidth() == 2);
    EXPECT_TRUE(img.getHeight() == 2);

    EXPECT_TRUE(img.getPixel(0, 0).equals(Color(1, 0, 0), 1e-6)); // top-left red
    EXPECT_TRUE(img.getPixel(1, 0).equals(Color(0, 1, 0), 1e-6)); // top-right green
    EXPECT_TRUE(img.getPixel(0, 1).equals(Color(0, 0, 1), 1e-6)); // bottom-left blue
    EXPECT_TRUE(img.getPixel(1, 1).equals(Color(1, 1, 1), 1e-6)); // bottom-right white
}
