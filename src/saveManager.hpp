#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

enum class eDataPosition{
    LevelsUnlocked,
    Currency,
    UpgradeData,
    HighScore
};

class SaveManager {
public:
    SaveManager() 
        : filename("saveFile.data") 
    {}

    ~SaveManager() = default;

    void writeData(eDataPosition pos, int value)    {
        auto data = loadAllData();
        
        switch (pos) {
        case eDataPosition::LevelsUnlocked:
            data[0] = value;
            break;
        case eDataPosition::Currency:
            data[1] = value;
            break;
        case eDataPosition::HighScore:
            data[3] = value;
            break;
        default:
            break;
        }
        
        saveAllData(data);
    }
    
    void writeStringData(eDataPosition pos, const std::string& value) {
        auto data = loadAllData();
        
        if (pos == eDataPosition::UpgradeData) {
            data[2] = 0; // Placeholder, actual string saved separately
            
            std::ofstream file(filename, std::ios::out | std::ios::trunc);
            if (!file.is_open()) return;
            
            // Write integers
            for (size_t i = 0; i < data.size(); ++i) {
                file << data[i];
                if (i < data.size() - 1) file << " ";
            }
            file << "\n";
            
            // Write upgrade string
            file << value;
            file.close();
        }
    }
    
    std::string readStringData(eDataPosition pos) {
        if (pos != eDataPosition::UpgradeData) return "";
        
        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) return "";
        
        std::string line;
        // Skip first line (integers)
        std::getline(file, line);
        // Read upgrade data
        std::getline(file, line);
        file.close();
        
        return line;
    }

    int readData(eDataPosition pos) {
        auto data = loadAllData();
        
        switch (pos) {
        case eDataPosition::LevelsUnlocked:
            return data[0];
        case eDataPosition::Currency:
            return data[1];
        case eDataPosition::HighScore:
            return data[3];
        default:
            return 0;
        }
    }

private:
    std::string filename;
    
    std::vector<int> loadAllData() {
        std::vector<int> data(4, 0); // levelsUnlocked, currency, upgradeDataPlaceholder, highScore
        
        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) {
            // Create default file
            std::ofstream create(filename, std::ios::out);
            if (create.is_open()) {
                create << "0 0 0 0\n";
                create.close();
            }
            return data;
        }
        
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        
        for (size_t i = 0; i < data.size() && iss >> data[i]; ++i) {}
        
        file.close();
        return data;
    }
    
    void saveAllData(const std::vector<int>& data) {
        std::string upgradeData = readStringData(eDataPosition::UpgradeData);
        
        std::ofstream file(filename, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Failed to save data" << std::endl;
            return;
        }
        
        for (size_t i = 0; i < data.size(); ++i) {
            file << data[i];
            if (i < data.size() - 1) file << " ";
        }
        file << "\n";
        
        if (!upgradeData.empty()) {
            file << upgradeData;
        }
        
        file.close();
    }
};