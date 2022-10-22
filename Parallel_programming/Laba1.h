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


#define MAX_THREADS  200
#define totalpoints 1000000
#define inaccuracy pow(10,(-9))
#define amountThreads 1

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
		points* point;
		uniform_real_distribution<double> interval;
		default_random_engine Rand;
		val_to_fun(points* point, uniform_real_distribution<double> cur_interval, default_random_engine cur_Rand) {
			this->point = point;
			interval = cur_interval;
			Rand = cur_Rand;
		}
	};
	DWORD WINAPI randomFun(void* value) {
		val_to_fun* cast_value = (val_to_fun*)value;
		for (long i = 0; i < totalpoints; i++) {
			double x = cast_value->interval(cast_value->Rand);
			double y = cast_value->interval(cast_value->Rand);

			if (x * x + y * y <= 1.0) {
				cast_value->point->pointsCircle++;

			}
		}
		//cout <<"cast_value->point->pointsCircle:  \t" << cast_value->point->pointsCircle <<  "\n";  
		return 0;
	}
	void threadFunction(double& PointsIn, int length)
	{
		PointsIn = 0;
		double xValue,
			yValue,
			distance;
		srand(time(NULL));

		for (int i = 0; i < totalpoints / length; i++)
		{
			xValue = (double)rand() / RAND_MAX; //Random point: x
			yValue = (double)rand() / RAND_MAX; //Random point: y
			distance = (xValue * xValue) + (yValue * yValue);
			distance = sqrt(distance);
			if (distance <= 1)
				PointsIn++;
		}
	}
	/// <summary>
	/// Summary for Laba1
	/// </summary>
	public ref class Laba1 : public System::Windows::Forms::Form
	{
	public:
		
		void Calc_Pi(double Pi, double PrevPi, uniform_real_distribution<double> interval,
			ofstream *f, int threadCount) {
			HANDLE* thread = new HANDLE[threadCount];
			points *massive_points = new points[threadCount];
			for (int i = 0; i < threadCount; i++) {
				massive_points[i].pointsCircle = 0;
			}
			/*for (int i = 0; i < threadCount; i++) {
				cout << "massive_points[i].pointsCircle: " << massive_points[i].pointsCircle << "\n";
			}*/ // work
			//while (true) {
				for (int i = 0; i < threadCount; i++) {
					std::default_random_engine generator(time(0)+i);
					val_to_fun *val = new val_to_fun(&massive_points[i], interval, generator);
					thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)randomFun, val, 0, NULL);
				}
				WaitForMultipleObjects(threadCount, thread, TRUE, INFINITE);
				for (int i = 0; i < threadCount; i++) {

					//cout << (double)massive_points[i].pointsCircle << "\n";
					//cout << threadCount << "threadCount" << "\n";
					//cout << i << "i" << "\n";
					//cout << "(double)massive_points[i].pointsCircle " << (double)massive_points[i].pointsCircle << "\n";
					Pi = (double)massive_points[i].pointsCircle * 4 / totalpoints;
					//cout << "PI:  " <<  Pi << "\n";
					*f << " Pi = " << Pi  << "\n";
					/*if (abs(Pi - PrevPi) < inaccuracy)

						return;*/
					PrevPi = Pi;
					//massive_points[i].pointsCircle = 0;
				}
			//}
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
			uniform_real_distribution<double> interval(-1.0, 1.0);
			int threadCount;
			for ( threadCount = 2; threadCount <= 20; threadCount = threadCount +2) {
				double Pi = 0, PrevPi = 0;
			
				__int64 ST = 0, FT = 0, Frequency = 0;

				if (QueryPerformanceCounter((LARGE_INTEGER*)&ST) != 0) {
					Calc_Pi(Pi, PrevPi, interval, &f, threadCount);
					f << "\n\n";
					QueryPerformanceCounter((LARGE_INTEGER*)&FT);
					QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);
					f << " Time: \t" << (FT - ST) * 1.0 / Frequency << "sec\n";
					f << " Number of threads : \t" << threadCount << ".\n";
					this->chart1->Series[0]->Points->AddXY(threadCount , (FT - ST) * 1.0 / Frequency );
					
				}
				else {
					DWORD dwError = GetLastError();
					f << "Error value =  ld : \t" << dwError << " id\n";
				}
			}

			
			//
			
			//LARGE_INTEGER time_s, freq, time_n;
			f << "3) При помощи класса std::thread из стандартной библиотеки C++11. \n";
			pi = 0;
				double lastPointsIn = 0;
			const int length = 12;
			double arPointsIn[length];


			std::thread thread_array[length];

			QueryPerformanceCounter(&time_s);
			QueryPerformanceFrequency(&freq);

			for (int i = 0; i < length; i++) {
				thread_array[i] = std::thread(threadFunction, std::ref(arPointsIn[i]), length);
			}
			for (int i = 0; i < length; i++) {
				thread_array[i].join();
				f << "pointsIn: " << arPointsIn[i] << " . " << std::endl;
				lastPointsIn = lastPointsIn + arPointsIn[i];
			}

			pi = 4 * lastPointsIn / totalpoints;

			QueryPerformanceCounter(&time_n);
			 SecondsElapsed = (time_s.QuadPart / freq.QuadPart) - (time_n.QuadPart / freq.QuadPart);

			f << "The vaule of pi according to monte carlo method is: " << pi << " with " << -SecondsElapsed << " seconds";
			//system("PAUSE");
			//return 0;

			

			

			ProcessStartInfo^ startInfo = gcnew ProcessStartInfo("notepad.exe");
			//startInfo->WindowStyle = ProcessWindowStyle::Minimized;
			//Process::Start(startInfo);
			startInfo->Arguments = "C://state/state.txt";
			Process::Start(startInfo);


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
private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart2;
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
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^ series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->BeginInit();
			this->SuspendLayout();
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(16, 15);
			this->chart1->Margin = System::Windows::Forms::Padding(4);
			this->chart1->Name = L"chart1";
			series1->BorderWidth = 5;
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series1->Legend = L"Legend1";
			series1->Name = L"Series1";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(720, 638);
			this->chart1->TabIndex = 0;
			this->chart1->Text = L"chart1";
			this->chart1->Click += gcnew System::EventHandler(this, &Laba1::chart1_Click);
			// 
			// chart2
			// 
			chartArea2->Name = L"ChartArea1";
			this->chart2->ChartAreas->Add(chartArea2);
			legend2->Name = L"Legend1";
			this->chart2->Legends->Add(legend2);
			this->chart2->Location = System::Drawing::Point(804, 13);
			this->chart2->Margin = System::Windows::Forms::Padding(4);
			this->chart2->Name = L"chart2";
			series2->BorderWidth = 5;
			series2->ChartArea = L"ChartArea1";
			series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Point;
			series2->Legend = L"Legend1";
			series2->Name = L"Series1";
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Point;
			series3->Legend = L"Legend1";
			series3->Name = L"Series2";
			series3->YValuesPerPoint = 2;
			this->chart2->Series->Add(series2);
			this->chart2->Series->Add(series3);
			this->chart2->Size = System::Drawing::Size(820, 714);
			this->chart2->TabIndex = 1;
			this->chart2->Text = L"chart2";
			// 
			// Laba1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1720, 796);
			this->Controls->Add(this->chart2);
			this->Controls->Add(this->chart1);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Laba1";
			this->Text = L"Laba1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void chart1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
};
}
