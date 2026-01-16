#include <iostream>
#include <string>
#include <fstream>

enum class eDataPosition{
    LevelsUnlocked
};

class SaveManager {
public:
    SaveManager() 
        : filename("saveFile.data") 
    {}

    ~SaveManager() = default;

    void writeData(eDataPosition pos, int value)    {
        std::fstream file(filename, std::ios::out | std::ios::trunc);

        if (!file.is_open())    {
            std::cerr << "failed to save data (could not open file): " << filename << std::endl;
            return;
        }

        switch (pos)
        {
        case eDataPosition::LevelsUnlocked:
            file << value;
            break;
        }

        file.close();
    }

    int readData(eDataPosition pos) {
        std::fstream file(filename, std::ios::in);
        int value = 0;

        if (!file.is_open())    {
            std::ofstream create(filename, std::ios::out);
            if(create.is_open())    {
                create << 0;
                create.close();
            } 
            else {
                throw std::runtime_error("Failed to create save file");
            }
            return 0;
        }
        switch (pos)
        {
        case eDataPosition::LevelsUnlocked:
            file >> value;
            break;
        }

        file.close();
        return value;
    }

private:
    std::string filename;
};