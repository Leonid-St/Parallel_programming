#pragma once
#include <iostream>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <stdlib.h> 
#include <thread>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath> 
#include <optional>
#include <algorithm>
#include <map>
#include <type_traits>
#include<stdio.h>
#include <random>


#define MAX_THREADS     200
#define totalpoints 100000000
#define amountPoints 150000
#define inaccuracy pow(10,(-9))
#define amountThreads 12

using namespace std;

namespace Parallelprogramming {

	using namespace System;
	using namespace System::Diagnostics;
	using namespace System::ComponentModel;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	struct points {
		long long pointsCircle = 0;
	};
	struct val_to_fun {
		void* point;
		uniform_real_distribution<double> interval;
		default_random_engine Rand;
		val_to_fun(void* point, uniform_real_distribution<double> cur_interval, default_random_engine cur_Rand) {
			this->point = point;
			interval = cur_interval;
			Rand = cur_Rand;
		}
	};
	DWORD WINAPI randomFun(void* point, uniform_real_distribution<double> interval, default_random_engine Rand) {
		for (long i = 0; i < amountPoints; i++) {
			double x = interval(Rand);
			double y = interval(Rand);
			if (x * x + y * y <= 1.0)
				((points*)point)->pointsCircle++;
		}
		return 0;
	}
	/// <summary>
	/// Summary for Laba1
	/// </summary>
	public ref class Laba1 : public System::Windows::Forms::Form

	{
	public:
		
		void Calc_Pi(double Pi, double PrevPi, uniform_real_distribution<double> interval,
			default_random_engine Rand, points mas[amountThreads], HANDLE thread[amountThreads],ofstream *f ) {
			while (true) {
				for (int i = 0; i < amountThreads; i++) {
					val_to_fun *val = new val_to_fun(&mas[i], interval, Rand);
						thread[i] = CreateThread(NULL, 0, &randomFun, val, 0, NULL);
				}
					
				WaitForMultipleObjects(amountThreads, thread, TRUE, INFINITE);
				for (int i = 0; i < amountThreads; i++) {
					Pi = (double)mas[i].pointsCircle * 4 / amountPoints;
					*f << " Pi = " << Pi << " Accuracy = " << abs(Pi - PrevPi) << "\n";
					if (abs(Pi - PrevPi) < inaccuracy)
						return;
					PrevPi = Pi;
					mas[i].pointsCircle = 0;
				}
			}
		}

		Laba1(void)
		{

			InitializeComponent();
			ofstream f;
			f.open("C://state/state.txt", std::ofstream::out | std::ofstream::trunc);
			if (!f)
			{
				cout << "error!";
			}
			f << "1) Последовательный алгоритм вычисление числа PI методом Монте - Карло:\n";
			LARGE_INTEGER time_s, freq, time_n;
			double pi,
				pointsIn = 0,
				xValue,
				yValue,
				distance;
			srand(time(NULL));
			QueryPerformanceCounter(&time_s);
			QueryPerformanceFrequency(&freq);

			for (int i = 0; i < totalpoints; i++)
			{
				xValue = (double)rand() / RAND_MAX; //Random point: x 
				yValue = (double)rand() / RAND_MAX; //Random point: y
				distance = (xValue * xValue) + (yValue * yValue);
				distance = sqrt(distance);
				if (distance <= 1)
					pointsIn++;
			}
			QueryPerformanceCounter(&time_n);
			int SecondsElapsed = (time_s.QuadPart / freq.QuadPart) - (time_n.QuadPart / freq.QuadPart);

			pi = 4 * pointsIn / totalpoints;
			f << "The vaule of pi according to monte carlo method is: " << pi << " with " << -SecondsElapsed << " seconds\n";
			f << "2)При помощи WinAPI (функция CreateThread)\n";
			default_random_engine Rand;
			uniform_real_distribution<double> interval(-1.0, 1.0);
			double Pi = 0, PrevPi = 0;
			HANDLE thread[amountThreads];
			points mas[amountThreads];
			__int64 ST = 0, FT = 0, Frequency = 0;
			if (QueryPerformanceCounter((LARGE_INTEGER*)&ST) != 0) {
				Calc_Pi(Pi, PrevPi,interval,Rand, mas,thread,&f);
				printf("\n\n");
				QueryPerformanceCounter((LARGE_INTEGER*)&FT);
				QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);
				f << " Time: \t" << (FT - ST) * 1.0 / Frequency << "sec\n";
				f << " Number of threads : \n" << amountThreads << ".\n";
			}
			else {
				DWORD dwError = GetLastError();
				f << "Error value =  ld : \t" << dwError << " id\n";
			}
			//getchar();

			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Laba1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart1;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(12, 12);
			this->chart1->Name = L"chart1";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series1->Legend = L"Legend1";
			series1->Name = L"Series1";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(672, 386);
			this->chart1->TabIndex = 0;
			this->chart1->Text = L"chart1";
			// 
			// Laba1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(696, 410);
			this->Controls->Add(this->chart1);
			this->Name = L"Laba1";
			this->Text = L"Laba1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
