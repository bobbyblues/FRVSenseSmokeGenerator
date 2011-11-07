#ifndef VSQREADER_H
#define VSQREADER_H
#include <fstream>
#include <vector>
#include <string>

class VSQReader
{
private:
    // Attributes
    std::string m_path;

    std::ifstream m_stream; // Stream of the current file
    unsigned char m_cur; // Indice of the current file
    std::vector<long> m_size; // Vector of the sizes of the files

    int m_cubeSize; // Size of the smoke
    int m_nbFrame; // Number of frames


    // Private method

    /**
     * @brief Loads the nth file in the m_stream, updating m_size if needed
     * @pre The size vector must be up to date for all previous files (ie. if we open the 6th file, the size vector must contain the
     * the values for the 6 previous files, from 0 to 5)
     * @post The size vector is updated
     */
    bool readFile(unsigned char number);
public:
    VSQReader(std::string path);
    ~VSQReader(){};

    int getSize(){return m_cubeSize;}
    int getNbFrame(){return m_nbFrame;}

    float * readFrame(int number);

};

#endif // VSQREADER_H
