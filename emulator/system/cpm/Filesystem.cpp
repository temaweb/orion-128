//
//  Filesystem.cpp
//  orion
//
//  Created by Артём Оконечников on 21.11.2020.
//

#include "Filesystem.hpp"
#include "Environment.hpp"

#include <fstream>
#include <vector>
#include <cstdint>

std::vector<uint8_t> Filesystem::read(std::string path)
{
    auto file = Environment::openBinaryFile(path);
    char buffer = 0x00;
    
    std::vector<uint8_t> content;
    while (!file.eof())
    {
        file.read(&buffer, sizeof(buffer));
        content.push_back(buffer);
    }
    
    file.close();
    
    return content;
}

void Filesystem::create(std::string path)
{
    auto content = read(path);
    uint8_t * ptr = content.data();

    size_t offset = 0;
    size_t size = content.size();

    int len = (ptr[0x0b] << 8) | ptr[0x0a];;
    if (len == 0)
    {
        offset = 0x4d;

        if (size < offset + 32) {
            return;
        }

        ptr += offset;
        len = (ptr[0x0b] << 8) | ptr[0x0a];
    }

    len = (((len - 1) | 0xf ) + 17);
    
    for (uint16_t i = 0; i < len; i++)
    {
        memory -> writeB(i, *ptr++);
    }
    
    memory -> writeB(len, 0xff);
}
