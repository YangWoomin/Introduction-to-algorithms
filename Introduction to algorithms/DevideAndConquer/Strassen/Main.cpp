#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <string>
#include <cmath>
#include <thread>

#include "../../Utils/PerformanceCounter/RunningTimeCounter.h"
#include "../../Utils/Progress/Progress.h"

int MakeRandomNum(int min, int max)
{
	static bool init = false;
	if (false == init)
	{
		srand((unsigned int)time(NULL));
		init = true;
	}

	return (int)rand() % (max - min + 1) + min;
}

std::size_t Log2(std::size_t n)
{
	return (std::size_t)(log(n) / log(2));
}

class Matrix
{
private:
	typedef char Value;
	Value** _values = nullptr;
	std::size_t _size = 0;
	std::size_t _n = 0;
	bool _useStrassen = false;
	std::string _name;

	void SetSizeToPowOf2()
	{
		_size = _n;
		if (false == std::_Is_pow_2(_size))
		{
			_size = (std::size_t)std::pow(2, Log2(_size) + 1);
		}
	}

	void Reset()
	{
		if (nullptr != _values)
		{
			for (std::size_t i = 0; i < _size; ++i)
			{
				if (nullptr != _values[i])
				{
					delete[] _values[i];
				}
			}

			delete[] _values;
		}
		_size = 0;
		_n = 0;
		_useStrassen = false;
	}

	void Copy(const Matrix& other)
	{
		_size = other._size;
		_n = other._n;
		_useStrassen = other._useStrassen;
		_name = other._name;

		_values = new Value * [_size];
		for (std::size_t i = 0; i < _size; ++i)
		{
			_values[i] = new Value[_size];
			memcpy(_values[i], other[i], _size);
		}
	}

public:
	// create a matrix with elements each of whose value is random
	Matrix(std::size_t n, int min, int max)
		: _n(n)
	{
		SetSizeToPowOf2();

		_values = new Value * [_size];
		for (std::size_t i = 0; i < _size; ++i)
		{
			_values[i] = new Value[_size];
			for (std::size_t j = 0; j < _size; ++j)
			{
				_values[i][j] = MakeRandomNum(min, max);
			}
		}
	}

	Matrix(std::size_t n)
		: _n(n)
	{
		SetSizeToPowOf2();

		_values = new Value * [_size];
		for (std::size_t i = 0; i < _size; ++i)
		{
			_values[i] = new Value[_size];
			memset(_values[i], 0, _size);
		}
	}

	Matrix(const Matrix& other)
	{
		Copy(other);
	}

	Matrix& operator=(const Matrix& other)
	{
		Reset();
		Copy(other);
	}

	Matrix(const Matrix&& other) noexcept
	{
		Copy(other);
	}

	Matrix& operator=(const Matrix&& other) noexcept
	{
		Reset();
		Copy(other);
		return *this;
	}

	virtual ~Matrix()
	{
		Reset();
	}

	Value* operator[](std::size_t idx) const
	{
		return _values[idx];
	}

	bool operator==(const Matrix& other) const
	{
		for (std::size_t i = 0; i < _size; ++i)
		{
			for (std::size_t j = 0; j < _size; ++j)
			{
				if (_values[i][j] != other[i][j])
				{
					return false;
				}
			}
		}

		return true;
	}

	Matrix& operator*=(const Matrix& other)
	{
		if (false == _useStrassen)
		{
			Matrix temp(*this);
			for (std::size_t i = 0; i < _size; ++i)
			{
				for (std::size_t j = 0; j < _size; ++j)
				{
					int sum = 0;
					for (std::size_t k = 0; k < _size; ++k)
					{
						sum += (temp[i][k] * other[k][j]);
					}
					(*this)[i][j] = sum;

					Progress::Proceed(_name, 1);
				}
			}
		}
		else
		{
			Strassen(other, 0, 0, 0, 0);
		}
		return *this;
	}

	Matrix operator*(const Matrix& other) const
	{
		Matrix res(*this);
		res *= other;
		return res;
	}

public:
	void SetUseStrassen(bool value)
	{
		_useStrassen = value;
	}

	void SetName(std::string name)
	{
		_name = name;
	}

	void Print(std::size_t width) const
	{
		std::cout << "-------------------------------------------------------------------------" << std::endl;
		for (std::size_t i = 0; i < _n; ++i)
		{
			for (std::size_t j = 0; j < _n; ++j)
			{
				std::cout << std::setw(width) << _values[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "-------------------------------------------------------------------------" << std::endl;
		std::cout << std::endl;
	}

	// for strassen
public:
	typedef int (*Calculate)(int, int);
	static int Add(int value1, int value2)
	{
		return value1 + value2;
	}
	static int Sub(int value1, int value2)
	{
		return value1 - value2;
	}
	static int Set(int value1, int value2)
	{
		return value2;
	}

	Matrix(const Matrix& other, std::size_t row1, std::size_t col1, std::size_t row2, std::size_t col2, std::size_t size, Calculate cal)
	{
		_n = _size = size;
		_useStrassen = other._useStrassen; // it must be true
		_name = other._name;

		_values = new Value * [_size];
		for (std::size_t i = 0; i < _size; ++i)
		{
			_values[i] = new Value[_size];
			for (std::size_t j = 0; j < _size; ++j)
			{
				_values[i][j] = cal(other[row1 + i][col1 + j], other[row2 + i][col2 + j]);
			}
		}
	}

	Matrix(const Matrix& other, std::size_t row, std::size_t col, std::size_t size)
	{
		_n = _size = size;
		_useStrassen = other._useStrassen;
		_name = other._name;

		_values = new Value * [_size];
		for (std::size_t i = 0; i < _size; ++i)
		{
			_values[i] = new Value[_size];
			memcpy(_values[i], other[row + i] + col, _size);
		}
	}

	void Calc(const Matrix& other, std::size_t row, std::size_t rowCount, std::size_t col, std::size_t colCount, Calculate cal)
	{
		for (std::size_t i = 0; i < rowCount; ++i)
		{
			for (std::size_t j = 0; j < colCount; ++j)
			{
				_values[row + i][col + j] = cal(_values[row + i][col + j], other[i][j]);
			}
		}
	}

	void Strassen(const Matrix& other, std::size_t thisRow, std::size_t thisCol, std::size_t otherRow, std::size_t otherCol)
	{
		// reference : https://ko.wikipedia.org/wiki/%EC%8A%88%ED%8A%B8%EB%9D%BC%EC%84%BC_%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98

		if (1 < _size)
		{
			std::size_t half = _size / 2;

			Matrix M1(*this, thisRow, thisCol, thisRow + half, thisCol + half, half, Add); // M1 = A11 + A22
			M1.Strassen(Matrix(other, otherRow, otherCol, otherRow + half, otherCol + half, half, Add), 0, 0, 0, 0); // M1 *= (B11 + B22)

			Matrix M2(*this, thisRow + half, thisCol, thisRow + half, thisCol + half, half, Add); // M2 = A21 + A22
			M2.Strassen(other, 0, 0, otherRow, otherCol); // M2 *= B11

			Matrix M3(*this, thisRow, thisCol, half); // M3 = A11
			M3.Strassen(Matrix(other, otherRow, otherCol + half, otherRow + half, otherCol + half, half, Sub), 0, 0, 0, 0); // M3 *= (B12 - B22)

			Matrix M4(*this, thisRow + half, thisCol + half, half); // M4 = A22
			M4.Strassen(Matrix(other, otherRow + half, otherCol, otherRow, otherCol, half, Sub), 0, 0, 0, 0); // M4 *= (B21 - B11)

			Matrix M5(*this, thisRow, thisCol, thisRow, thisCol + half, half, Add); // M5 = A11 + A12
			M5.Strassen(other, 0, 0, otherRow + half, otherCol + half); // M5 *= B22

			Matrix M6(*this, thisRow + half, thisCol, thisRow, thisCol, half, Sub); // M6 = A21 - A11
			M6.Strassen(Matrix(other, otherRow, otherCol, otherRow, otherCol + half, half, Add), 0, 0, 0, 0); // M6 *= (B11 + B12)

			Matrix M7(*this, thisRow, thisCol + half, thisRow + half, thisCol + half, half, Sub); // M7 = A12 - A22
			M7.Strassen(Matrix(other, otherRow + half, otherCol, otherRow + half, otherCol + half, half, Add), 0, 0, 0, 0); // M7 *= (B21 + B22)

			// for C11
			//Calc(M1, 0, half, 0, half, Set); // C11 = M1
			for (std::size_t i = 0; i < half; ++i)
			{
				memcpy(_values[i], M1[i], half);
			}
			Calc(M4, 0, half, 0, half, Add); // C11 += M4
			Calc(M5, 0, half, 0, half, Sub); // C11 -= M5
			Calc(M7, 0, half, 0, half, Add); // C11 += M7

			// for C12
			//Calc(M3, 0, half, half, half, Set); // C12 = M3
			for (std::size_t i = 0; i < half; ++i)
			{
				memcpy(_values[i] + half, M3[i], half);
			}
			Calc(M5, 0, half, half, half, Add); // C12 += M5

			// for C21
			//Calc(M2, half, half, 0, half, Set); // C21 = M2
			for (std::size_t i = 0; i < half; ++i)
			{
				memcpy(_values[half + i], M2[i], half);
			}
			Calc(M4, half, half, 0, half, Add); // C21 += M4

			// for C22
			//Calc(M1, half, half, half, half, Set); // C22 = M1
			for (std::size_t i = 0; i < half; ++i)
			{
				memcpy(_values[half + i] + half, M1[i], half);
			}
			Calc(M2, half, half, half, half, Sub); // C22 -= M2
			Calc(M3, half, half, half, half, Add); // C22 += M3
			Calc(M6, half, half, half, half, Add); // C22 += m6
		}
		else
		{
			_values[thisRow][thisCol] *= other[otherRow][otherCol];
			Progress::Proceed(_name, 1);
		}
	}

	void Strassen(const Matrix&& other, std::size_t row1, std::size_t col1, std::size_t row2, std::size_t col2)
	{
		Strassen(other, row1, col1, row2, col2);
	}
};

int** MultiplyMatrix(int** A, int** B, std::size_t n)
{
	int** C = new int* [n];

	int sum = 0;
	for (std::size_t i = 0; i < n; ++i)
	{
		C[i] = new int[n];

		for (std::size_t j = 0; j < n; ++j)
		{
			sum = 0;
			for (std::size_t k = 0; k < n; ++k)
			{
				sum += (A[i][k] * B[k][j]);
			}
			C[i][j] = sum;
		}
	}

	return C;
}

void PrintMatrix(int** matrix, std::size_t n)
{
	std::cout << "------------------------------------";
	for (std::size_t i = 0; i < n; ++i)
	{
		std::cout << std::endl;
		for (std::size_t j = 0; j < n; ++j)
		{
			std::cout << std::setw(5) << matrix[i][j] << " ";
		}
	}
	std::cout << std::endl << "------------------------------------" << std::endl;
}

int main()
{
	std::size_t n = 256;
	Matrix A_1(n, -10, 10);
	Matrix A_2 = A_1;
	Matrix B_1(n, -10, 10);
	Matrix B_2 = B_1;
	Matrix naive(n);
	Matrix strassen(n);

	//std::cout << "** Matrix A" << std::endl;
	//A_1.Print(5);
	//std::cout << "** Matrix B" << std::endl;
	//B_1.Print(5);

	// progress
	std::string naiveName = "naive";
	std::string strassenName = "strassen";
	A_1.SetName(naiveName);
	B_1.SetName(naiveName);
	A_2.SetName(strassenName);
	B_2.SetName(strassenName);

	Progress::Create(naiveName, n * n, n * n / 100);
	std::size_t strassenMax = (std::size_t)std::pow(7, std::log2(n)) + 1;
	Progress::Create(strassenName, strassenMax, strassenMax / 100);
	Progress::Initiate();

	A_2.SetUseStrassen(true);
	B_2.SetUseStrassen(true);

	RunningTimeCounter naiveRTC;

	auto run = [](std::string name, RunningTimeCounter* rt, Matrix* A, Matrix* B, Matrix* C) {
		rt->Start();
		*C = (*A) * (*B);
		rt->End();

		std::chrono::duration<double> sec = rt->GetDuration<std::chrono::duration<double>>();
		std::string message = "Performance count finished. [" + name + "] : ";
		message.append(std::to_string(sec.count()));
		message.append(" sec");
		Progress::Message(message);
	};

	std::string message = "Performance count started. size : ";
	message.append(std::to_string(n));
	Progress::Message(message);

	std::thread naiveThread(run, std::string("naive"), &naiveRTC, &A_1, &B_1, &naive);

	// strassen
	double strassenSec = 0.0; // total duration of strassen
	{
		std::size_t half = n / 2;

		auto runStrassen1 = [](RunningTimeCounter* rt, Matrix* A, std::size_t A_row1, std::size_t A_col1, std::size_t A_row2, std::size_t A_col2, Matrix::Calculate A_cal,
			Matrix* B, std::size_t B_row1, std::size_t B_col1, std::size_t B_row2, std::size_t B_col2, Matrix::Calculate B_cal, std::size_t size, Matrix** M)
		{
			rt->Start();
			*M = new Matrix(*A, A_row1, A_col1, A_row2, A_col2, size, A_cal);
			(*M)->Strassen(Matrix(*B, B_row1, B_col1, B_row2, B_col2, size, B_cal), 0, 0, 0, 0);
			rt->End();
		};

		auto runStrassen2 = [](RunningTimeCounter* rt, Matrix* A, std::size_t A_row1, std::size_t A_col1, std::size_t A_row2, std::size_t A_col2, Matrix::Calculate A_cal,
			Matrix* B, std::size_t B_row, std::size_t B_col, std::size_t size, Matrix** M)
		{
			rt->Start();
			*M = new Matrix(*A, A_row1, A_col1, A_row2, A_col2, size, A_cal);
			(*M)->Strassen(*B, 0, 0, B_row, B_col);
			rt->End();
		};

		auto runStrassen3 = [](RunningTimeCounter* rt, Matrix* A, std::size_t A_row, std::size_t A_col,
			Matrix* B, std::size_t B_row1, std::size_t B_col1, std::size_t B_row2, std::size_t B_col2, Matrix::Calculate B_cal, std::size_t size, Matrix** M)
		{
			rt->Start();
			*M = new Matrix(*A, A_row, A_col, size);
			(*M)->Strassen(Matrix(*B, B_row1, B_col1, B_row2, B_col2, size, B_cal), 0, 0, 0, 0);
			rt->End();
		};

		RunningTimeCounter M7_RT;

		// M1
		Matrix* M1 = nullptr;
		RunningTimeCounter M1_RT;
		std::thread tM1(runStrassen1, &M1_RT, &A_2, 0, 0, half, half, Matrix::Add, &B_2, 0, 0, half, half, Matrix::Add, half, &M1);

		// M2
		Matrix* M2 = nullptr;
		RunningTimeCounter M2_RT;
		std::thread tM2(runStrassen2, &M2_RT, &A_2, half, 0, half, half, Matrix::Add, &B_2, 0, 0, half, &M2);

		// M3
		Matrix* M3 = nullptr;
		RunningTimeCounter M3_RT;
		std::thread tM3(runStrassen3, &M3_RT, &A_2, 0, 0, &B_2, 0, half, half, half, Matrix::Sub, half, &M3);

		// M4
		Matrix* M4 = nullptr;
		RunningTimeCounter M4_RT;
		std::thread tM4(runStrassen3, &M4_RT, &A_2, half, half, &B_2, half, 0, 0, 0, Matrix::Sub, half, &M4);

		// M5
		Matrix* M5 = nullptr;
		RunningTimeCounter M5_RT;
		std::thread tM5(runStrassen2, &M5_RT, &A_2, 0, 0, 0, half, Matrix::Add, &B_2, half, half, half, &M5);

		// M6
		Matrix* M6 = nullptr;
		RunningTimeCounter M6_RT;
		std::thread tM6(runStrassen1, &M6_RT, &A_2, half, 0, 0, 0, Matrix::Sub, &B_2, 0, 0, 0, half, Matrix::Add, half, &M6);

		// M7
		Matrix M7_A(A_2, 0, half, half, half, half, Matrix::Sub); // M7_A = A12 - A22
		Matrix M7_B(B_2, half, 0, half, half, half, Matrix::Add); // M7_B = B21 + B22
		Matrix M7 = M7_A * M7_B;

		strassenSec = M7_RT.GetElapsed<std::chrono::duration<double>>().count();

		tM1.join();
		tM2.join();
		tM3.join();
		tM4.join();
		tM5.join();
		tM6.join();

		M7_RT.Start();

		// C11
		//strassen.Calc(*M1, 0, half, 0, half, Matrix::Set); // C11 = M1
		for (std::size_t i = 0; i < half; ++i)
		{
			memcpy(strassen[i], (*M1)[i], half);
		}
		strassen.Calc(*M4, 0, half, 0, half, Matrix::Add); // C11 += M4
		strassen.Calc(*M5, 0, half, 0, half, Matrix::Sub); // C11 -= M5
		strassen.Calc(M7, 0, half, 0, half, Matrix::Add); // C11 += M7

		// C12
		//strassen.Calc(*M3, 0, half, half, half, Matrix::Set); // C12 = M3
		for (std::size_t i = 0; i < half; ++i)
		{
			memcpy(strassen[i] + half, (*M3)[i], half);
		}
		strassen.Calc(*M5, 0, half, half, half, Matrix::Add); // C12 += M5

		// C21
		//strassen.Calc(*M2, half, half, 0, half, Matrix::Set); // C21 = M2
		for (std::size_t i = 0; i < half; ++i)
		{
			memcpy(strassen[half + i], (*M2)[i], half);
		}
		strassen.Calc(*M4, half, half, 0, half, Matrix::Add); // C21 += M4

		// C22
		//strassen.Calc(*M1, half, half, half, half, Matrix::Set); // C22 = M1
		for (std::size_t i = 0; i < half; ++i)
		{
			memcpy(strassen[half + i] + half, (*M1)[i], half);
		}
		strassen.Calc(*M2, half, half, half, half, Matrix::Sub); // C22 -= M2
		strassen.Calc(*M3, half, half, half, half, Matrix::Add); // C22 += M3
		strassen.Calc(*M6, half, half, half, half, Matrix::Add); // C22 += M6
		
		delete M1;
		delete M2;
		delete M3;
		delete M4;
		delete M5;
		delete M6;

		strassenSec += M7_RT.GetElapsed<std::chrono::duration<double>>().count();
		strassenSec += M1_RT.GetDuration<std::chrono::duration<double>>().count();
		strassenSec += M2_RT.GetDuration<std::chrono::duration<double>>().count();
		strassenSec += M3_RT.GetDuration<std::chrono::duration<double>>().count();
		strassenSec += M4_RT.GetDuration<std::chrono::duration<double>>().count();
		strassenSec += M5_RT.GetDuration<std::chrono::duration<double>>().count();
		strassenSec += M6_RT.GetDuration<std::chrono::duration<double>>().count();

		message = "Performance count finished. [strassen] : ";
		message.append(std::to_string(strassenSec));
		message.append(" sec");
		Progress::Message(message);
	}

	//strassen = A_2 * B_2;

	naiveThread.join();

	Progress::Message("All performance count finished.");
	std::chrono::duration<double> naiveSec = naiveRTC.GetDuration<std::chrono::duration<double>>();
	message = "Naive duration time : ";
	message.append(std::to_string(naiveSec.count()));
	message.append(" sec");
	Progress::Message(message);
	message = "Strassen duration time : ";
	message.append(std::to_string(strassenSec));
	message.append(" sec");
	Progress::Message(message);

	if (naive == strassen)
	{
		Progress::Message("Result : same (succeeded)");
		//std::cout << "** Matrix strassen" << std::endl;
		//strassen.Print(5);
	}
	else
	{
		Progress::Message("Result : different (failed)");
		//std::cout << "** Matrix naive" << std::endl;
		//naive.Print(5);
		//std::cout << "** Matrix strassen" << std::endl;
		//strassen.Print(5);
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	Progress::Finalize();

	/*
	std::size_t n = 4;
	int** A = new int* [n];
	int** B = new int* [n];

	for (std::size_t i = 0; i < n; ++i)
	{
		A[i] = new int[n];
		B[i] = new int[n];

		for (std::size_t j = 0; j < n; ++j)
		{
			A[i][j] = MakeRandomNum(-10, 10);
			B[i][j] = MakeRandomNum(-10, 10);
		}
	}

	PrintMatrix(A, n);
	PrintMatrix(B, n);

	int** C = MultiplyMatrix(A, B, n);

	PrintMatrix(C, n);

	for (std::size_t i = 0; i < n; ++i)
	{
		delete[] A[i];
		delete[] B[i];
		delete[] C[i];
	}
	delete[] A;
	delete[] B;
	delete[] C;
	*/

	return 0;
}
