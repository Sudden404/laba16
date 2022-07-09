#include "WaveFmt.h"
//сдвиг в секундах а не отсчётах
int main(int argc, char** argv) {
    int i = atoi(argv[3]);
    std::cout << i << std::endl;
    if (argc != 4) {
        std::cout<<"wrong number of arguments"<<std::endl;
        return -1;
    }
    std::ifstream fin(argv[1], std::ios_base::binary);
    if(!fin.is_open()) {
        std::cout<<"error with read file"<<std::endl;
        exit(0);
    }
    std::ofstream fout(argv[2], std::ios_base::binary);
    if(!fout.is_open()) {
        std::cout<<"error with recording file"<<std::endl;
        exit(0);
    }
    WaveFmt f;
    f.Load(fin);
    f.Shift(i);
    f.Enter(fout);
    std::cout << "successfully" << std::endl;
    return 0;
}