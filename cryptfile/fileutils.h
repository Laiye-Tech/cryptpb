#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdio>
 
using std::string;
using std::vector;
using std::ios;
using std::ifstream;
using std::ofstream;
 
class FileUtils
{
public:
    static std::string read(const std::string& filename)
    {
        ifstream ifs(filename.c_str(), ios::in | ios::binary | ios::ate);
 
        ifstream::pos_type filesize = ifs.tellg();
        ifs.seekg(0, ios::beg);
 
        string file(filesize,'\0');
        ifs.read(&file[0], filesize);
        return file;//uses NRVO to avoid copy constructor
    }
    static void write(const std::string& filename, const std::string& data)
    {
        ofstream ofs(filename.c_str(), ios::out | ios::binary | ios::trunc);
 
        ofs.write(&data[0], data.length());
        return;
    }
    static void write_append(const std::string& filename, const std::string& data)
    {
        ofstream ofs(filename.c_str(), ios::out | ios::binary | ios::app);
 
        ofs.write(&data[0], data.length());
        return;
    }
    unsigned long static filesize(const string& filename) 
    { 
        ifstream file(filename.c_str(), ios::binary | ios::ate); 
        return file.tellg(); 
    }
    bool static inline rm(const string& filename) 
    { 
        return file_exists(filename.c_str()) && remove(filename.c_str());
    }
    bool static inline file_exists(const string& filename)
    {
        ifstream f(filename.c_str());
        bool exists = f.good();
        f.close();
        return exists;
    }
    void static inline rand_bytes(char* buffer, int count) 
    { 
        ifstream i("/dev/urandom", ios::in|ios::binary); 
        i.read(buffer,count); 
        i.close();
    }
    string static inline rand_string(size_t length)
    {
        char *c = new char[length];
        rand_bytes(c,length); 
        for(int i=0; i<length; i++){ c[i] = ((unsigned)c[i]%25)+97; }
        string s(c,length);
        delete[] c;
        return s;
    }
    string static inline temp_filename(const string& prepend) 
    { 
        for(int i=0; i<100; i++)
        { 
            string f = prepend + rand_string(16); 
            if (!file_exists(f.c_str())) 
            { 
                return f;
            }
        } 
        return "";
    }
 
private:
    FileUtils(){};//static class
};