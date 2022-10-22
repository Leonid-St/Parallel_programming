#include "MainWindow.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(0);
    Parallelprogramming::MainWindow form;
    Application::Run(% form);
} 