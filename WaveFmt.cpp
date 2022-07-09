#include "WaveFmt.h"
void error(const char* massage) {
    std::cout << massage << std::endl;
    exit(1);
}
WaveFmt::WaveFmt() {
    DataMas = nullptr;
}
WaveFmt::~WaveFmt() {
    delete[] DataMas;
}
void WaveFmt::Load(std::ifstream& fin) {
    fin.read(Riff,sizeof(Riff));
    if (strncmp(Riff,"RIFF",4))
        error("Not Riff");
    fin.read((char*)&RiffSize,sizeof(RiffSize));
    fin.read(Wave,sizeof(Wave));
    if (strncmp(Wave,"WAVE",4))
        error("Not Wave");
    fin.read(Fmt,sizeof(Fmt));
    if (strncmp(Fmt,"fmt ",4))
        error("Not Fmt");
    fin.read((char*)&FmtSize,sizeof(FmtSize));
    fin.read((char*)&CompressionCode,sizeof(CompressionCode));
    if (CompressionCode != 1)
        error("Not PCM");
    fin.read((char*)&NumberChannels,sizeof(NumberChannels));
    fin.read((char*)&SampleRate,sizeof(SampleRate));
    fin.read((char*)&AverageBytesPerSec,sizeof(AverageBytesPerSec));
    fin.read((char*)&BlockAlign,sizeof(BlockAlign));
    fin.read((char*)&SignificantBitsPerSample,sizeof(SignificantBitsPerSample));
    fin.seekg(FmtSize+20,std::ios_base::beg);
    char buf[4];
    uint32_t SkipSize;
    int i;
    for(i = 40;i > 0;i--) {
        fin.read(buf,sizeof(buf));
        if (strncmp(buf,"data",4) != 0) {
            fin.read((char*)&SkipSize,sizeof(SkipSize));
            fin.seekg(SkipSize,std::ios_base::cur);
        }
        else
            break;
    }
    if (i == 0)
        error("Data chunk");
    memcpy(Data,buf,4);
    fin.read((char*)&DataSize,sizeof(DataSize));
    DataMas = new char [DataSize];
    fin.read(DataMas, DataSize);
}

void WaveFmt::Enter(std::ofstream &fout) {
    fout.write(Riff,4);
    fout.write((char*)&RiffSize,sizeof(RiffSize));
    fout.write(Wave,4);
    fout.write(Fmt,4);
    fout.write((char*)&FmtSize,sizeof(FmtSize));
    fout.write((char*)&CompressionCode,sizeof(CompressionCode));
    fout.write((char*)&NumberChannels,sizeof(NumberChannels));
    fout.write((char*)&SampleRate,sizeof(SampleRate));
    fout.write((char*)&AverageBytesPerSec,sizeof(AverageBytesPerSec));
    fout.write((char*)&BlockAlign,sizeof(BlockAlign));
    fout.write((char*)&SignificantBitsPerSample,sizeof(SignificantBitsPerSample));

    fout.write(Data,4);
    fout.write((char*)&DataSize,sizeof(DataSize));
    fout.write(DataMas, DataSize);
}

void WaveFmt::Shift(int& n) {
    if (n == 0)
        return;
    n *= SampleRate;
    if ((n < 0) && ((n * -1) >= DataSize)) {
        DataSize = 1;
        RiffSize -= (DataSize - 1);
        delete [] DataMas;
        DataMas = new char [1];
        DataMas[0] = 0;
        return;
    }

    char* tmp = new char [DataSize+n];
    for(int i = 0;i<n;i++)
        tmp[i] = 0;
    if (n > 0){
        for(int i = 0;i < DataSize;i++) {
            tmp[i+n] = DataMas[i];
        }
    }
    else {
        for(int i = 0;i < DataSize + n;i++) {
            tmp[i] = DataMas[i-n];
        }
    }
    DataSize += n;
    RiffSize += n;
    delete [] DataMas;
    DataMas = tmp;
}