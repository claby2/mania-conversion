#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct hitObject {
    int x;
    int y;
    int time;
};

hitObject getObjectInfo(std::string line) {
    int objectInfo[3];
    std::string sub;
    int commaCount = 0;
    hitObject object;

    for(int i = 0; i < line.length(); i++) {
        if(line[i] == ',') {
            objectInfo[commaCount] = std::stoi(sub);
            sub.clear();
            commaCount++;
            if(commaCount == 3) {
                break;
            }
            continue;
        }
        sub += line[i];
    }

    object.x = objectInfo[0];
    object.y = objectInfo[1];
    object.time = objectInfo[2];

    return object;
}

std::vector<hitObject> getHitObjects(std::string fileLocation) {
    std::vector<hitObject> hitObjects;
    std::string line;
    bool record = false;
    std::ifstream mapfile(fileLocation);
    if(mapfile.is_open()) {
        while(getline(mapfile, line)) {
            if(line == "[HitObjects]") {
                record = true;
            } else if(record) {
                hitObjects.push_back(getObjectInfo(line));
            }
        }
        mapfile.close();
    }

    return hitObjects;
}
