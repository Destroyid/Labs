#include <iostream>
#include <fstream>
#include <clocale>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cmath>

using namespace std;

void delimeters(size_t x){ // ������� ������ ��������� �����
  for(size_t i=4;i<x;i++){
      if(x%i==0){
          cout << i << endl;
        }
    }
  cout << endl;
}

void encode(size_t left_index, size_t right_index ,vector<string> &codes,vector<size_t> &ptable) // ������� ������ ����� ������-����
{
  size_t i;
  long left_index_pos;
  size_t tmp;
  size_t pfull;
  size_t phalf;
  if(left_index == 0 && right_index == 0 && ptable.size() == 1)
    codes[0] = "1";
  if( left_index == right_index )
    return;
  else if( right_index - left_index == 1 )
    {
      //  ���� �������� ����� 2 ��������
      codes[left_index] += '0';
      codes[right_index] += '1';
    }
  else
    {
      //  ���������� ����� ������������ �� ������������� ���������
      pfull = 0;
      for( i=left_index; i<=right_index; ++i )
        pfull += ptable[i];

      //  ����� ������
      tmp = 0;
      left_index_pos = -1; // ������ ����� �������
      phalf = pfull/2;
      for( i=left_index; i<=right_index; ++i )
        {
          tmp += ptable[i];
          if(tmp <= phalf)
            codes[i] += '0';
          else
            {
              codes[i] += '1';
              if( left_index_pos < 0 )
                left_index_pos = i;
            }
        }

      if( left_index_pos < 0 )
        left_index_pos = left_index+1;

      //  ���������(�����������) ���
      encode( left_index, left_index_pos-1 , codes , ptable);
      encode( left_index_pos, right_index , codes , ptable);
    }
}

bool comp(string x,string y){ // ����������
  return x>y;
}

size_t partition(pair<vector<string>,vector<size_t>> &tmp,long low_index,long high_index){ // ������� ��� ������� ����������(quicksort)
  size_t x = tmp.second[high_index];
  long i = (low_index - 1);

  for (int j = low_index; j <= high_index - 1; j++) {
      if (tmp.second[j] <= x) {
          i++;
          swap(tmp.second[i],tmp.second[j]);
          tmp.first[i].swap(tmp.first[j]);
        }
    }
  swap(tmp.second[i+1],tmp.second[high_index]);
  tmp.first[i+1].swap(tmp.first[high_index]);
  return (i + 1);
}

void quicksort_(pair<vector<string>,vector<size_t>> &tmp,long low_index,long high_index){ // ������� ������� ����������(quicksort)
  if (low_index < high_index) {
      long p = partition(tmp, low_index, high_index);
      quicksort_(tmp, low_index, p - 1);
      quicksort_(tmp, p + 1, high_index);
    }
}

int main()
{
  std::ios::sync_with_stdio(false);
  setlocale(0,"");
  ifstream input_file("D:\\Labs\\input.txt",ios_base::binary);// ������� ����
  ofstream output_file("D:\\Labs\\output.txt",ios_base::trunc|ios_base::binary); // �������� ����
  string buffer,input,output;
  size_t count_01;
  vector<string> to_check;
  vector<string> to_insert;
  if(input_file.fail() or output_file.fail()){
      cout << "�������� ���� �� �������� ��� ��������� �����" << endl;
      return 1;
    }

  input_file.seekg(0,ios::end);
  size_t len = input_file.tellg();
  input_file.seekg(0,ios::beg);
  input.resize(len);
  input_file.read((char*)&input[0],len);
  input_file.close();
  {
    string tmp;
    for(size_t i=0;i<input.size();i++){
        bitset <8> bits(input[i]);
        tmp+=bits.to_string();
      }
    input = tmp;
  }// �������� � string input ����(� ���� 01)
  len = input.size(); // ���������������� ���������

  cout << "������(�����) : " << input.size()/8 << endl;
  cout << "������(����) : " << input.size() << endl;
  cout << "��������� ��������� ������� ������� ���� : " << endl;
  delimeters(len);
  cout << "������� ������ �������� ����� : ";
  do{
      cin >> count_01;
      if(count_01<4 || len%count_01!=0)
        cout << "������� ������ �������� �����, ������� ����� >4 " << endl;
    }while(count_01<4 || len%count_01!=0);

  for(size_t i=0;i<input.size();i+=count_01){
      buffer.clear();
      for(size_t k=0;k<count_01;k++){
          buffer+=input[i+k];
        }
      to_check.push_back(buffer);
      to_insert.push_back(buffer);
    } // ���� ��� ������ �������� ����� to_check(��� ��������),to_insert(��������� ������������ ������� ����������)
  std::sort(to_check.begin(),to_check.end(),comp); // ����������
  //cout << "Sort to_check ended " << endl;

  pair<vector<string>,vector<size_t>> not_coded; // ������� ������������ ��� ������� ����������
  string tmp_bits;
  size_t iter_bits = 0;
  for(size_t i=0;i<to_check.size();i++){
      tmp_bits = buffer;
      buffer = to_check[i];
      if(not_coded.first.size()==0){
          not_coded.first.push_back(buffer);
          not_coded.second.push_back(1);
          tmp_bits = buffer;
        }
      else{
          if(tmp_bits == buffer)
            not_coded.second[iter_bits]++;
          else{
              not_coded.first.push_back(buffer);
              iter_bits++;
              not_coded.second.push_back(1);
            }
        }
    }

  //cout << "Count of bits ended " << endl;

  quicksort_(not_coded,0,not_coded.first.size()-1); // ���������� �� ����������� ������������ ������� � ������������ �������� ������������

  //cout << "Sort not_coded ended " << endl;
  pair<vector<string>,vector<string>> codes; // ���� �������� �����: ��������-���� ������-����
  codes.first.swap(not_coded.first);
  codes.second.resize(codes.first.size());
  encode(0,codes.first.size()-1,codes.second,not_coded.second); // ��������� ����� ������-����

  cout << endl << "������� �������-�������� �������" << endl; // ����� �������� � ��������� �������
  for(size_t i=0;i<codes.first.size();i++){
      cout << codes.first[i] << "->" << codes.second[i] << endl;
    }

  //cout << "Binary 00 and 01 ended " << endl;

  string pre_output; // ������ � ������� ����� ���� ������-����
  for(vector<string>::iterator it1=to_insert.begin();it1!=to_insert.end();it1++){
      for(vector<string>::iterator it2=codes.first.begin(),it3=codes.second.begin();it2!=codes.first.end();it2++,it3++){
          if(*it1==*it2){
              pre_output+=*it3;
              break;
            }
        }
    }

  //cout << "Pre_output was created " << endl;
  buffer.clear();

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

  //cout << "file was wrote " << endl;
  cout << "������ ��������� �����(����) : "<< len << endl;
  cout << "������ ��������� �����(����) : " << pre_output.size() << endl;
  cout << "������� ������ : " << float(float(len)/float(pre_output.size())) << endl;
  output_file.close(); // �������� ��������� �����

  ofstream config("D:\\Labs\\config.txt",ios_base::trunc|ios_base::binary);
  if(output_[output_.size()-1].size()<8 || output_[output_.size()-1].size()==8) // ���������� ������������ 0 � ��������� �����
    config << 8-output_[output_.size()-1].size() << endl;
  config << codes.first.size() << endl; // ���������� �����
  for(size_t i=0;i<codes.first.size();i++){ // ������ �����
      config << codes.first[i] << "-" << codes.second[i] << endl;
    }
  config.close(); // �������� ����� ������������
}
