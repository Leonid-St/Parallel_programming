#include "MainWindow.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array < String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(0);
    Parallelprogramming::MainWindow form;
    Application::Run(% form);
} 