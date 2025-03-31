#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include <dirent.h>
#include <vector>

class AssetManager {
    std::map<std::string, ALLEGRO_BITMAP *> bitmaps;
    std::string assetsPath;

public:
    explicit AssetManager(std::string path) : assetsPath(std::move(path)) {
        if (!assetsPath.empty() && assetsPath.back() != '/')
            assetsPath += '/';
    }

    ~AssetManager() {
        for (auto &pair: bitmaps)
            al_destroy_bitmap(pair.second);
        bitmaps.clear();
    }

    bool loadAll() {
        std::vector<std::string> files;

#ifdef _WIN32
        // Implementação para Windows
        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile((assetsPath + "*").c_str(), &findFileData);

        if (hFind == INVALID_HANDLE_VALUE)
            return false;

        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                files.push_back(findFileData.cFileName);
        } while (FindNextFile(hFind, &findFileData));

        FindClose(hFind);
#else
        // Implementação para Linux/Mac
        if (DIR *dir; (dir = opendir(assetsPath.c_str())) != nullptr) {
            dirent *ent;
            while ((ent = readdir(dir)) != nullptr) {
                if (std::string filename = ent->d_name; filename != "." && filename != "..")
                    files.push_back(filename);
            }
            closedir(dir);
        } else
            return false;
#endif

        for (const auto &file: files) {
            if (const auto extension = file.substr(file.find_last_of(".") + 1);
                extension == "png" || extension == "jpg" || extension == "jpeg" || extension == "bmp") {
                auto fullPath = assetsPath + file;

                if (ALLEGRO_BITMAP *bmp = al_load_bitmap(fullPath.c_str())) {
                    // Usa o nome do arquivo sem extensão como chave
                    auto key = file.substr(0, file.find_last_of("."));
                    bitmaps[key] = bmp;
                } else
                    fprintf(stderr, "Failed to load bitmap '%s'\n", fullPath.c_str());
            }
        }

        return true;
    }

    ALLEGRO_BITMAP *getBitmap(const std::string &name) {
        if (const auto it = bitmaps.find(name); it != bitmaps.end())
            return it->second;
        return nullptr;
    }

    void addBitmap(const std::string &name, ALLEGRO_BITMAP *bitmap) {
        if (bitmap && !name.empty())
            bitmaps[name] = bitmap;
    }
};

#endif //ASSETS_MANAGER_H
