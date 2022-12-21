#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

class complex {
public:
  double mem_a, mem_b, fy;

  complex() {
    mem_a = 0;
    mem_b = 0;
    fy = 0;
  }

  double getAbs() {
    return sqrt(pow(mem_a,2)+pow(mem_b,2));
  }

};

#define macro_F 3*cos(6*M_PI*x);

int main() {
  setlocale(0, "");
  cout << "Функция 3*cos(6*M_PI*x)";
  double t0;
  cout << " Введите t0: ";
  cin >> t0;
  double T;
  cout << "Введите Т: ";
  cin >> T;
  double N;
  cout << "Введите N: ";
  cin >> N;
  double v = N * T;
  double* func = new double[size_t(N)];

  for (int i = 0; i < N; i++) {
      double x = t0 + T / (N - 1) * i;
      func[i] = macro_F;
    }

  complex* Furie = new complex[size_t(N+1)];
  for (int i = 0; i < N + 1; i++) {
      for (int j = 0; j < N; j++) {
          Furie[i].mem_a += func[j] * cos(2.0 * M_PI * (double)i * (double)j / N);
          Furie[i].mem_b += func[j] * sin(2.0 * M_PI * (double)i * (double)j / N);
        }
      Furie[i].fy = v * i;
    }
  delete[] func;
  cout << "  +----------+---------+----------+----------+" << endl;
  cout << "N |Амплитуда | Частота |Коэф. а(k)|Коэф. b(k)|" << endl;
  cout << "  |----------|---------|----------|----------|" << endl;
  for (int i = 0; i < (N + 1 ) / 2 ; i++) {
      cout << i+1 << " ";
      printf("|%10.4f|%9.0f|%10.4f|%10.4f| \n", Furie[i].getAbs() / N * 2, Furie[i].fy, Furie[i].mem_a / N * 2,
             Furie[i].mem_b / N * 2);
      cout  << "  |----------|---------|----------|----------|";

      cout << endl;
    }
  delete[] Furie;
  return 0;
}
