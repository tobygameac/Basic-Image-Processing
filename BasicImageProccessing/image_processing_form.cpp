#include "image_processing_form.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^> ^args) {
  Application::EnableVisualStyles();
  Application::SetCompatibleTextRenderingDefault(false);
  BasicImageProcessing::ImageProccessingForm image_processing_form;
  Application::Run(%image_processing_form);
  return 0;
}