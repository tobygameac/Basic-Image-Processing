#pragma once

#include <memory>

#include "image_processing.h"

namespace BasicImageProcessing {

  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;

  /// <summary>
  /// Summary for form
  /// </summary>
  public ref class ImageProccessingForm : public System::Windows::Forms::Form {

  public:

    ImageProccessingForm() {
      InitializeComponent();
      //
      //TODO: Add the constructor code here
      //

      this->open_file_tool_strip_menu_item->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OpenFileButtonClick);

      this->button_r_extraction->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonRExtractionClick);
      this->button_g_extraction->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonGExtractionClick);
      this->button_b_extraction->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonBExtractionClick);
      this->button_rgb_to_gray->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonRGBToGrayClick);
      this->button_histogram_equalization->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonHistogramEqualizationClick);
      this->button_threshold->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonThresholdClick);
      this->button_mean_filter->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonMeanFilterClick);
      this->button_median_filter->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonMedianFilterClick);
      this->button_vertical_sobel_edge_detection->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonVerticalSobelEdgeDetectionClick);
      this->button_horizontal_sobel_edge_detection->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonHorizontalSobelEdgeDetectionClick);
      this->button_sobel_edge_detection->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonSobelEdgeDetectionClick);
      this->button_overlap_sobel_edge_detection_result->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonOverlapSobelEdgeDetectionResultClick);
      this->button_connect_component->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::ButtonConnectComponentClick);
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~ImageProccessingForm() {
      if (components) {
        delete components;
      }
    }

  private:

    System::Drawing::Bitmap ^source_image;

  private: System::Windows::Forms::PictureBox ^picture_box_source;
  private: System::Windows::Forms::PictureBox ^picture_box_result;
  private: System::Windows::Forms::MenuStrip ^menu_strip;
  private: System::Windows::Forms::ToolStripMenuItem ^file_tool_strip_menu_item;
  private: System::Windows::Forms::ToolStripMenuItem ^open_file_tool_strip_menu_item;
  private: System::Windows::Forms::Label ^label_original_image;
  private: System::Windows::Forms::Label ^label_result_image;
  private: System::Windows::Forms::Button ^button_r_extraction;
  private: System::Windows::Forms::Button ^button_g_extraction;
  private: System::Windows::Forms::Button ^button_b_extraction;
  private: System::Windows::Forms::Button ^button_rgb_to_gray;
  private: System::Windows::Forms::Button ^button_histogram_equalization;
  private: System::Windows::Forms::Button ^button_threshold;
  private: System::Windows::Forms::Button ^button_mean_filter;
  private: System::Windows::Forms::Button ^button_median_filter;
  private: System::Windows::Forms::Button ^button_vertical_sobel_edge_detection;
  private: System::Windows::Forms::Button ^button_horizontal_sobel_edge_detection;
  private: System::Windows::Forms::Button ^button_sobel_edge_detection;
  private: System::Windows::Forms::Button ^button_overlap_sobel_edge_detection_result;
  private: System::Windows::Forms::Button ^button_connect_component;
  private: System::Windows::Forms::TrackBar ^track_bar_threshold;


  private: System::ComponentModel::IContainer ^components;

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>


#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent() {
      this->picture_box_source = (gcnew System::Windows::Forms::PictureBox());
      this->picture_box_result = (gcnew System::Windows::Forms::PictureBox());
      this->menu_strip = (gcnew System::Windows::Forms::MenuStrip());
      this->file_tool_strip_menu_item = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->open_file_tool_strip_menu_item = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->label_original_image = (gcnew System::Windows::Forms::Label());
      this->label_result_image = (gcnew System::Windows::Forms::Label());
      this->button_r_extraction = (gcnew System::Windows::Forms::Button());
      this->button_g_extraction = (gcnew System::Windows::Forms::Button());
      this->button_b_extraction = (gcnew System::Windows::Forms::Button());
      this->button_rgb_to_gray = (gcnew System::Windows::Forms::Button());
      this->button_histogram_equalization = (gcnew System::Windows::Forms::Button());
      this->button_threshold = (gcnew System::Windows::Forms::Button());
      this->button_mean_filter = (gcnew System::Windows::Forms::Button());
      this->button_median_filter = (gcnew System::Windows::Forms::Button());
      this->button_vertical_sobel_edge_detection = (gcnew System::Windows::Forms::Button());
      this->button_horizontal_sobel_edge_detection = (gcnew System::Windows::Forms::Button());
      this->button_sobel_edge_detection = (gcnew System::Windows::Forms::Button());
      this->button_overlap_sobel_edge_detection_result = (gcnew System::Windows::Forms::Button());
      this->button_connect_component = (gcnew System::Windows::Forms::Button());
      this->track_bar_threshold = (gcnew System::Windows::Forms::TrackBar());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture_box_source))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture_box_result))->BeginInit();
      this->menu_strip->SuspendLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->track_bar_threshold))->BeginInit();
      this->SuspendLayout();
      // 
      // picture_box_source
      // 
      this->picture_box_source->Location = System::Drawing::Point(150, 250);
      this->picture_box_source->Name = L"picture_box_source";
      this->picture_box_source->Size = System::Drawing::Size(400, 400);
      this->picture_box_source->TabIndex = 0;
      this->picture_box_source->TabStop = false;
      // 
      // picture_box_result
      // 
      this->picture_box_result->Location = System::Drawing::Point(600, 250);
      this->picture_box_result->Name = L"picture_box_result";
      this->picture_box_result->Size = System::Drawing::Size(400, 400);
      this->picture_box_result->TabIndex = 1;
      this->picture_box_result->TabStop = false;
      // 
      // menu_strip
      // 
      this->menu_strip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^>(1) {
        this->file_tool_strip_menu_item
      });
      this->menu_strip->Location = System::Drawing::Point(0, 0);
      this->menu_strip->Name = L"menu_strip";
      this->menu_strip->Size = System::Drawing::Size(1024, 24);
      this->menu_strip->TabIndex = 2;
      this->menu_strip->Text = L"menuStrip1";
      // 
      // file_tool_strip_menu_item
      // 
      this->file_tool_strip_menu_item->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^>(1) {
        this->open_file_tool_strip_menu_item
      });
      this->file_tool_strip_menu_item->Name = L"file_tool_strip_menu_item";
      this->file_tool_strip_menu_item->Size = System::Drawing::Size(37, 20);
      this->file_tool_strip_menu_item->Text = L"File";
      // 
      // open_file_tool_strip_menu_item
      // 
      this->open_file_tool_strip_menu_item->Name = L"open_file_tool_strip_menu_item";
      this->open_file_tool_strip_menu_item->Size = System::Drawing::Size(124, 22);
      this->open_file_tool_strip_menu_item->Text = L"Open File";
      // 
      // label_original_image
      // 
      this->label_original_image->AutoSize = true;
      this->label_original_image->Location = System::Drawing::Point(150, 200);
      this->label_original_image->Name = L"label_original_image";
      this->label_original_image->Size = System::Drawing::Size(74, 13);
      this->label_original_image->TabIndex = 3;
      this->label_original_image->Text = L"Original Image";
      // 
      // label_result_image
      // 
      this->label_result_image->AutoSize = true;
      this->label_result_image->Location = System::Drawing::Point(600, 200);
      this->label_result_image->Name = L"label_result_image";
      this->label_result_image->Size = System::Drawing::Size(69, 13);
      this->label_result_image->TabIndex = 4;
      this->label_result_image->Text = L"Result Image";
      // 
      // button_r_extraction
      // 
      this->button_r_extraction->Location = System::Drawing::Point(10, 100);
      this->button_r_extraction->Name = L"button_r_extraction";
      this->button_r_extraction->Size = System::Drawing::Size(75, 25);
      this->button_r_extraction->TabIndex = 5;
      this->button_r_extraction->Text = L"R extraction";
      this->button_r_extraction->UseVisualStyleBackColor = true;
      // 
      // button_g_extraction
      // 
      this->button_g_extraction->Location = System::Drawing::Point(10, 130);
      this->button_g_extraction->Name = L"button_g_extraction";
      this->button_g_extraction->Size = System::Drawing::Size(75, 25);
      this->button_g_extraction->TabIndex = 6;
      this->button_g_extraction->Text = L"G extraction";
      this->button_g_extraction->UseVisualStyleBackColor = true;
      // 
      // button_b_extraction
      // 
      this->button_b_extraction->Location = System::Drawing::Point(10, 160);
      this->button_b_extraction->Name = L"button_b_extraction";
      this->button_b_extraction->Size = System::Drawing::Size(75, 25);
      this->button_b_extraction->TabIndex = 7;
      this->button_b_extraction->Text = L"B extraction";
      this->button_b_extraction->UseVisualStyleBackColor = true;
      // 
      // button_rgb_to_gray
      // 
      this->button_rgb_to_gray->Location = System::Drawing::Point(10, 190);
      this->button_rgb_to_gray->Name = L"button_rgb_to_gray";
      this->button_rgb_to_gray->Size = System::Drawing::Size(75, 25);
      this->button_rgb_to_gray->TabIndex = 8;
      this->button_rgb_to_gray->Text = L"RGB to gray";
      this->button_rgb_to_gray->UseVisualStyleBackColor = true;
      // 
      // button_histogram_equalization
      // 
      this->button_histogram_equalization->Location = System::Drawing::Point(10, 220);
      this->button_histogram_equalization->Name = L"button_histogram_equalization";
      this->button_histogram_equalization->Size = System::Drawing::Size(75, 25);
      this->button_histogram_equalization->TabIndex = 9;
      this->button_histogram_equalization->Text = L"Histogram equalization";
      this->button_histogram_equalization->UseVisualStyleBackColor = true;
      // 
      // button_threshold
      // 
      this->button_threshold->Location = System::Drawing::Point(10, 250);
      this->button_threshold->Name = L"button_threshold";
      this->button_threshold->Size = System::Drawing::Size(75, 25);
      this->button_threshold->TabIndex = 10;
      this->button_threshold->Text = L"Threshold";
      this->button_threshold->UseVisualStyleBackColor = true;
      // 
      // button_mean_filter
      // 
      this->button_mean_filter->Location = System::Drawing::Point(10, 280);
      this->button_mean_filter->Name = L"button_mean_filter";
      this->button_mean_filter->Size = System::Drawing::Size(75, 25);
      this->button_mean_filter->TabIndex = 11;
      this->button_mean_filter->Text = L"Mean filter";
      this->button_mean_filter->UseVisualStyleBackColor = true;
      // 
      // button_median_filter
      // 
      this->button_median_filter->Location = System::Drawing::Point(10, 310);
      this->button_median_filter->Name = L"button_median_filter";
      this->button_median_filter->Size = System::Drawing::Size(75, 25);
      this->button_median_filter->TabIndex = 12;
      this->button_median_filter->Text = L"Median filter";
      this->button_median_filter->UseVisualStyleBackColor = true;
      // 
      // button_vertical_sobel_edge_detection
      // 
      this->button_vertical_sobel_edge_detection->Location = System::Drawing::Point(10, 340);
      this->button_vertical_sobel_edge_detection->Name = L"button_vertical_sobel_edge_detection";
      this->button_vertical_sobel_edge_detection->Size = System::Drawing::Size(75, 25);
      this->button_vertical_sobel_edge_detection->TabIndex = 13;
      this->button_vertical_sobel_edge_detection->Text = L"Vertical Sobel edge detection";
      this->button_vertical_sobel_edge_detection->UseVisualStyleBackColor = true;
      // 
      // button_horizontal_sobel_edge_detection
      // 
      this->button_horizontal_sobel_edge_detection->Location = System::Drawing::Point(10, 370);
      this->button_horizontal_sobel_edge_detection->Name = L"button_horizontal_sobel_edge_detection";
      this->button_horizontal_sobel_edge_detection->Size = System::Drawing::Size(75, 25);
      this->button_horizontal_sobel_edge_detection->TabIndex = 14;
      this->button_horizontal_sobel_edge_detection->Text = L"Horizontal Sobel edge detection";
      this->button_horizontal_sobel_edge_detection->UseVisualStyleBackColor = true;
      // 
      // button_sobel_edge_detection
      // 
      this->button_sobel_edge_detection->Location = System::Drawing::Point(10, 400);
      this->button_sobel_edge_detection->Name = L"button_sobel_edge_detection";
      this->button_sobel_edge_detection->Size = System::Drawing::Size(75, 25);
      this->button_sobel_edge_detection->TabIndex = 15;
      this->button_sobel_edge_detection->Text = L"Sobel edge detection";
      this->button_sobel_edge_detection->UseVisualStyleBackColor = true;
      // 
      // button_overlap_sobel_edge_detection_result
      // 
      this->button_overlap_sobel_edge_detection_result->Location = System::Drawing::Point(10, 430);
      this->button_overlap_sobel_edge_detection_result->Name = L"button_overlap_sobel_edge_detection_result";
      this->button_overlap_sobel_edge_detection_result->Size = System::Drawing::Size(75, 25);
      this->button_overlap_sobel_edge_detection_result->TabIndex = 16;
      this->button_overlap_sobel_edge_detection_result->Text = L"Overlap Sobel edge detection result";
      this->button_overlap_sobel_edge_detection_result->UseVisualStyleBackColor = true;
      // 
      // button_connect_component
      // 
      this->button_connect_component->Location = System::Drawing::Point(10, 460);
      this->button_connect_component->Name = L"button_connect_component";
      this->button_connect_component->Size = System::Drawing::Size(75, 25);
      this->button_connect_component->TabIndex = 17;
      this->button_connect_component->Text = L"Connect Component";
      this->button_connect_component->UseVisualStyleBackColor = true;
      // 
      // track_bar_threshold
      // 
      this->track_bar_threshold->Location = System::Drawing::Point(450, 100);
      this->track_bar_threshold->Maximum = 255;
      this->track_bar_threshold->Name = L"track_bar_threshold";
      this->track_bar_threshold->Size = System::Drawing::Size(100, 45);
      this->track_bar_threshold->TabIndex = 18;
      // 
      // ImageProccessingForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(1024, 768);
      this->Controls->Add(this->track_bar_threshold);
      this->Controls->Add(this->button_connect_component);
      this->Controls->Add(this->button_overlap_sobel_edge_detection_result);
      this->Controls->Add(this->button_sobel_edge_detection);
      this->Controls->Add(this->button_horizontal_sobel_edge_detection);
      this->Controls->Add(this->button_vertical_sobel_edge_detection);
      this->Controls->Add(this->button_median_filter);
      this->Controls->Add(this->button_mean_filter);
      this->Controls->Add(this->button_threshold);
      this->Controls->Add(this->button_histogram_equalization);
      this->Controls->Add(this->button_rgb_to_gray);
      this->Controls->Add(this->button_b_extraction);
      this->Controls->Add(this->button_g_extraction);
      this->Controls->Add(this->button_r_extraction);
      this->Controls->Add(this->label_result_image);
      this->Controls->Add(this->label_original_image);
      this->Controls->Add(this->picture_box_result);
      this->Controls->Add(this->picture_box_source);
      this->Controls->Add(this->menu_strip);
      this->MainMenuStrip = this->menu_strip;
      this->Name = L"ImageProccessingForm";
      this->Text = L"form";
      this->Load += gcnew System::EventHandler(this, &ImageProccessingForm::form_Load);
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture_box_source))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture_box_result))->EndInit();
      this->menu_strip->ResumeLayout(false);
      this->menu_strip->PerformLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->track_bar_threshold))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }

    void OpenFileButtonClick(System::Object ^sender, System::EventArgs ^e) {
      OpenFileDialog ^open_bmp_file_dialog = gcnew OpenFileDialog();
      open_bmp_file_dialog->Filter = "BMP image files | *.bmp";
      open_bmp_file_dialog->Title = "Select a BMP image file.";

      if (open_bmp_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        source_image = gcnew Bitmap(open_bmp_file_dialog->FileName);

        //this->picture_box_source->Size = source_image->Size;
        //this->picture_box_result->Size = source_image->Size;

        this->picture_box_source->Image = source_image;
        this->picture_box_result->Image = source_image;
      }
    }

    void ButtonRExtractionClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::ExtractImage(source_image, 0);
    }

    void ButtonGExtractionClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::ExtractImage(source_image, 1);
    }

    void ButtonBExtractionClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::ExtractImage(source_image, 2);
    }

    void ButtonRGBToGrayClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::RGBToGray(source_image);
    }

    void ButtonHistogramEqualizationClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::HistogramEqualization(source_image);
    }

    void ButtonThresholdClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::Threshold(source_image, track_bar_threshold->Value);
    }

    void ButtonMeanFilterClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::MeanFilter(source_image);
    }

    void ButtonMedianFilterClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::MedianFilter(source_image);
    }

    void ButtonVerticalSobelEdgeDetectionClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::SobelEdgeDetection(source_image, 0);
    }

    void ButtonHorizontalSobelEdgeDetectionClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::SobelEdgeDetection(source_image, 1);
    }

    void ButtonSobelEdgeDetectionClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::SobelEdgeDetection(source_image, 2);
    }

    void ButtonOverlapSobelEdgeDetectionResultClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::OverlapSobelEdgeDetectionResult(source_image, track_bar_threshold->Value);
    }

    void ButtonConnectComponentClick(System::Object ^sender, System::EventArgs ^e) {
      this->picture_box_result->Image = ImageProcessing::ConnectComponent(source_image);
    }

#pragma endregion
  private: System::Void toolStripMenuItem1_Click(System::Object ^sender, System::EventArgs ^e) {
  }
  private: System::Void form_Load(System::Object ^sender, System::EventArgs ^e) {
  }
  };
}
