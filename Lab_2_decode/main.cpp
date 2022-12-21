#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <bitset>
#include <cmath>

using namespace std;

int main()
{
  ifstream input_file("D:\\Labs\\output.txt",ios_base::binary); // ������� ����(�������� ��������)
  ofstream output_file("D:\\Labs\\output_decode.txt",ios_base::trunc|ios_base::binary); // �������� ���� ��������
  ifstream config_file("D:\\Labs\\config.txt",ios_base::binary); // ���������������� ����, ��������� ���������
  string input,config;

  input_file.seekg(0,ios::end);
  size_t len_input = input_file.tellg();
  input_file.seekg(0,ios::beg);
  input.resize(len_input);
  input_file.read((char*)&input[0],len_input);
  input_file.close(); // ��������� � ������� ������� ����

  config_file.seekg(0,ios::end);
  size_t len_config = config_file.tellg();
  config_file.seekg(0,ios::beg);
  config.resize(len_config);
  config_file.read((char*)&config[0],len_config);
  config_file.seekg(0); // ��������� ���������������� ���� ��� ��������� ����������

  pair<vector<string>,vector<string>> codes;//������� �������-�������� �������

  int count_to_ignore = config[0]-'0'; // ���������� ���������� ������������ 0 � ��������� �����
  size_t count = 0;
  string buffer;
  for(size_t i=2;i<config.size();i++) // ����������� ���������� �����
    if(config[i]-'0'>= 0 && config[i]-'0'<=9)
      buffer+=config[i];
    else{
      count = stoull(buffer);
      break;
    }
  buffer.clear();
  config.clear();
  codes.first.resize(count);
  codes.second.resize(count);
  {                           // ������ ������������ ����� ������������ � ��������� � ���������: ������� �������-�������� �������
    vector<string> temp;
    temp.resize(count+2);
    size_t i=0;
    while(!config_file.eof()){
      config_file >> temp[i];
      i++;
    }
    bool check = false;
    for(size_t i=2,k=0;i<temp.size();i++,k++){
      check = false;
      for(size_t j=0;j<temp[i].size();j++){
        if(temp[i][j]=='-')
          check=true;
        else{
          if(check==false)
            codes.first[k]+=temp[i][j];
          else
            codes.second[k]+=temp[i][j];
        }
      }
    }
  } // ����� ������������ ����� ������������ � ��������� � ���������: ������� �������-�������� �������
  for(size_t i=0;i<codes.first.size();i++){
      cout << codes.first[i] << "-" << codes.second[i] << endl;
    }
  for(size_t i=0;i<len_input;i++){ // ������������ ���� ����� �� �������� � 01 � �� ���������� ������������ 0
    bitset<8> bite(input[i]);
    if(len_input-1==i){
      string tmp = bite.to_string();
      for(int j=0;j<count_to_ignore;j++){
        for(size_t k=0;k<8;k++){
          tmp[k] = tmp[k+1];
        }
      }
      tmp.resize(8-count_to_ignore);
      buffer+=tmp;
    }else{
      buffer+=bite.to_string();
    }
  }
  input.swap(buffer);

  buffer.clear();
  string pre_output; // �������� ������ � ������� ����� �������������� ������� ������������������
  for(size_t i=0;i<input.size();i++){
    buffer+=input[i];
    for(size_t j=0;j<codes.second.size();j++){
      if(buffer == codes.second[j]){
        pre_output+=codes.first[j];
        buffer.clear();
        break;
      }
    }
  }

  vector<string> output_; // ���������� � ������ � ����(������ �����)
  output_.resize((size_t)ceil(double(pre_output.size())/8.0f));
  for(size_t i=0,k=0;i<pre_output.size();i++){
      output_[k].push_back(pre_output[i]);
      if(output_[k].size()==8)
        k++;
    }

  for(size_t i=0;i<output_.size();i++){ // ���������� � ������ � ����
    bitset<8> bit(output_[i]);
    output_file << char(bit.to_ulong());
  }
  return 0;
}
