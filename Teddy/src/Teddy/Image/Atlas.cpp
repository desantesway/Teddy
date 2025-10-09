#include "Teddy/Image/Atlas.h"

#include "TeddyPch.h"
#include "Teddy/Utils/PlatformUtils.h"

#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Teddy::Atlas
{
    void Generate(const std::vector<std::string>& filepaths,
        int maxWidth, int maxHeight, int toleration,
        const std::string& outputPrefix)
    {
        if (filepaths.empty())
            TED_CORE_ERROR("No files provided");

        toleration = std::clamp(toleration, 0, 100);

        int refW, refH, channels;
        {
            unsigned char* data = stbi_load(filepaths[0].c_str(), &refW, &refH, &channels, 4);
            if (!data)
                TED_CORE_ERROR("Failed to load {}", filepaths[0]);
            stbi_image_free(data);
        }

        int maxW = refW, maxH = refH;
        for (size_t i = 1; i < filepaths.size(); i++) {
            int w, h, ch;
            unsigned char* data = stbi_load(filepaths[i].c_str(), &w, &h, &ch, 4);
            if (!data)
                TED_CORE_ERROR("Failed to load {}", filepaths[i]);
            stbi_image_free(data);

            maxW = std::max(maxW, w);
            maxH = std::max(maxH, h);
        }

        int spriteW = maxW;
        int spriteH = maxH;

        int padding = 1;
        auto ComputeCapacity = [&](int w, int h) {
            int pw = w + 2 * padding;
            int ph = h + 2 * padding;
            int cols = maxWidth / pw;
            int rows = maxHeight / ph;
            return cols * rows;
            };

        int baseCapacity = ComputeCapacity(spriteW, spriteH);

        float scale = 1.0f - (toleration / 100.0f);
        int scaledW = spriteW * scale;
        int scaledH = spriteH * scale;

        int scaledCapacity = ComputeCapacity(scaledW, scaledH);

		int scaledAtlasQuantity = 0;
		int baseAtlasQuantity = 0;
        
		int fileCount = filepaths.size();
        while (fileCount > 0)
        {
            scaledAtlasQuantity++;
			fileCount -= scaledCapacity;
        }
        fileCount = filepaths.size();
        while (fileCount > 0)
        {
			baseAtlasQuantity++;
            fileCount -= baseCapacity;
		}

        if (baseAtlasQuantity > scaledAtlasQuantity) {
            spriteW = scaledW;
            spriteH = scaledH;
            TED_CORE_INFO("Applied toleration shrink: new sprite size = {}x{}, capacity {} (was {})",
                spriteW, spriteH, scaledCapacity, baseCapacity);
        }
        else {
            TED_CORE_INFO("No benefit from toleration; keeping original sprite size.");
        }

        int paddingW = spriteW + 2 * padding;
        int paddingH = spriteH + 2 * padding;
        int cols = maxWidth / paddingW;
        int rows = maxHeight / paddingH;
        int capacity = cols * rows;

        if (capacity < 1) {
            TED_CORE_ERROR("Sprites are too large to fit even one per atlas (increase tolerance or decrease sprite size)");
            return;
        }

        size_t total = filepaths.size();
        size_t processed = 0;
        int atlasIndex = 0;

        std::string atlasPath = FileDialogs::SaveFile("*.*\0");
        if (atlasPath.empty()) {
            TED_CORE_ERROR("Path doesn't exist!");
            return;
        }

        while (processed < total) {
            size_t remaining = total - processed;
            size_t count = std::min(static_cast<size_t>(capacity), remaining);

            int atlasW = maxWidth;
            int atlasH = maxHeight;

            if (count == 1) {
                atlasW = paddingW;
                atlasH = paddingH;
            }

            std::vector<unsigned char> buffer(atlasW * atlasH * 4, 0);

            for (size_t i = 0; i < count; i++) {
                size_t index = processed + i;

                int col = i % cols;
                int row = i / cols;
                int entryX = col * paddingW + padding;
                int entryY = row * paddingH + padding;

                int w, h, ch;
                unsigned char* src = stbi_load(filepaths[index].c_str(), &w, &h, &ch, 4);
                if (!src)
                    TED_CORE_ERROR("Failed to load {}", filepaths[index]);

                int drawW = w;
                int drawH = h;

                if (w > spriteW || h > spriteH) {
                    float scale = std::min(spriteW / (float)w, spriteH / (float)h);
                    drawW = static_cast<int>(w * scale);
                    drawH = static_cast<int>(h * scale);
                }

                int offsetX = entryX + (spriteW - drawW) / 2;
                int offsetY = entryY + (spriteH - drawH) / 2;

                if (drawW == w && drawH == h) {
                    for (int y = 0; y < h; y++) {
                        if (offsetY + y >= atlasH) break;
                        for (int x = 0; x < w; x++) {
                            if (offsetX + x >= atlasW) break;
                            unsigned char* srcPixel = &src[(y * w + x) * 4];
                            unsigned char* dstPixel = &buffer[((offsetY + y) * atlasW + (offsetX + x)) * 4];
                            memcpy(dstPixel, srcPixel, 4);
                        }
                    }
                }
                else {
                    for (int y = 0; y < drawH; y++) {
                        int srcY = static_cast<int>((y / (float)drawH) * h);
                        for (int x = 0; x < drawW; x++) {
                            int srcX = static_cast<int>((x / (float)drawW) * w);
                            unsigned char* srcPixel = &src[(srcY * w + srcX) * 4];
                            unsigned char* dstPixel = &buffer[((offsetY + y) * atlasW + (offsetX + x)) * 4];
                            memcpy(dstPixel, srcPixel, 4);
                        }
                    }
                }

                stbi_image_free(src);
            }

            for (int y = 0; y < atlasH / 2; y++) {
                int oppY = atlasH - 1 - y;
                for (int x = 0; x < atlasW * 4; x++) {
                    std::swap(buffer[y * atlasW * 4 + x], buffer[oppY * atlasW * 4 + x]);
                }
            }

            std::string filename = atlasPath + outputPrefix + "_" +
                std::to_string(paddingW) + "x" + std::to_string(paddingH) + "_" +
                std::to_string(atlasW) + "x" + std::to_string(atlasH) + "_" +
                std::to_string(atlasIndex) + ".png";

            if (!stbi_write_png(filename.c_str(), atlasW, atlasH, 4, buffer.data(), atlasW * 4))
                TED_CORE_ERROR("Failed to save {}", filename);
            else
                TED_CORE_INFO("Saved atlas to: {}", filename);

            processed += count;
            atlasIndex++;
        }
    }

}
