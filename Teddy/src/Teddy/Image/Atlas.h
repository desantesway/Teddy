#pragma once

namespace Teddy::Atlas
{
    void Generate(const std::vector<std::string>& filepaths,
        int maxWidth, int maxHeight, int toleration,
        const std::string& outputPrefix);
}