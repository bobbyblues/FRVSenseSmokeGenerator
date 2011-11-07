#include "VSQReader.h"
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sstream>

VSQReader::VSQReader(std::string path):
    m_path(path),
    m_cur(0)
{

    // We check if the extension is correct
    std::string ext = path.substr(path.size()-4-1,4);

    m_stream.open(path.c_str(), std::ifstream::in | std::ifstream::binary);

    std::cout << "Opening of the file " << path.c_str() << std::endl;

    if (!strcmp(ext.c_str(),".vsq")){
        std::cout << "[Warning] Wrong file extension (.vsq needed)" << std::endl;
    }
    if (!m_stream.is_open()){
        std::cerr << "[Error] Unable to open file " << m_path << std::endl;
        return;
    }

    m_stream.read((char*)&m_cubeSize, sizeof(int));
    m_stream.read((char*)&m_nbFrame, sizeof(int));

    m_stream.seekg(0, std::ios::end);

    m_size.push_back(m_stream.tellg());

    m_stream.seekg(0);

}

bool VSQReader::readFile(unsigned char number){
    if (number == m_cur)
        return true;
    if (number == 0){
        m_stream.open(m_path.c_str(), std::ifstream::in | std::ifstream::binary);
    }
    else{
        std::string curPath = m_path;
        std::stringstream str;
        str << curPath.substr(0,curPath.size()-4);
        str << "#";
        if (number < 10)
            str << "0";
        str << number;
        str << curPath.substr(curPath.size()-4-1,4);
        curPath = str.str();
        m_stream.open(curPath.c_str(), std::ifstream::in | std::ifstream::binary);
    }
    if (!m_stream.is_open()){
        std::cerr << "[Error] Unable to open file " << m_path << std::endl;
        m_cur == -1;
        return false;
    }

    // We update the size vector
    if (m_size.size() == number){
        m_stream.seekg(0, std::ios::end);
        m_size.push_back(m_stream.tellg());
        m_stream.seekg(0);
    }

    m_cur == number;

    return true;
}

float * VSQReader::readFrame(int number){
    if (number > m_nbFrame){
        std::cerr << "[Error] Frame number out of range" << std::endl;
        return NULL;
    }
    // We find in wich file the frame is in
    long frameSize = m_cubeSize * m_cubeSize * m_cubeSize * sizeof(float); // size of a frame in bytes
    long startingByte = number * frameSize + 2 * sizeof(int);
    long oldStartingByte = startingByte;
    int fileNumber = 0;
    do{
        if(m_size.size() < fileNumber)
            readFile(fileNumber);
        oldStartingByte = startingByte;
        startingByte -= m_size[fileNumber];

    }while(startingByte >= 0);


    readFile(fileNumber);

    float*  result = new float[frameSize];
    memset(result,1,frameSize);

    long readSize = m_size[fileNumber] - oldStartingByte;
    m_stream.seekg(oldStartingByte);
    readSize = std::min(readSize,frameSize);
    m_stream.read((char*)result, readSize);

    if(readSize < frameSize){
        readFile(fileNumber+1);
        m_stream.read((char*)&result[readSize], frameSize - readSize);
    }

    return result;

}
