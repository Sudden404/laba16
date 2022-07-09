#ifndef LABA16_WAVEFMT_H
#define LABA16_WAVEFMT_H

#include <iostream>
#include <fstream>
#include <cstring>


class WaveFmt {
private:
    char Riff[4];
    uint32_t RiffSize;//Размер файла после riffsize
    char Wave[4];//Тип RIFF

    char Fmt[4];//ID секции
    uint32_t FmtSize;//Размер данных секции формата
    uint16_t CompressionCode;//Код типа сжатия аудиоданных
    uint16_t NumberChannels;//Количество каналов
    uint32_t SampleRate;//Частота дискретизации
    uint32_t AverageBytesPerSec;//Количество байт в секунду
    uint16_t BlockAlign;//Размер блока
    uint16_t SignificantBitsPerSample;//Количество значащих бит на выборку

    char Data[4];//ID секции
    uint32_t DataSize;//Размер секции с самой информацией
    char* DataMas;
public:
    WaveFmt();
    ~WaveFmt();
    void Load(std::ifstream&);
    void Enter(std::ofstream&);
    void Shift(int& n);
};


#endif //LABA16_WAVEFMT_H
