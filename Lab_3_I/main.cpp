#include <bitset>
#include <math.h>
#include <algorithm>
#include <locale>
#include <ios>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int XorCheck(string &str);
void Encode();
void Decode();

int XorCheck(string& str){
    int Count_1 = 0;
    for(size_t i=0;i<8;i++){
        if(str[i]=='1'){
            Count_1++;
        }
    }
    return Count_1%2;
}

void Encode(){
    string Data;
    vector<string> bits;
    ifstream in("D:\\Projects\\Lab_3\\File\\i.txt",ios_base::binary);
    in.seekg(0,ios::end);
    size_t len = in.tellg();
    in.seekg(0,ios::beg);
    Data.resize(len);
    in.read((char*)&Data[0],len);
    in.close();
    for(const auto &c:Data){
        bitset<8> bite(c);
        bits.push_back(bite.to_string());
    }
    Data.clear();
    string PreOut;
    for(size_t i=0;i<bits.size();i++){
        if(XorCheck(bits[i]) == 1){
            bits[i].append("1");
        }
        else{
            bits[i].append("0");
        }
        PreOut+=bits[i];
    }
    bits.clear();
    ofstream out("D:\\Projects\\Lab_3\\File\\o.txt",ios_base::binary | ios_base::trunc);
    ofstream ExBits("D:\\Projects\\Lab_3\\File\\config.txt",ios_base::binary | ios_base::trunc);
    ExBits << (PreOut.size() - ceil(PreOut.size()/8)*8) << "\n";
    ExBits.close();
    for(size_t i=0,k=1;i<PreOut.size();i++,k++){
        Data+=PreOut[i];
        if(k==8){
            bitset<8> TMP(Data);
            k=0;
            Data.clear();
            out << char(TMP.to_ulong());
        }
    }
    if(Data.size() != 0){
        bitset<8> TMP(Data);
        out << char(TMP.to_ulong());
    }
    out.close();
}

void Decode(){
    ifstream in("D:\\Projects\\Lab_3\\File\\o.txt",ios_base::binary);
    ofstream out("D:\\Projects\\Lab_3\\File\\decode.txt",ios_base::binary | ios_base::trunc);
    string Data;
    ifstream extra("D:\\Projects\\Lab_3\\File\\config.txt",ios_base::binary);
    extra >> Data;
    extra.close();
    size_t ExBits = stoi(Data);
    Data.clear();
    vector<string> bits;
    in.seekg(0,ios::end);
    size_t len = in.tellg();
    in.seekg(0,ios::beg);
    Data.resize(len);
    in.read((char*)&Data[0],len);
    in.close();
    if(ExBits!=0){
        Data.erase(0,Data.find_first_of("\n")+1);
    }

    string TMP;
    for(const auto &c:Data){
        bitset<8> bite(c);
        TMP+=bite.to_string();
    }
    Data.swap(TMP);
    TMP.clear();

    if(ExBits!=0){
        for(size_t i=Data.size()-1,j=0;j<ExBits;j++,i--){
            TMP+=Data[i];
        }
        Data.erase(Data.size()-8,Data.size());
        reverse(TMP.begin(),TMP.end());
        Data += TMP;
    }

    TMP.clear();
    for(size_t i=0,j=0;i<Data.size();i++,j++){
        TMP+=Data[i];
        if(j%8==0 and j != 0){
            bits.push_back(TMP);
            TMP.clear();
            j = -1;
        }
    }
    vector<int> Check;
    for(size_t i=0;i<bits.size();i++){
        if(XorCheck(bits[i]) != (bits[i][8] == '1' ? 1 : 0))
            Check.push_back(i+1);
        bits[i].resize(8);
        bitset<8> bite(bits[i]);
        out << char(bite.to_ulong());
    }
    for(const auto &c:Check)
        cout << "Cимвол находится не на своей позиции: " << c << endl;
}

int main()
{
    setlocale(LC_ALL,"rus");

    Encode();
    Decode();
}
