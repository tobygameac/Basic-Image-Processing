#pragma once

#include <cstdlib>
#include <ctime>
#include <memory>

#include "image_processing.h"

namespace BasicImageProcessing {

  const size_t PICTURE_BOX_LOCATION_GAP = 50;

  ImageProcessing::ImageProcesser image_processer;

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

      open_file_tool_strip_menu_item_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      save_file_tool_strip_menu_item_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);

      button_r_extraction_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_g_extraction_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_b_extraction_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_rgb_to_gray_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_histogram_equalization_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_threshold_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_mean_filter_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_median_filter_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_vertical_sobel_edge_detection_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_horizontal_sobel_edge_detection_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_sobel_edge_detection_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_overlap_sobel_edge_detection_result_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_connect_component_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_random_color_mapping_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);
      button_reserve_result_->Click += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::OnButtonsClick);

      track_bar_threshold_->ValueChanged += gcnew System::EventHandler(this, &BasicImageProcessing::ImageProccessingForm::TrackBarThresholdValueChanged);

      track_bar_threshold_->Value = 127;
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

    void OnButtonsClick(System::Object ^sender, System::EventArgs ^e) {
      if (sender == open_file_tool_strip_menu_item_) {
        OpenFileDialog ^open_image_file_dialog = gcnew OpenFileDialog();
        open_image_file_dialog->Filter = "Image files | *.*";
        open_image_file_dialog->Title = "Open an image file.";

        if (open_image_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
          Bitmap ^source_bitmap = gcnew Bitmap(open_image_file_dialog->FileName);

          image_processer.SetPixelValuesFromBitmap(source_bitmap);

          picture_box_source_->Image = image_processer.GetSourceBitmapFromPixelValues();

          picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();

          picture_box_result_->Location = System::Drawing::Point(picture_box_source_->Location.X + source_bitmap->Width + PICTURE_BOX_LOCATION_GAP, picture_box_source_->Location.Y);

          label_original_image_->Location = System::Drawing::Point(picture_box_source_->Location.X + source_bitmap->Width * 0.5, label_original_image_->Location.Y);

          label_result_image_->Location = System::Drawing::Point(picture_box_result_->Location.X + source_bitmap->Width * 0.5, label_original_image_->Location.Y);
        }

      } else if (sender == save_file_tool_strip_menu_item_) {
        SaveFileDialog ^save_bmp_file_dialog = gcnew SaveFileDialog();
        save_bmp_file_dialog->Filter = "BMP image files | *.bmp";
        save_bmp_file_dialog->Title = "Save a BMP image file.";

        if (save_bmp_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
          System::IO::Stream^ bmp_file_stream;
          if ((bmp_file_stream = save_bmp_file_dialog->OpenFile()) != nullptr && save_bmp_file_dialog->FileName != "") {
            bmp_file_stream->Close();
            picture_box_result_->Image->Save(save_bmp_file_dialog->FileName, System::Drawing::Imaging::ImageFormat::Bmp);
          }
        }
      } else if (sender == button_r_extraction_) {
        image_processer.ExtractImage(0);
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_g_extraction_) {
        image_processer.ExtractImage(1);
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_b_extraction_) {
        image_processer.ExtractImage(2);
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_rgb_to_gray_) {
        image_processer.RGBToGray();
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_histogram_equalization_) {
        image_processer.HistogramEqualization();
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_threshold_) {
        image_processer.Thresholding(track_bar_threshold_->Value);
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_mean_filter_) {
        image_processer.MeanFilter();
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_median_filter_) {
        image_processer.MedianFilter();
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_vertical_sobel_edge_detection_) {
        image_processer.SobelEdgeDetection(0);
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_horizontal_sobel_edge_detection_) {
        image_processer.SobelEdgeDetection(1);
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_sobel_edge_detection_) {
        image_processer.SobelEdgeDetection(2);
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_overlap_sobel_edge_detection_result_) {
        image_processer.OverlapSobelEdgeDetectionResult(track_bar_threshold_->Value);
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_connect_component_) {
        image_processer.ConnectComponent();
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_random_color_mapping_) {
        image_processer.RandomColorMapping();
        picture_box_result_->Image = image_processer.GetResultBitmapFromPixelValues();
      } else if (sender == button_reserve_result_) {
        image_processer.ReserveResult();
        picture_box_source_->Image = image_processer.GetSourceBitmapFromPixelValues();
      }
    }

    void TrackBarThresholdValueChanged(System::Object ^sender, System::EventArgs ^e) {
      label_threshold_value_->Text = "Threshold : " + track_bar_threshold_->Value;
    }    

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
    System::ComponentModel::IContainer ^components;

    System::Windows::Forms::PictureBox ^picture_box_source_;
    System::Windows::Forms::PictureBox ^picture_box_result_;

    System::Windows::Forms::MenuStrip ^menu_strip;
    System::Windows::Forms::ToolStripMenuItem ^file_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^open_file_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^save_file_tool_strip_menu_item_;

    System::Windows::Forms::Label ^label_original_image_;
    System::Windows::Forms::Label ^label_result_image_;

    System::Windows::Forms::Button ^button_r_extraction_;
    System::Windows::Forms::Button ^button_g_extraction_;
    System::Windows::Forms::Button ^button_b_extraction_;
    System::Windows::Forms::Button ^button_rgb_to_gray_;
    System::Windows::Forms::Button ^button_histogram_equalization_;
    System::Windows::Forms::Button ^button_threshold_;
    System::Windows::Forms::Button ^button_mean_filter_;
    System::Windows::Forms::Button ^button_median_filter_;
    System::Windows::Forms::Button ^button_vertical_sobel_edge_detection_;
    System::Windows::Forms::Button ^button_horizontal_sobel_edge_detection_;
    System::Windows::Forms::Button ^button_sobel_edge_detection_;
    System::Windows::Forms::Button ^button_overlap_sobel_edge_detection_result_;
    System::Windows::Forms::Button ^button_connect_component_;
    System::Windows::Forms::Button ^button_reserve_result_;
    System::Windows::Forms::TrackBar ^track_bar_threshold_;
    System::Windows::Forms::Label ^label_threshold_value_;
    System::Windows::Forms::Button ^button_random_color_mapping_;

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent() {
      this->picture_box_source_ = (gcnew System::Windows::Forms::PictureBox());
      this->picture_box_result_ = (gcnew System::Windows::Forms::PictureBox());
      this->menu_strip = (gcnew System::Windows::Forms::MenuStrip());
      this->file_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->open_file_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->save_file_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->label_original_image_ = (gcnew System::Windows::Forms::Label());
      this->label_result_image_ = (gcnew System::Windows::Forms::Label());
      this->button_r_extraction_ = (gcnew System::Windows::Forms::Button());
      this->button_g_extraction_ = (gcnew System::Windows::Forms::Button());
      this->button_b_extraction_ = (gcnew System::Windows::Forms::Button());
      this->button_rgb_to_gray_ = (gcnew System::Windows::Forms::Button());
      this->button_histogram_equalization_ = (gcnew System::Windows::Forms::Button());
      this->button_threshold_ = (gcnew System::Windows::Forms::Button());
      this->button_mean_filter_ = (gcnew System::Windows::Forms::Button());
      this->button_median_filter_ = (gcnew System::Windows::Forms::Button());
      this->button_vertical_sobel_edge_detection_ = (gcnew System::Windows::Forms::Button());
      this->button_horizontal_sobel_edge_detection_ = (gcnew System::Windows::Forms::Button());
      this->button_sobel_edge_detection_ = (gcnew System::Windows::Forms::Button());
      this->button_overlap_sobel_edge_detection_result_ = (gcnew System::Windows::Forms::Button());
      this->button_connect_component_ = (gcnew System::Windows::Forms::Button());
      this->track_bar_threshold_ = (gcnew System::Windows::Forms::TrackBar());
      this->label_threshold_value_ = (gcnew System::Windows::Forms::Label());
      this->button_reserve_result_ = (gcnew System::Windows::Forms::Button());
      this->button_random_color_mapping_ = (gcnew System::Windows::Forms::Button());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture_box_source_))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture_box_result_))->BeginInit();
      this->menu_strip->SuspendLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->track_bar_threshold_))->BeginInit();
      this->SuspendLayout();
      // 
      // picture_box_source_
      // 
      this->picture_box_source_->Location = System::Drawing::Point(150, 100);
      this->picture_box_source_->Name = L"picture_box_source_";
      this->picture_box_source_->Size = System::Drawing::Size(600, 600);
      this->picture_box_source_->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
      this->picture_box_source_->TabIndex = 0;
      this->picture_box_source_->TabStop = false;
      // 
      // picture_box_result_
      // 
      this->picture_box_result_->Location = System::Drawing::Point(800, 100);
      this->picture_box_result_->Name = L"picture_box_result_";
      this->picture_box_result_->Size = System::Drawing::Size(600, 600);
      this->picture_box_result_->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
      this->picture_box_result_->TabIndex = 1;
      this->picture_box_result_->TabStop = false;
      // 
      // menu_strip
      // 
      this->menu_strip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {
        this->file_tool_strip_menu_item_
      });
      this->menu_strip->Location = System::Drawing::Point(0, 0);
      this->menu_strip->Name = L"menu_strip";
      this->menu_strip->Size = System::Drawing::Size(1400, 24);
      this->menu_strip->TabIndex = 2;
      this->menu_strip->Text = L"Menu strip";
      // 
      // file_tool_strip_menu_item_
      // 
      this->file_tool_strip_menu_item_->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
        this->open_file_tool_strip_menu_item_,
          this->save_file_tool_strip_menu_item_
      });
      this->file_tool_strip_menu_item_->Name = L"file_tool_strip_menu_item_";
      this->file_tool_strip_menu_item_->Size = System::Drawing::Size(37, 20);
      this->file_tool_strip_menu_item_->Text = L"File";
      // 
      // open_file_tool_strip_menu_item_
      // 
      this->open_file_tool_strip_menu_item_->Name = L"open_file_tool_strip_menu_item_";
      this->open_file_tool_strip_menu_item_->Size = System::Drawing::Size(124, 22);
      this->open_file_tool_strip_menu_item_->Text = L"Open File";
      // 
      // save_file_tool_strip_menu_item_
      // 
      this->save_file_tool_strip_menu_item_->Name = L"save_file_tool_strip_menu_item_";
      this->save_file_tool_strip_menu_item_->Size = System::Drawing::Size(124, 22);
      this->save_file_tool_strip_menu_item_->Text = L"Save File";
      // 
      // label_original_image_
      // 
      this->label_original_image_->AutoSize = true;
      this->label_original_image_->Location = System::Drawing::Point(400, 75);
      this->label_original_image_->Name = L"label_original_image_";
      this->label_original_image_->Size = System::Drawing::Size(74, 13);
      this->label_original_image_->TabIndex = 3;
      this->label_original_image_->Text = L"Original Image";
      // 
      // label_result_image_
      // 
      this->label_result_image_->AutoSize = true;
      this->label_result_image_->Location = System::Drawing::Point(1000, 75);
      this->label_result_image_->Name = L"label_result_image_";
      this->label_result_image_->Size = System::Drawing::Size(69, 13);
      this->label_result_image_->TabIndex = 4;
      this->label_result_image_->Text = L"Result Image";
      // 
      // button_r_extraction_
      // 
      this->button_r_extraction_->Location = System::Drawing::Point(10, 100);
      this->button_r_extraction_->Name = L"button_r_extraction_";
      this->button_r_extraction_->Size = System::Drawing::Size(125, 25);
      this->button_r_extraction_->TabIndex = 5;
      this->button_r_extraction_->Text = L"R extraction";
      this->button_r_extraction_->UseVisualStyleBackColor = true;
      // 
      // button_g_extraction_
      // 
      this->button_g_extraction_->Location = System::Drawing::Point(10, 130);
      this->button_g_extraction_->Name = L"button_g_extraction_";
      this->button_g_extraction_->Size = System::Drawing::Size(125, 25);
      this->button_g_extraction_->TabIndex = 6;
      this->button_g_extraction_->Text = L"G extraction";
      this->button_g_extraction_->UseVisualStyleBackColor = true;
      // 
      // button_b_extraction_
      // 
      this->button_b_extraction_->Location = System::Drawing::Point(10, 160);
      this->button_b_extraction_->Name = L"button_b_extraction_";
      this->button_b_extraction_->Size = System::Drawing::Size(125, 25);
      this->button_b_extraction_->TabIndex = 7;
      this->button_b_extraction_->Text = L"B extraction";
      this->button_b_extraction_->UseVisualStyleBackColor = true;
      // 
      // button_rgb_to_gray_
      // 
      this->button_rgb_to_gray_->Location = System::Drawing::Point(10, 190);
      this->button_rgb_to_gray_->Name = L"button_rgb_to_gray_";
      this->button_rgb_to_gray_->Size = System::Drawing::Size(125, 25);
      this->button_rgb_to_gray_->TabIndex = 8;
      this->button_rgb_to_gray_->Text = L"RGB to gray";
      this->button_rgb_to_gray_->UseVisualStyleBackColor = true;
      // 
      // button_histogram_equalization_
      // 
      this->button_histogram_equalization_->Location = System::Drawing::Point(10, 220);
      this->button_histogram_equalization_->Name = L"button_histogram_equalization_";
      this->button_histogram_equalization_->Size = System::Drawing::Size(125, 25);
      this->button_histogram_equalization_->TabIndex = 9;
      this->button_histogram_equalization_->Text = L"Histogram equalization";
      this->button_histogram_equalization_->UseVisualStyleBackColor = true;
      // 
      // button_threshold_
      // 
      this->button_threshold_->Location = System::Drawing::Point(10, 250);
      this->button_threshold_->Name = L"button_threshold_";
      this->button_threshold_->Size = System::Drawing::Size(125, 25);
      this->button_threshold_->TabIndex = 10;
      this->button_threshold_->Text = L"Threshold";
      this->button_threshold_->UseVisualStyleBackColor = true;
      // 
      // button_mean_filter_
      // 
      this->button_mean_filter_->Location = System::Drawing::Point(10, 280);
      this->button_mean_filter_->Name = L"button_mean_filter_";
      this->button_mean_filter_->Size = System::Drawing::Size(125, 25);
      this->button_mean_filter_->TabIndex = 11;
      this->button_mean_filter_->Text = L"Mean filter";
      this->button_mean_filter_->UseVisualStyleBackColor = true;
      // 
      // button_median_filter_
      // 
      this->button_median_filter_->Location = System::Drawing::Point(10, 310);
      this->button_median_filter_->Name = L"button_median_filter_";
      this->button_median_filter_->Size = System::Drawing::Size(125, 25);
      this->button_median_filter_->TabIndex = 12;
      this->button_median_filter_->Text = L"Median filter";
      this->button_median_filter_->UseVisualStyleBackColor = true;
      // 
      // button_vertical_sobel_edge_detection_
      // 
      this->button_vertical_sobel_edge_detection_->Location = System::Drawing::Point(10, 340);
      this->button_vertical_sobel_edge_detection_->Name = L"button_vertical_sobel_edge_detection_";
      this->button_vertical_sobel_edge_detection_->Size = System::Drawing::Size(125, 25);
      this->button_vertical_sobel_edge_detection_->TabIndex = 13;
      this->button_vertical_sobel_edge_detection_->Text = L"Vertical Sobel edge detection";
      this->button_vertical_sobel_edge_detection_->UseVisualStyleBackColor = true;
      // 
      // button_horizontal_sobel_edge_detection_
      // 
      this->button_horizontal_sobel_edge_detection_->Location = System::Drawing::Point(10, 370);
      this->button_horizontal_sobel_edge_detection_->Name = L"button_horizontal_sobel_edge_detection_";
      this->button_horizontal_sobel_edge_detection_->Size = System::Drawing::Size(125, 25);
      this->button_horizontal_sobel_edge_detection_->TabIndex = 14;
      this->button_horizontal_sobel_edge_detection_->Text = L"Horizontal Sobel edge detection";
      this->button_horizontal_sobel_edge_detection_->UseVisualStyleBackColor = true;
      // 
      // button_sobel_edge_detection_
      // 
      this->button_sobel_edge_detection_->Location = System::Drawing::Point(10, 400);
      this->button_sobel_edge_detection_->Name = L"button_sobel_edge_detection_";
      this->button_sobel_edge_detection_->Size = System::Drawing::Size(125, 25);
      this->button_sobel_edge_detection_->TabIndex = 15;
      this->button_sobel_edge_detection_->Text = L"Sobel edge detection";
      this->button_sobel_edge_detection_->UseVisualStyleBackColor = true;
      // 
      // button_overlap_sobel_edge_detection_result_
      // 
      this->button_overlap_sobel_edge_detection_result_->Location = System::Drawing::Point(10, 430);
      this->button_overlap_sobel_edge_detection_result_->Name = L"button_overlap_sobel_edge_detection_result_";
      this->button_overlap_sobel_edge_detection_result_->Size = System::Drawing::Size(125, 25);
      this->button_overlap_sobel_edge_detection_result_->TabIndex = 16;
      this->button_overlap_sobel_edge_detection_result_->Text = L"Overlap Sobel edge detection result";
      this->button_overlap_sobel_edge_detection_result_->UseVisualStyleBackColor = true;
      // 
      // button_connect_component_
      // 
      this->button_connect_component_->Location = System::Drawing::Point(10, 460);
      this->button_connect_component_->Name = L"button_connect_component_";
      this->button_connect_component_->Size = System::Drawing::Size(125, 25);
      this->button_connect_component_->TabIndex = 17;
      this->button_connect_component_->Text = L"Connect components";
      this->button_connect_component_->UseVisualStyleBackColor = true;
      // 
      // track_bar_threshold_
      // 
      this->track_bar_threshold_->Location = System::Drawing::Point(100, 50);
      this->track_bar_threshold_->Maximum = 255;
      this->track_bar_threshold_->Name = L"track_bar_threshold_";
      this->track_bar_threshold_->Size = System::Drawing::Size(100, 45);
      this->track_bar_threshold_->TabIndex = 18;
      this->track_bar_threshold_->TickStyle = System::Windows::Forms::TickStyle::None;
      // 
      // label_threshold_value_
      // 
      this->label_threshold_value_->AutoSize = true;
      this->label_threshold_value_->Location = System::Drawing::Point(10, 50);
      this->label_threshold_value_->Name = L"label_threshold_value_";
      this->label_threshold_value_->Size = System::Drawing::Size(69, 13);
      this->label_threshold_value_->TabIndex = 19;
      this->label_threshold_value_->Text = L"Threshold : 0";
      // 
      // button_reserve_result_
      // 
      this->button_reserve_result_->Location = System::Drawing::Point(10, 530);
      this->button_reserve_result_->Name = L"button_reserve_result_";
      this->button_reserve_result_->Size = System::Drawing::Size(125, 25);
      this->button_reserve_result_->TabIndex = 20;
      this->button_reserve_result_->Text = L"Reserve result";
      this->button_reserve_result_->UseVisualStyleBackColor = true;
      // 
      // button_random_color_mapping_
      // 
      this->button_random_color_mapping_->Location = System::Drawing::Point(10, 491);
      this->button_random_color_mapping_->Name = L"button_random_color_mapping_";
      this->button_random_color_mapping_->Size = System::Drawing::Size(125, 25);
      this->button_random_color_mapping_->TabIndex = 21;
      this->button_random_color_mapping_->Text = L"Random color mapping";
      this->button_random_color_mapping_->UseVisualStyleBackColor = true;
      // 
      // ImageProccessingForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->AutoScroll = true;
      this->ClientSize = System::Drawing::Size(1384, 562);
      this->Controls->Add(this->button_random_color_mapping_);
      this->Controls->Add(this->button_reserve_result_);
      this->Controls->Add(this->label_threshold_value_);
      this->Controls->Add(this->track_bar_threshold_);
      this->Controls->Add(this->button_connect_component_);
      this->Controls->Add(this->button_overlap_sobel_edge_detection_result_);
      this->Controls->Add(this->button_sobel_edge_detection_);
      this->Controls->Add(this->button_horizontal_sobel_edge_detection_);
      this->Controls->Add(this->button_vertical_sobel_edge_detection_);
      this->Controls->Add(this->button_median_filter_);
      this->Controls->Add(this->button_mean_filter_);
      this->Controls->Add(this->button_threshold_);
      this->Controls->Add(this->button_histogram_equalization_);
      this->Controls->Add(this->button_rgb_to_gray_);
      this->Controls->Add(this->button_b_extraction_);
      this->Controls->Add(this->button_g_extraction_);
      this->Controls->Add(this->button_r_extraction_);
      this->Controls->Add(this->label_result_image_);
      this->Controls->Add(this->label_original_image_);
      this->Controls->Add(this->picture_box_result_);
      this->Controls->Add(this->picture_box_source_);
      this->Controls->Add(this->menu_strip);
      this->MainMenuStrip = this->menu_strip;
      this->Name = L"ImageProccessingForm";
      this->Text = L"Image Proccessing Form";
      this->Load += gcnew System::EventHandler(this, &ImageProccessingForm::form_Load);
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture_box_source_))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picture_box_result_))->EndInit();
      this->menu_strip->ResumeLayout(false);
      this->menu_strip->PerformLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->track_bar_threshold_))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }

#pragma endregion
  private: System::Void form_Load(System::Object ^sender, System::EventArgs ^e) {
  }
  };
}
