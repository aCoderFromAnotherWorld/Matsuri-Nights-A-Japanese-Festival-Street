#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <filesystem>
#include <iostream>

namespace TextureGenerator
{
    inline bool writeBMP24(const std::string& filepath, int width, int height, const std::vector<unsigned char>& rgb)
    {
        int rowPadded = (width * 3 + 3) & (~3);
        uint32_t imageSize = rowPadded * height;
        uint32_t fileSize = 54 + imageSize;

        unsigned char fileHeader[14] = {
            'B', 'M',
            (unsigned char)(fileSize), (unsigned char)(fileSize >> 8), (unsigned char)(fileSize >> 16), (unsigned char)(fileSize >> 24),
            0, 0, 0, 0,
            54, 0, 0, 0
        };

        unsigned char infoHeader[40] = {
            40, 0, 0, 0,
            (unsigned char)(width), (unsigned char)(width >> 8), (unsigned char)(width >> 16), (unsigned char)(width >> 24),
            (unsigned char)(height), (unsigned char)(height >> 8), (unsigned char)(height >> 16), (unsigned char)(height >> 24),
            1, 0,
            24, 0,
            0, 0, 0, 0,
            (unsigned char)(imageSize), (unsigned char)(imageSize >> 8), (unsigned char)(imageSize >> 16), (unsigned char)(imageSize >> 24),
            0x13, 0x0B, 0, 0, // 2835 ppm resolution
            0x13, 0x0B, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        };

        std::ofstream out(filepath, std::ios::binary);
        if (!out.is_open())
        {
            std::cerr << "[TextureGenerator] Could not create file: " << filepath << std::endl;
            return false;
        }

        out.write((char*)fileHeader, 14);
        out.write((char*)infoHeader, 40);

        std::vector<unsigned char> row(rowPadded, 0);
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int srcIdx = (y * width + x) * 3;
                // BMP expects BGR byte order
                row[x * 3 + 0] = rgb[srcIdx + 2]; // Blue
                row[x * 3 + 1] = rgb[srcIdx + 1]; // Green
                row[x * 3 + 2] = rgb[srcIdx + 0]; // Red
            }
            out.write((char*)row.data(), rowPadded);
        }
        return true;
    }

    inline float pseudoNoise(int x, int y)
    {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f) * 0.5f + 0.5f;
    }

    inline void generateWoodTimber(const std::string& path, int size = 256)
    {
        std::vector<unsigned char> rgb(size * size * 3);
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                float fx = (float)x;
                float fy = (float)y;
                // Sinusoidal cedar wood grain striations
                float grain = std::sin(fy * 0.18f + std::sin(fx * 0.04f) * 6.0f);
                float noise = pseudoNoise(x, y) * 0.25f;
                float val = 0.5f + 0.35f * grain + noise;
                val = std::clamp(val, 0.0f, 1.0f);

                int idx = (y * size + x) * 3;
                rgb[idx + 0] = (unsigned char)std::clamp(140.0f * val + 25.0f, 0.0f, 255.0f); // R
                rgb[idx + 1] = (unsigned char)std::clamp(90.0f * val + 15.0f, 0.0f, 255.0f);  // G
                rgb[idx + 2] = (unsigned char)std::clamp(52.0f * val + 10.0f, 0.0f, 255.0f);  // B
            }
        }
        writeBMP24(path, size, size, rgb);
    }

    inline void generateRoofTiles(const std::string& path, int size = 256)
    {
        std::vector<unsigned char> rgb(size * size * 3);
        int rows = 8;
        int rowHeight = size / rows;
        for (int y = 0; y < size; ++y)
        {
            int rowY = y % rowHeight;
            float rowNorm = (float)rowY / (float)rowHeight;
            for (int x = 0; x < size; ++x)
            {
                // Scalloped wave curve along x
                float wave = std::sin((float)x * 0.16f);
                float shade = 0.45f + 0.40f * (1.0f - rowNorm) + 0.15f * wave;
                if (rowY < 2) shade *= 0.35f; // Deep shadow lip at tile bottom

                int idx = (y * size + x) * 3;
                rgb[idx + 0] = (unsigned char)std::clamp(70.0f * shade, 0.0f, 255.0f);
                rgb[idx + 1] = (unsigned char)std::clamp(78.0f * shade, 0.0f, 255.0f);
                rgb[idx + 2] = (unsigned char)std::clamp(88.0f * shade, 0.0f, 255.0f);
            }
        }
        writeBMP24(path, size, size, rgb);
    }

    inline void generateStonePavement(const std::string& path, int size = 256)
    {
        std::vector<unsigned char> rgb(size * size * 3);
        int blockSize = 32;
        for (int y = 0; y < size; ++y)
        {
            int by = y / blockSize;
            int shift = (by % 2 == 0) ? 0 : (blockSize / 2);
            for (int x = 0; x < size; ++x)
            {
                int sx = (x + shift) % blockSize;
                int sy = y % blockSize;
                bool isMortar = (sx <= 2 || sy <= 2);
                float noise = pseudoNoise(x, y) * 0.22f;

                int idx = (y * size + x) * 3;
                if (isMortar)
                {
                    rgb[idx + 0] = 55; rgb[idx + 1] = 52; rgb[idx + 2] = 48;
                }
                else
                {
                    float stone = 0.75f + noise;
                    rgb[idx + 0] = (unsigned char)std::clamp(150.0f * stone, 0.0f, 255.0f);
                    rgb[idx + 1] = (unsigned char)std::clamp(146.0f * stone, 0.0f, 255.0f);
                    rgb[idx + 2] = (unsigned char)std::clamp(140.0f * stone, 0.0f, 255.0f);
                }
            }
        }
        writeBMP24(path, size, size, rgb);
    }

    inline void generateLanternPaper(const std::string& path, int size = 256)
    {
        std::vector<unsigned char> rgb(size * size * 3);
        for (int y = 0; y < size; ++y)
        {
            bool isRib = (y % 18 <= 2);
            for (int x = 0; x < size; ++x)
            {
                float noise = pseudoNoise(x, y) * 0.15f;
                int idx = (y * size + x) * 3;
                if (isRib)
                {
                    rgb[idx + 0] = 85; rgb[idx + 1] = 12; rgb[idx + 2] = 10; // Dark bamboo ring rib
                }
                else
                {
                    float paper = 0.85f + noise;
                    rgb[idx + 0] = (unsigned char)std::clamp(225.0f * paper, 0.0f, 255.0f);
                    rgb[idx + 1] = (unsigned char)std::clamp(42.0f * paper, 0.0f, 255.0f);
                    rgb[idx + 2] = (unsigned char)std::clamp(28.0f * paper, 0.0f, 255.0f);
                }
            }
        }
        writeBMP24(path, size, size, rgb);
    }

    inline void generateTatamiCloth(const std::string& path, int size = 256)
    {
        std::vector<unsigned char> rgb(size * size * 3);
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                // Fine interlaced cross-weave pattern
                float weave = ((x % 4 < 2) ^ (y % 4 < 2)) ? 0.92f : 0.68f;
                float noise = pseudoNoise(x, y) * 0.12f;
                float val = weave + noise;

                int idx = (y * size + x) * 3;
                rgb[idx + 0] = (unsigned char)std::clamp(185.0f * val, 0.0f, 255.0f);
                rgb[idx + 1] = (unsigned char)std::clamp(175.0f * val, 0.0f, 255.0f);
                rgb[idx + 2] = (unsigned char)std::clamp(155.0f * val, 0.0f, 255.0f);
            }
        }
        writeBMP24(path, size, size, rgb);
    }

    inline void generateGoldLeaf(const std::string& path, int size = 256)
    {
        std::vector<unsigned char> rgb(size * size * 3);
        int leafGrid = 64;
        for (int y = 0; y < size; ++y)
        {
            bool isSeamY = (y % leafGrid == 0);
            for (int x = 0; x < size; ++x)
            {
                bool isSeamX = (x % leafGrid == 0);
                float noise = pseudoNoise(x, y) * 0.25f;
                float gold = 0.85f + noise;
                if (isSeamX || isSeamY) gold *= 0.65f;

                int idx = (y * size + x) * 3;
                rgb[idx + 0] = (unsigned char)std::clamp(235.0f * gold, 0.0f, 255.0f);
                rgb[idx + 1] = (unsigned char)std::clamp(195.0f * gold, 0.0f, 255.0f);
                rgb[idx + 2] = (unsigned char)std::clamp(55.0f * gold, 0.0f, 255.0f);
            }
        }
        writeBMP24(path, size, size, rgb);
    }

    inline void generateSakuraBark(const std::string& path, int size = 256)
    {
        std::vector<unsigned char> rgb(size * size * 3);
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                float verticalGrain = std::sin((float)x * 0.35f + pseudoNoise(x, y) * 4.0f);
                float lenticel = (y % 28 <= 3 && x % 40 < 18) ? 0.45f : 1.0f; // horizontal cherry bark lenticels
                float val = (0.65f + 0.35f * verticalGrain) * lenticel;

                int idx = (y * size + x) * 3;
                rgb[idx + 0] = (unsigned char)std::clamp(92.0f * val, 0.0f, 255.0f);
                rgb[idx + 1] = (unsigned char)std::clamp(68.0f * val, 0.0f, 255.0f);
                rgb[idx + 2] = (unsigned char)std::clamp(54.0f * val, 0.0f, 255.0f);
            }
        }
        writeBMP24(path, size, size, rgb);
    }

    inline void generateTakoyakiFood(const std::string& path, int size = 256)
    {
        std::vector<unsigned char> rgb(size * size * 3);
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                float u = (float)x / (float)size;
                float v = (float)y / (float)size;

                // 1. Fried batter base: warm golden ochre with porous batter variations
                float noise1 = pseudoNoise(x * 2, y * 2);
                float noise2 = pseudoNoise(x * 5 + 13, y * 5 + 37);
                float batter = 0.80f + 0.20f * noise1;

                // Griddle scorch / toasted caramelization specks
                float scorch = (noise2 > 0.82f) ? 0.65f : 1.0f;

                float r = (218.0f * batter + 10.0f) * scorch;
                float g = (152.0f * batter + 5.0f) * scorch;
                float b = (62.0f * batter) * scorch;

                // 2. Rich Dark Savory Takoyaki Sauce (drizzled streams and pooled glaze)
                // Flowing glossy ribbons across center/upper latitudes
                float saucePattern = std::sin(u * 14.0f + std::sin(v * 8.0f) * 2.5f) * 0.5f + 0.5f;
                float sauceCoverage = std::sin(v * 3.14159f); // Thicker toward center
                bool isSauce = (saucePattern * sauceCoverage > 0.42f) || (std::abs(v - 0.50f) < 0.22f && noise1 > 0.35f);

                if (isSauce)
                {
                    // Glossy dark mahogany / sweet soy fruit glaze
                    float sauceGloss = 0.85f + 0.25f * noise1;
                    r = 52.0f * sauceGloss;
                    g = 22.0f * sauceGloss;
                    b = 10.0f * sauceGloss;
                }

                // 3. Creamy Japanese Kewpie Mayonnaise (criss-cross zig-zag stripes)
                float mayo1 = std::abs(std::sin((u + v) * 24.0f));
                float mayo2 = std::abs(std::sin((u - v) * 24.0f));
                bool isMayo = (mayo1 < 0.15f || mayo2 < 0.15f) && (isSauce || noise2 > 0.4f) && (v > 0.15f && v < 0.85f);
                if (isMayo)
                {
                    // Ivory pale-cream egg mayonnaise
                    r = 248.0f;
                    g = 242.0f;
                    b = 210.0f;
                }

                // 4. Emerald Green Aonori (dried seaweed flake sprinkles)
                float aonoriNoise = pseudoNoise(x * 11 + 101, y * 11 + 203);
                if (aonoriNoise > 0.88f && (isSauce || isMayo))
                {
                    r = 24.0f;
                    g = 128.0f;
                    b = 36.0f;
                }

                // 5. Curled Katsuobushi (shaved bonito tuna flakes)
                float bonitoNoise = pseudoNoise(x / 4, y / 3);
                if (bonitoNoise > 0.78f && noise1 > 0.55f && v > 0.25f && v < 0.75f)
                {
                    r = 210.0f;
                    g = 152.0f;
                    b = 115.0f;
                }

                int idx = (y * size + x) * 3;
                rgb[idx + 0] = (unsigned char)std::clamp(r, 0.0f, 255.0f);
                rgb[idx + 1] = (unsigned char)std::clamp(g, 0.0f, 255.0f);
                rgb[idx + 2] = (unsigned char)std::clamp(b, 0.0f, 255.0f);
            }
        }
        writeBMP24(path, size, size, rgb);
    }

    inline void ensureTextureAssetsExist(const std::string& dir = "assets/textures")
    {
        try
        {
            if (!std::filesystem::exists(dir))
            {
                std::filesystem::create_directories(dir);
            }

            std::string fWood = dir + "/wood_timber.bmp";
            std::string fRoof = dir + "/roof_tiles.bmp";
            std::string fStone = dir + "/stone_pavement.bmp";
            std::string fLantern = dir + "/lantern_paper.bmp";
            std::string fTatami = dir + "/tatami_cloth.bmp";
            std::string fGold = dir + "/gold_leaf.bmp";
            std::string fBark = dir + "/sakura_bark.bmp";
            std::string fTakoyaki = dir + "/takoyaki_food.bmp";

            if (!std::filesystem::exists(fWood)) generateWoodTimber(fWood);
            if (!std::filesystem::exists(fRoof)) generateRoofTiles(fRoof);
            if (!std::filesystem::exists(fStone)) generateStonePavement(fStone);
            if (!std::filesystem::exists(fLantern)) generateLanternPaper(fLantern);
            if (!std::filesystem::exists(fTatami)) generateTatamiCloth(fTatami);
            if (!std::filesystem::exists(fGold)) generateGoldLeaf(fGold);
            if (!std::filesystem::exists(fBark)) generateSakuraBark(fBark);
            if (!std::filesystem::exists(fTakoyaki)) generateTakoyakiFood(fTakoyaki);

            std::cout << "[TextureGenerator] Texture assets verified in: " << dir << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "[TextureGenerator] Exception: " << e.what() << std::endl;
        }
    }
}
