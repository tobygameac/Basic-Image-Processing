#include <cmath>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "rgba.h"
#include "omp.h"

namespace BasicImageProcessing {

  namespace ImageProcessing {

    RGBA<unsigned char> rgb_random_color_mapping[256][256][256];
    bool color_generated = false;

    inline bool IsInImage(const std::vector<std::vector<RGBA<unsigned char> > > &pixel_values, int r, int c) {
      return r >= 0 && r < pixel_values.size() && c >= 0 && c < pixel_values[r].size();
    }

    void ExtractImage(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values, const size_t extracted_channel) {
      // extracted_channel 0 : R
      // extracted_channel 1 : G
      // extracted_channel 2 : B
      if (extracted_channel < 0 || extracted_channel > 2) {
        return;
      }

#pragma omp parallel for
      for (int row = 0; row < source_pixel_values.size(); ++row) {
#pragma omp parallel for
        for (int column = 0; column < source_pixel_values[row].size(); ++column) {
          unsigned char extracted_value = source_pixel_values[row][column].r_;
          if (extracted_channel == 1) {
            extracted_value = source_pixel_values[row][column].g_;
          } else if (extracted_channel == 2) {
            extracted_value = source_pixel_values[row][column].b_;
          }
          result_pixel_values[row][column] = RGBA<unsigned char>(extracted_value, extracted_value, extracted_value);
        }
      }
    }

    void RGBToGray(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values) {
#pragma omp parallel for
      for (int row = 0; row < source_pixel_values.size(); ++row) {
#pragma omp parallel for
        for (int column = 0; column < source_pixel_values[row].size(); ++column) {
          RGBA<unsigned char> rgb_value = source_pixel_values[row][column];
          double gray_value = (rgb_value.r_ + rgb_value.g_ + rgb_value.b_) / 3.0;
          result_pixel_values[row][column] = RGBA<unsigned char>(gray_value, gray_value, gray_value);
        }
      }
    }

    void RandomColorMapping(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values) {
      if (!color_generated) {
#pragma omp parallel for
        for (int r = 0; r < 256; ++r) {
#pragma omp parallel for
          for (int g = 0; g < 256; ++g) {
#pragma omp parallel for
            for (int b = 0; b < 256; ++b) {
              rgb_random_color_mapping[r][g][b].r_ = rand() % 256;
              rgb_random_color_mapping[r][g][b].g_ = rand() % 256;
              rgb_random_color_mapping[r][g][b].b_ = rand() % 256;
            }
          }
        }
        color_generated = true;
      }

#pragma omp parallel for
      for (int row = 0; row < source_pixel_values.size(); ++row) {
#pragma omp parallel for
        for (int column = 0; column < source_pixel_values[row].size(); ++column) {
          RGBA<unsigned char> rgb_value = source_pixel_values[row][column];
          result_pixel_values[row][column] = rgb_random_color_mapping[rgb_value.r_][rgb_value.g_][rgb_value.b_];
        }
      }
    }

    void HistogramEqualization(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values) {
      size_t pixel_value_count[256] = {};

      for (size_t row = 0; row < source_pixel_values.size(); ++row) {
        for (size_t column = 0; column < source_pixel_values[row].size(); ++column) {
          RGBA<unsigned char> rgb_value = source_pixel_values[row][column];
          unsigned char gray_value = (rgb_value.r_ + rgb_value.g_ + rgb_value.b_) / 3.0;
          ++pixel_value_count[gray_value];
        }
      }

      size_t total_pixel_count = source_pixel_values.size() * source_pixel_values[0].size();

      unsigned char new_gray_value[256];
      float cdf = 0;

      for (size_t gray_level = 0; gray_level < 256; ++gray_level) {
        cdf += pixel_value_count[gray_level] / (float)total_pixel_count;
        new_gray_value[gray_level] = cdf * 255;
      }


#pragma omp parallel for
      for (int row = 0; row < source_pixel_values.size(); ++row) {
#pragma omp parallel for
        for (int column = 0; column < source_pixel_values[row].size(); ++column) {
          RGBA<unsigned char> rgb_value = source_pixel_values[row][column];
          unsigned char gray_value = (rgb_value.r_ + rgb_value.g_ + rgb_value.b_) / 3.0;
          RGBA<unsigned char> new_rgb_value(new_gray_value[gray_value], new_gray_value[gray_value], new_gray_value[gray_value]);
          result_pixel_values[row][column] = new_rgb_value;
        }
      }
    }

    void Thresholding(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values, const float threshold) {
      for (size_t row = 0; row < source_pixel_values.size(); ++row) {
        for (size_t column = 0; column < source_pixel_values[row].size(); ++column) {
          RGBA<unsigned char> rgb_value = source_pixel_values[row][column];
          float gray_level = (rgb_value.r_ + rgb_value.g_ + rgb_value.b_) / 3.0;
          result_pixel_values[row][column] = (gray_level >= threshold) ? RGBA<unsigned char>(255, 255, 255) : RGBA<unsigned char>(0, 0, 0);
        }
      }
    }

    void MeanFilter(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values, int filter_size) {

      if (!(filter_size & 1)) {
        // Only accept odd size filter
        ++filter_size;
      }

#pragma omp parallel for
      for (int row = 0; row < source_pixel_values.size(); ++row) {
#pragma omp parallel for
        for (int column = 0; column < source_pixel_values[row].size(); ++column) {
          size_t total_pixel_count = 0;
          RGBA<size_t> rgb_sum(0, 0, 0);
          for (int delta_r = -filter_size / 2; delta_r <= filter_size / 2; ++delta_r) {
            for (int delta_c = -filter_size / 2; delta_c <= filter_size / 2; ++delta_c) {
              int neighbor_r = (int)row + delta_r;
              int neighbor_c = (int)column + delta_c;
              if (IsInImage(source_pixel_values, neighbor_r, neighbor_c)) {
                ++total_pixel_count;
                RGBA<unsigned char> rgb_value = source_pixel_values[neighbor_r][neighbor_c];
                rgb_sum.r_ += rgb_value.r_;
                rgb_sum.g_ += rgb_value.g_;
                rgb_sum.b_ += rgb_value.b_;
              }
            }
          }
          if (total_pixel_count) {
            result_pixel_values[row][column] = RGBA<unsigned char>(rgb_sum.r_ / (float)total_pixel_count, rgb_sum.g_ / (float)total_pixel_count, rgb_sum.b_ / (float)total_pixel_count);
          }
        }
      }
    }

    void MeanFilter(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values) {
      MeanFilter(source_pixel_values, result_pixel_values, 3);
    }

    void MedianFilter(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values, int filter_size) {

      if (!(filter_size & 1)) {
        // Only accept odd size filter
        ++filter_size;
      }

      // Gray image only
#pragma omp parallel for
      for (int row = 0; row < source_pixel_values.size(); ++row) {
#pragma omp parallel for
        for (int column = 0; column < source_pixel_values[row].size(); ++column) {
          std::vector<unsigned char> gray_values;
          for (int delta_r = -filter_size / 2; delta_r <= filter_size / 2; ++delta_r) {
            for (int delta_c = -filter_size / 2; delta_c <= filter_size / 2; ++delta_c) {
              int target_r = row + delta_r;
              int target_c = column + delta_c;
              if (IsInImage(source_pixel_values, target_r, target_c)) {
                RGBA<unsigned char> rgb_value = source_pixel_values[target_r][target_c];
                gray_values.push_back(((double)rgb_value.r_ + (double)rgb_value.g_ + (double)rgb_value.b_) / 3.0);
              }
            }
          }

          if (gray_values.size()) {
            std::sort(gray_values.begin(), gray_values.end());
            unsigned char gray_value = gray_values[gray_values.size() / 2];
            result_pixel_values[row][column] = RGBA<unsigned char>(gray_value, gray_value, gray_value);
          }
        }
      }
    }

    void MedianFilter(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values) {
      MedianFilter(source_pixel_values, result_pixel_values, 3);
    }

    void SobelEdgeDetection(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values, const size_t type) {
      // type 0 : vertical
      // type 1 : horizontal
      // type 2 : combined
      if (type < 0 || type > 2) {
        return;
      }

      const static int KERNAL_SIZE = 3;
      const static float VERTICAL_KERNAL[KERNAL_SIZE][KERNAL_SIZE] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
      const static float HORIZONTAL_KERNAL[KERNAL_SIZE][KERNAL_SIZE] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

#pragma omp parallel for
      for (int row = 0; row < source_pixel_values.size(); ++row) {
#pragma omp parallel for
        for (int column = 0; column < source_pixel_values[row].size(); ++column) {
          RGBA<float> rgb_sum_vertical(0, 0, 0);
          RGBA<float> rgb_sum_horizontal(0, 0, 0);
          for (int delta_r = -KERNAL_SIZE / 2; delta_r <= KERNAL_SIZE / 2; ++delta_r) {
            for (int delta_c = -KERNAL_SIZE / 2; delta_c <= KERNAL_SIZE / 2; ++delta_c) {
              int target_r = row + delta_r;
              int target_c = column + delta_c;
              if (IsInImage(source_pixel_values, target_r, target_c)) {
                RGBA<unsigned char> rgb_value = source_pixel_values[target_r][target_c];
                float vertical_weight = VERTICAL_KERNAL[delta_r + KERNAL_SIZE / 2][delta_c + KERNAL_SIZE / 2];
                float horizontal_weight = HORIZONTAL_KERNAL[delta_r + KERNAL_SIZE / 2][delta_c + KERNAL_SIZE / 2];

                rgb_sum_vertical.r_ += rgb_value.r_ * vertical_weight;
                rgb_sum_vertical.g_ += rgb_value.g_ * vertical_weight;
                rgb_sum_vertical.b_ += rgb_value.b_ * vertical_weight;

                rgb_sum_horizontal.r_ += rgb_value.r_ * horizontal_weight;
                rgb_sum_horizontal.g_ += rgb_value.g_ * horizontal_weight;
                rgb_sum_horizontal.b_ += rgb_value.b_ * horizontal_weight;
              }
            }
          }

          rgb_sum_vertical = RGBA<float>(std::abs(rgb_sum_vertical.r_), std::abs(rgb_sum_vertical.g_), std::abs(rgb_sum_vertical.b_));
          rgb_sum_horizontal = RGBA<float>(std::abs(rgb_sum_horizontal.r_), std::abs(rgb_sum_horizontal.g_), std::abs(rgb_sum_horizontal.b_));

          System::Drawing::Color rgb_value = System::Drawing::Color::FromArgb(0, 0, 0);

          RGBA<float> rgb_sum = rgb_sum_vertical;
          if (type == 1) {
            rgb_sum = rgb_sum_horizontal;
          } else if (type == 2) {
            rgb_sum = RGBA<float>(sqrt(rgb_sum_vertical.r_ * rgb_sum_vertical.r_ + rgb_sum_horizontal.r_ * rgb_sum_horizontal.r_),
              sqrt(rgb_sum_vertical.g_ * rgb_sum_vertical.g_ + rgb_sum_horizontal.g_ * rgb_sum_horizontal.g_),
              sqrt(rgb_sum_vertical.b_ * rgb_sum_vertical.b_ + rgb_sum_horizontal.b_ * rgb_sum_horizontal.b_));
          }

          RGBA<float> real_rgb_value(rgb_sum.r_, rgb_sum.g_, rgb_sum.b_);
          real_rgb_value.r_ = std::max(0.0f, real_rgb_value.r_);
          real_rgb_value.g_ = std::max(0.0f, real_rgb_value.g_);
          real_rgb_value.b_ = std::max(0.0f, real_rgb_value.b_);

          real_rgb_value.r_ = std::min(255.0f, real_rgb_value.r_);
          real_rgb_value.g_ = std::min(255.0f, real_rgb_value.g_);
          real_rgb_value.b_ = std::min(255.0f, real_rgb_value.b_);

          rgb_value = System::Drawing::Color::FromArgb(real_rgb_value.r_, real_rgb_value.g_, real_rgb_value.b_);

          result_pixel_values[row][column] = RGBA<unsigned char>(real_rgb_value.r_, real_rgb_value.g_, real_rgb_value.b_);
        }
      }
    }

    void OverlapSobelEdgeDetectionResult(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values, const float threshold) {

      std::vector<std::vector<RGBA<unsigned char> > > edge_pixel_values = source_pixel_values;
      SobelEdgeDetection(source_pixel_values, edge_pixel_values, 2);

      std::vector<std::vector<RGBA<unsigned char> > > edge_binary_pixel_values = source_pixel_values;
      Thresholding(edge_pixel_values, edge_binary_pixel_values, threshold);

      const RGBA<unsigned char> EDGE_COLOR(0, 255, 0);

#pragma omp parallel for
      for (int row = 0; row < source_pixel_values.size(); ++row) {
#pragma omp parallel for
        for (int column = 0; column < source_pixel_values[row].size(); ++column) {
          if (edge_binary_pixel_values[row][column].r_) { // White
            result_pixel_values[row][column] = EDGE_COLOR;
          }
        }
      }
    }
    
    void ConnectComponent(const std::vector<std::vector<RGBA<unsigned char> > > &source_pixel_values, std::vector<std::vector<RGBA<unsigned char> > > &result_pixel_values) {
      
      std::vector<std::vector<int> > visited(source_pixel_values.size(), std::vector<int>(source_pixel_values[0].size(), false));
      std::vector<std::vector<int> > group(source_pixel_values.size(), std::vector<int>(source_pixel_values[0].size(), 0));
      int total_group_count = 0;

      for (size_t row = 0; row < source_pixel_values.size(); ++row) {
        for (size_t column = 0; column < source_pixel_values[0].size(); ++column) {
          if (!visited[row][column]) {
            visited[row][column] = true;
            RGBA<unsigned char> rgb_value = source_pixel_values[row][column];
            if (rgb_value.r_ || rgb_value.g_ || rgb_value.b_) {
              // black pixel only
              continue;
            }
            std::queue<std::pair<int, int> > q;
            q.push(std::make_pair(row, column));
            ++total_group_count;
            while (!q.empty()) {
              int current_r = q.front().first;
              int current_c = q.front().second;
              q.pop();

              RGBA<unsigned char> rgb_value = source_pixel_values[current_r][current_c];
              if (rgb_value.r_ || rgb_value.g_ || rgb_value.b_) {
                // black pixel only
                continue;
              }

              group[current_r][current_c] = total_group_count;
              for (int delta_r = -1; delta_r <= 1; ++delta_r) {
                for (int delta_c = -1; delta_c <= 1; ++delta_c) {
                  if (!delta_r && !delta_c) {
                    continue;
                  }
                  int next_r = current_r + delta_r;
                  int next_c = current_c + delta_c;
                  if (IsInImage(source_pixel_values, next_r, next_c)) {
                    if (!visited[next_r][next_c]) {
                      visited[next_r][next_c] = true;
                      q.push(std::make_pair(next_r, next_c));
                    }
                  }
                }
              }
            }
          }
        }
      }

      std::vector<RGBA<unsigned char> > group_colors(total_group_count + 1);
      group_colors[0] = RGBA<unsigned char>(255, 255, 255);
      for (int i = 1; i < group_colors.size(); ++i) {
        group_colors[i] = RGBA<unsigned char>(rand() % 256, rand() % 256, rand() % 256);
      }

#pragma omp parallel for
      for (int row = 0; row < source_pixel_values.size(); ++row) {
#pragma omp parallel for
        for (int column = 0; column < source_pixel_values[row].size(); ++column) {
          result_pixel_values[row][column] = group_colors[group[row][column]];
        }
      }

      // Homework requirement
      std::cout << "Group count : " << total_group_count << "\n";
    }

    class ImageProcesser {

    public:

      ImageProcesser() : image_loaded_(false), ground_truth_image_loaded_(false) {
      }

      ImageProcesser(System::Drawing::Bitmap ^source_bitmap) : ground_truth_image_loaded_(false) {
        SetPixelValuesFromBitmap(source_bitmap);
      }

      void SetPixelValuesFromBitmap(System::Drawing::Bitmap ^source_bitmap) {
        BitmapToVectorOfPixels(source_bitmap, source_pixel_values_);
        result_pixel_values_ = source_pixel_values_;
        image_loaded_ = source_pixel_values_.size() && source_pixel_values_[0].size();
      }

      void SetGroundTruthPixelValuesFromBitmap(System::Drawing::Bitmap ^source_bitmap) {
        BitmapToVectorOfPixels(source_bitmap, ground_truth_pixel_values_);
        ground_truth_image_loaded_ = ground_truth_pixel_values_.size() && ground_truth_pixel_values_[0].size();
      }

      void ReserveResult() {
        source_pixel_values_ = result_pixel_values_;
      }

      System::Drawing::Bitmap ^GetSourceBitmapFromPixelValues() {
        if (!image_loaded_) {
          return nullptr;
        }

        System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_pixel_values_[0].size(), source_pixel_values_.size(), System::Drawing::Imaging::PixelFormat::Format32bppRgb);

        VectorOfPixelsToBitmap(source_pixel_values_, result_image);

        return result_image;
      }

      System::Drawing::Bitmap ^GetResultBitmapFromPixelValues() {
        if (!image_loaded_) {
          return nullptr;
        }

        System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(result_pixel_values_[0].size(), result_pixel_values_.size(), System::Drawing::Imaging::PixelFormat::Format32bppRgb);

        VectorOfPixelsToBitmap(result_pixel_values_, result_image);

        return result_image;
      }

      void ExtractImage(const size_t extracted_channel) {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::ExtractImage(source_pixel_values_, result_pixel_values_, extracted_channel);
      }

      void RGBToGray() {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::RGBToGray(source_pixel_values_, result_pixel_values_);
      }

      void HistogramEqualization() {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::HistogramEqualization(source_pixel_values_, result_pixel_values_);
      }

      void Thresholding(const float threshold) {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::Thresholding(source_pixel_values_, result_pixel_values_, threshold);
      }

      void MeanFilter() {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::MeanFilter(source_pixel_values_, result_pixel_values_, 3);
      }

      void MedianFilter() {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::MedianFilter(source_pixel_values_, result_pixel_values_, 3);
      }

      void SobelEdgeDetection(const size_t type) {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::SobelEdgeDetection(source_pixel_values_, result_pixel_values_, type);
      }

      void OverlapSobelEdgeDetectionResult(const float threshold) {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::OverlapSobelEdgeDetectionResult(source_pixel_values_, result_pixel_values_, threshold);
      }

      void ConnectComponent() {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::ConnectComponent(source_pixel_values_, result_pixel_values_);
      }

      void RandomColorMapping() {
        if (!image_loaded_) {
          return;
        }
        ImageProcessing::RandomColorMapping(source_pixel_values_, result_pixel_values_);
      }

    private:

      void BitmapToVectorOfPixels(System::Drawing::Bitmap ^source_image, std::vector<std::vector<RGBA<unsigned char> > > &source_image_pixel_values) {
        System::Drawing::Rectangle image_rectangle(0, 0, source_image->Width, source_image->Height);
        System::Drawing::Imaging::BitmapData ^source_image_data = source_image->LockBits(image_rectangle, System::Drawing::Imaging::ImageLockMode::ReadOnly, source_image->PixelFormat);

        source_image_pixel_values = std::vector<std::vector<RGBA<unsigned char> > >(source_image->Height, std::vector<RGBA<unsigned char> >(source_image->Width));

        unsigned char *base_pointer = (unsigned char *)(void *)source_image_data->Scan0;
        size_t bytes_per_pixel = source_image_data->Stride / source_image_data->Width;

        for (size_t r = 0; r < source_image_data->Height; ++r) {
          unsigned char *row_base_pointer = base_pointer + r * source_image_data->Stride;
          for (size_t c = 0; c < source_image_data->Width; ++c) {
            unsigned char *pixel_pointer = row_base_pointer + c * bytes_per_pixel;
            source_image_pixel_values[r][c].r_ = pixel_pointer[2];
            source_image_pixel_values[r][c].g_ = pixel_pointer[1];
            source_image_pixel_values[r][c].b_ = pixel_pointer[0];
          }
        }

        source_image->UnlockBits(source_image_data);
      }

      void VectorOfPixelsToBitmap(const std::vector<std::vector<RGBA<unsigned char> > > &source_image_pixel_values, System::Drawing::Bitmap ^destination_image) {

        if (!source_image_pixel_values.size()) {
          return;
        }

        if (source_image_pixel_values.size() != destination_image->Height || source_image_pixel_values[0].size() != destination_image->Width) {
          return;
        }

        System::Drawing::Rectangle image_rectangle(0, 0, destination_image->Width, destination_image->Height);
        System::Drawing::Imaging::BitmapData ^destination_image_data = destination_image->LockBits(image_rectangle, System::Drawing::Imaging::ImageLockMode::ReadWrite, destination_image->PixelFormat);

        unsigned char *base_pointer = (unsigned char *)(void *)destination_image_data->Scan0;
        size_t bytes_per_pixel = destination_image_data->Stride / destination_image_data->Width;

        for (size_t r = 0; r < destination_image_data->Height; ++r) {
          unsigned char *row_base_pointer = base_pointer + r * destination_image_data->Stride;
          for (size_t c = 0; c < destination_image_data->Width; ++c) {
            unsigned char *pixel_pointer = row_base_pointer + c * bytes_per_pixel;
            pixel_pointer[0] = source_image_pixel_values[r][c].b_;
            pixel_pointer[1] = source_image_pixel_values[r][c].g_;
            pixel_pointer[2] = source_image_pixel_values[r][c].r_;
          }
        }

        destination_image->UnlockBits(destination_image_data);
      }

      bool image_loaded_;
      bool ground_truth_image_loaded_;

      std::vector<std::vector<RGBA<unsigned char> > > source_pixel_values_;
      std::vector<std::vector<RGBA<unsigned char> > > result_pixel_values_;
      std::vector<std::vector<RGBA<unsigned char> > > ground_truth_pixel_values_;
    };
  }
}