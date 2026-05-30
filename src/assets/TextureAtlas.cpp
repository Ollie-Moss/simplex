#include "assets/TextureAtlas.h"
#include "assets/AssetLoader.h"
#include "glm/fwd.hpp"
#include "graphics/util/Texture.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <filesystem>
#include <format>
#include "stb_image_write.h"
#include <iostream>
#include <limits>
#include <mutex>
#include <optional>
#include <string>

std::vector<std::string> getPNGFilesRecursive(const std::string &path)
{
    std::vector<std::string> pngFiles;

    try
    {
        if(!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
        {
            std::cerr << "Invalid path: " << path << std::endl;
            return pngFiles;
        }

        for(const auto &entry : std::filesystem::recursive_directory_iterator(path))
        {
            if(entry.is_regular_file() && entry.path().extension() == ".png")
            {
                pngFiles.push_back(entry.path().string());
            }
        }
    }
    catch(const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }

    return pngFiles;
}

struct ImageData
{
    ImageData(std::string path, glm::vec2 position, glm::vec2 size) : path(path), position(position), size(size) {};
    std::string path;
    glm::vec2 size;
    glm::vec2 position;
};

struct Rect
{
    Rect(glm::vec2 position, glm::vec2 size) : position(position), size(size) {};
    glm::vec2 position;
    glm::vec2 size;
};

float area(glm::vec2 size)
{
    return size.x * size.y;
}

bool quadsIntersect(glm::vec2 pos1, glm::vec2 size1, glm::vec2 pos2, glm::vec2 size2)
{
    float left1 = pos1.x;
    float right1 = pos1.x + size1.x;
    float top1 = pos1.y;
    float bottom1 = pos1.y + size1.y;

    float left2 = pos2.x;
    float right2 = pos2.x + size2.x;
    float top2 = pos2.y;
    float bottom2 = pos2.y + size2.y;

    bool overlapsX = left1 <= right2 && right1 >= left2;
    bool overlapsY = top1 <= bottom2 && bottom1 >= top2;

    return overlapsX && overlapsY;
}

bool rectContainedIn(const Rect &rect1, const Rect &rect2)
{
    float left1 = rect1.position.x;
    float right1 = rect1.position.x + rect1.size.x;
    float top1 = rect1.position.y;
    float bottom1 = rect1.position.y + rect1.size.y;

    float left2 = rect2.position.x;
    float right2 = rect2.position.x + rect2.size.x;
    float top2 = rect2.position.y;
    float bottom2 = rect2.position.y + rect2.size.y;

    return left1 >= left2 && right1 <= right2 && top1 >= top2 && bottom1 <= bottom2;
}

class MaxRectsPacker
{
  public:
    MaxRectsPacker(glm::vec2 size) : size(size) {}
    glm::vec2 size;
    std::vector<ImageData> packedImages;

    bool Pack(std::vector<ImageData> images)
    {
        rects = {Rect(glm::vec2(0, 0), size)};
        std::sort(images.begin(), images.end(), [](ImageData a, ImageData b) {
            return area(a.size) > area(b.size);
        });

        for(auto image : images)
        {
            std::cout << image.path << "\n";
            std::cout << rects.size() << "\n";
            auto rect = GetBestRect(image);

            if(rect == nullptr)
                return false;

            image.position = rect->position;
            packedImages.push_back(image);
            SplitRect(image);
            // rects.erase(rects.begin() + (rect - &rects[0]));
        }
        return true;
    }

  private:
    std::vector<Rect> rects;

    Rect *GetBestRect(ImageData image)
    {
        float bestWaste = std::numeric_limits<float>::infinity();
        Rect *bestRect = nullptr;

        for(auto &rect : rects)
        {
            float waste = area(rect.size) - area(image.size);
            if(waste < bestWaste)
            {
                bestWaste = waste;
                bestRect = &rect;
            }
        }
        return bestRect;
    }

    void SplitRect(const ImageData &image)
    {
        std::vector<Rect> rectsToAdd;
        std::vector<size_t> rectsToRemove;

        for(size_t i = 0; i < rects.size(); i++)
        {
            auto &rect = rects[i];
            if(!quadsIntersect(rect.position, rect.size, image.position, image.size))
            {
                std::cout << "aa\n";
                continue;
            }

            // Left Rect
            float leftDist = image.position.x - rect.position.x;
            if(leftDist > 0)
                rectsToAdd.push_back(Rect(
                    glm::vec2(rect.position),
                    glm::vec2(leftDist, rect.size.y)));

            // Top Rect
            float topDist = image.position.y - rect.position.y;
            if(topDist > 0)
                rectsToAdd.push_back(Rect(
                    glm::vec2(rect.position),
                    glm::vec2(rect.size.x, topDist)));

            // Right Rect
            float rightDist = rect.size.x - image.size.x;
            if(rightDist > 0)
                rectsToAdd.push_back(Rect(
                    glm::vec2(image.size.x, rect.position.y),
                    glm::vec2(rightDist, rect.size.y)));

            // Bottom Rect
            float bottomDist = rect.size.y - image.size.y;
            if(bottomDist > 0)
                rectsToAdd.push_back(Rect(
                    glm::vec2(rect.position.x, image.size.y),
                    glm::vec2(rect.size.x, bottomDist)));

            std::cout << "bottom: " << bottomDist << "\n";
            std::cout << "right: " << rightDist << "\n";
            std::cout << "left: " << leftDist << "\n";
            std::cout << "top: " << topDist << "\n";

            std::cout << "image size: " << image.size.x << " " << image.size.y << "\n";
            std::cout << "image position: " << image.position.x << " " << image.position.y << "\n";

            rectsToRemove.push_back(i);
        }

        for(auto &rect : rectsToAdd)
        {
            std::cout << "adding\n";
            rects.push_back(rect);
        }

        // Erase in reverse order so indices stay valid
        for(int i = rectsToRemove.size() - 1; i >= 0; i--)
        {
            rects.erase(rects.begin() + rectsToRemove[i]);
        }

        PruneContainingRects();
    }

    void PruneContainingRects()
    {
        std::vector<size_t> rectsToRemove;

        for(size_t i = 0; i < rects.size(); i++)
        {
            // Check if rect i is completely contained in any other rect
            for(size_t j = 0; j < rects.size(); j++)
            {
                if(i == j)
                    continue;

                if(rectContainedIn(rects[i], rects[j]))
                {
                    rectsToRemove.push_back(i);
                    break; // Only add i once
                }
            }
        }

        // Remove duplicates (shouldn't be any with the break above, but be safe)
        std::sort(rectsToRemove.begin(), rectsToRemove.end());
        rectsToRemove.erase(std::unique(rectsToRemove.begin(), rectsToRemove.end()), rectsToRemove.end());

        // Erase in reverse order so indices stay valid
        for(int i = rectsToRemove.size() - 1; i >= 0; i--)
        {
            rects.erase(rects.begin() + rectsToRemove[i]);
        }
    }
};

TextureAtlas
AssetLoader<TextureAtlas>::Load(const TextureAtlas::LoaderConfig &config)
{
    TextureAtlas atlas;
    std::vector<ImageData> imageData;

    // Load all images
    for(auto texturePath : getPNGFilesRecursive(config.path))
    {
        int width, height, nrChannels;
        bool loaded = stbi_info(texturePath.c_str(), &width, &height, &nrChannels);
        if(loaded)
            imageData.push_back(ImageData(texturePath, glm::vec2(0, 0), glm::vec2(width, height)));
    }

    MaxRectsPacker packer(glm::vec2(2000, 2000));
    bool packed = packer.Pack(imageData);
    assert(packed && "Failed to pack images");

    int outChannels = 4;
    std::vector<unsigned char> out_img(packer.size.x * packer.size.y * outChannels, 0);

    for(auto &image : packer.packedImages)
    {
        int width, height, nrChannels;
        unsigned char *imageData = stbi_load(image.path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

        if(imageData == nullptr)
        {
            std::cerr << "Failed to load image: " << image.path << std::endl;
            continue;
        }

        for(int row = 0; row < image.size.y; ++row)
        {
            int src_offset = row * image.size.x * outChannels;
            int dst_offset = ((image.position.y + row) * packer.size.x + image.position.x) * outChannels;

            std::memcpy(
                out_img.data() + dst_offset,
                imageData + src_offset,
                image.size.x * outChannels);
        }

        stbi_image_free(imageData);
    }

    // Save atlas image
    stbi_write_png(
        std::format("{}/atlas.png", config.path).c_str(),
        static_cast<int>(packer.size.x),
        static_cast<int>(packer.size.y),
        outChannels,
        out_img.data(),
        static_cast<int>(packer.size.x * outChannels));

    // Load the atlas texture
    atlas.texture = AssetLoader<Texture>::Load({.path = std::format("{}/atlas.png", config.path)});

    return atlas;
}
