#include <cmath>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

namespace BasicImageProcessing {

  namespace ImageProcessing {
    template<class T>
    class RGBA {

    public:

      RGBA() {
      }

      RGBA(T r, T g, T b) : r_(r), g_(g), b_(b) {
      }

      RGBA(T r, T g, T b, T a) : RGBA(r, g, b), a_(a) {
      }

      RGBA<T> Square() {
        return RGBA<T>(r_ * r_, g_ * g_, b_ * b_, a_);
      }

      RGBA<T> Sqrt() {
        return RGBA<T>(sqrt(r_), sqrt(g_), sqrt(b_), a_);
      }

      T r_, g_, b_, a_;
    };

    RGBA<size_t> rgb_random_color_mapping[256][256][256];

    System::Drawing::Bitmap ^ExtractImage(System::Drawing::Bitmap ^source_image, size_t extracted_channel) {
      // extracted_channel 0 : R
      // extracted_channel 1 : G
      // extracted_channel 2 : B
      if (extracted_channel < 0 || extracted_channel > 2) {
        return nullptr;
      }

      if (!source_image) {
        return nullptr;
      }

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);
      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          System::Drawing::Color rgb_value = source_image->GetPixel(column, row);
          unsigned char extracted_value = rgb_value.R;
          if (extracted_channel == 1) {
            extracted_value = rgb_value.G;
          } else if (extracted_channel == 2) {
            extracted_value = rgb_value.B;
          }
          result_image->SetPixel(column, row, System::Drawing::Color::FromArgb(extracted_value, extracted_value, extracted_value));
        }
      }
      return result_image;
    }

    System::Drawing::Bitmap ^RGBToGray(System::Drawing::Bitmap ^source_image) {
      if (!source_image) {
        return nullptr;
      }

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);
      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          System::Drawing::Color rgb_value = source_image->GetPixel(column, row);
          unsigned char gray_value = (rgb_value.R + rgb_value.G + rgb_value.B) / 3.0;
          result_image->SetPixel(column, row, System::Drawing::Color::FromArgb(gray_value, gray_value, gray_value));
        }
      }
      return result_image;
    }

    System::Drawing::Bitmap ^RandomColorMapping(System::Drawing::Bitmap ^source_image) {
      if (!source_image) {
        return nullptr;
      }

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);

      for (size_t r = 0; r < 256; ++r) {
        for (size_t g = 0; g < 256; ++g) {
          for (size_t b = 0; b < 256; ++b) {
            rgb_random_color_mapping[r][g][b].r_ = rand() % 256;
            rgb_random_color_mapping[r][g][b].g_ = rand() % 256;
            rgb_random_color_mapping[r][g][b].b_ = rand() % 256;
          }
        }
      }


      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          System::Drawing::Color rgb_value = source_image->GetPixel(column, row);
          RGBA<size_t> mapped_rgb = rgb_random_color_mapping[rgb_value.R][rgb_value.G][rgb_value.B];
          result_image->SetPixel(column, row, System::Drawing::Color::FromArgb(mapped_rgb.r_, mapped_rgb.g_, mapped_rgb.b_));
        }
      }
      return result_image;
    }

    System::Drawing::Bitmap ^HistogramEqualization(System::Drawing::Bitmap ^source_image) {
      if (!source_image) {
        return nullptr;
      }

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);

      size_t pixel_value_count[256] = {};

      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          System::Drawing::Color rgb_value = source_image->GetPixel(column, row);
          unsigned char gray_value = (rgb_value.R + rgb_value.G + rgb_value.B) / 3.0;
          ++pixel_value_count[gray_value];
        }
      }

      size_t total_pixel_count = source_image->Height * source_image->Width;

      unsigned char new_gray_value[256];
      float cdf = 0;

      for (size_t gray_level = 0; gray_level < 256; ++gray_level) {
        cdf += pixel_value_count[gray_level] / (float)total_pixel_count;
        new_gray_value[gray_level] = cdf * 255;
      }

      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          System::Drawing::Color rgb_value = source_image->GetPixel(column, row);
          unsigned char gray_value = (rgb_value.R + rgb_value.G + rgb_value.B) / 3.0;
          System::Drawing::Color new_rgb_value = System::Drawing::Color::FromArgb(new_gray_value[gray_value], new_gray_value[gray_value], new_gray_value[gray_value]);
          result_image->SetPixel(column, row, new_rgb_value);
        }
      }

      return result_image;
    }

    System::Drawing::Bitmap ^Threshold(System::Drawing::Bitmap ^source_image, float threshold) {
      if (!source_image) {
        return nullptr;
      }

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);

      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          System::Drawing::Color rgb_value = source_image->GetPixel(column, row);
          float gray_value = (rgb_value.R + rgb_value.G + rgb_value.B) / 3.0;
          float new_gray_value = (gray_value >= threshold) * 255;
          System::Drawing::Color new_rgb_value = System::Drawing::Color::FromArgb(new_gray_value, new_gray_value, new_gray_value);
          result_image->SetPixel(column, row, new_rgb_value);
        }
      }

      return result_image;
    }

    System::Drawing::Bitmap ^MeanFilter(System::Drawing::Bitmap ^source_image, int filter_size) {
      if (!source_image) {
        return nullptr;
      }

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);

      if (!(filter_size & 1)) {
        // Only accept odd size filter
        ++filter_size;
      }

      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          size_t total_pixel_count = 0;
          RGBA<size_t> rgb_sum(0, 0, 0);
          for (int delta_r = -filter_size / 2; delta_r <= filter_size / 2; ++delta_r) {
            for (int delta_c = -filter_size / 2; delta_c <= filter_size / 2; ++delta_c) {
              int target_r = row + delta_r;
              int target_c = column + delta_c;
              if (target_r >= 0 && target_r < source_image->Height && target_c >= 0 && target_c < source_image->Width) {
                ++total_pixel_count;
                System::Drawing::Color rgb_value = source_image->GetPixel(target_c, target_r);
                rgb_sum.r_ += rgb_value.R;
                rgb_sum.g_ += rgb_value.G;
                rgb_sum.b_ += rgb_value.B;
              }
            }
          }
          System::Drawing::Color rgb_value = System::Drawing::Color::FromArgb(0, 0, 0);
          if (total_pixel_count) {
            rgb_value = System::Drawing::Color::FromArgb(rgb_sum.r_ / (float)total_pixel_count, rgb_sum.g_ / (float)total_pixel_count, rgb_sum.b_ / (float)total_pixel_count);
          }
          result_image->SetPixel(column, row, rgb_value);
        }
      }

      return result_image;
    }

    System::Drawing::Bitmap ^MeanFilter(System::Drawing::Bitmap ^source_image) {
      return MeanFilter(source_image, 3);
    }

    System::Drawing::Bitmap ^MedianFilter(System::Drawing::Bitmap ^source_image, int filter_size) {
      if (!source_image) {
        return nullptr;
      }

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);

      if (!(filter_size & 1)) {
        // Only accept odd size filter
        ++filter_size;
      }

      // Gray image only
      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          std::vector<unsigned char> gray_values;
          for (int delta_r = -filter_size / 2; delta_r <= filter_size / 2; ++delta_r) {
            for (int delta_c = -filter_size / 2; delta_c <= filter_size / 2; ++delta_c) {
              int target_r = row + delta_r;
              int target_c = column + delta_c;
              if (target_r >= 0 && target_r < source_image->Height && target_c >= 0 && target_c < source_image->Width) {
                System::Drawing::Color rgb_value = source_image->GetPixel(target_c, target_r);
                gray_values.push_back((rgb_value.R + rgb_value.G + rgb_value.B) / 3.0);
              }
            }
          }
          System::Drawing::Color rgb_value = System::Drawing::Color::FromArgb(0, 0, 0);
          if (gray_values.size()) {
            std::sort(gray_values.begin(), gray_values.end());
            unsigned char gray_value = gray_values[gray_values.size() / 2];
            rgb_value = System::Drawing::Color::FromArgb(gray_value, gray_value, gray_value);
          }
          result_image->SetPixel(column, row, rgb_value);
        }
      }

      return result_image;
    }

    System::Drawing::Bitmap ^MedianFilter(System::Drawing::Bitmap ^source_image) {
      return MedianFilter(source_image, 3);
    }

    System::Drawing::Bitmap ^SobelEdgeDetection(System::Drawing::Bitmap ^source_image, size_t type) {
      // type 0 : vertical
      // type 1 : horizontal
      // type 2 : combined
      if (type < 0 || type > 2) {
        return nullptr;
      }

      if (!source_image) {
        return nullptr;
      }

      const static int KERNAL_SIZE = 3;
      const static float VERTICAL_KERNAL[KERNAL_SIZE][KERNAL_SIZE] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
      const static float HORIZONTAL_KERNAL[KERNAL_SIZE][KERNAL_SIZE] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);

      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          RGBA<float> rgb_sum_vertical(0, 0, 0);
          RGBA<float> rgb_sum_horizontal(0, 0, 0);
          for (int delta_r = -KERNAL_SIZE / 2; delta_r <= KERNAL_SIZE / 2; ++delta_r) {
            for (int delta_c = -KERNAL_SIZE / 2; delta_c <= KERNAL_SIZE / 2; ++delta_c) {
              int target_r = row + delta_r;
              int target_c = column + delta_c;
              if (target_r >= 0 && target_r < source_image->Height && target_c >= 0 && target_c < source_image->Width) {
                System::Drawing::Color rgb_value = source_image->GetPixel(target_c, target_r);
                float vertical_weight = VERTICAL_KERNAL[delta_r + KERNAL_SIZE / 2][delta_c + KERNAL_SIZE / 2];
                float horizontal_weight = HORIZONTAL_KERNAL[delta_r + KERNAL_SIZE / 2][delta_c + KERNAL_SIZE / 2];

                rgb_sum_vertical.r_ += rgb_value.R * vertical_weight;
                rgb_sum_vertical.g_ += rgb_value.G * vertical_weight;
                rgb_sum_vertical.b_ += rgb_value.B * vertical_weight;

                rgb_sum_horizontal.r_ += rgb_value.R * horizontal_weight;
                rgb_sum_horizontal.g_ += rgb_value.G * horizontal_weight;
                rgb_sum_horizontal.b_ += rgb_value.B * horizontal_weight;
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

          result_image->SetPixel(column, row, rgb_value);
        }
      }

      return result_image;
    }

    System::Drawing::Bitmap ^OverlapSobelEdgeDetectionResult(System::Drawing::Bitmap ^source_image, float threshold) {
      if (!source_image) {
        return nullptr;
      }

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);

      System::Drawing::Bitmap ^edge_binary_image = Threshold(SobelEdgeDetection(source_image, 2), threshold);

      const System::Drawing::Color EDGE_COLOR = System::Drawing::Color::FromArgb(0, 255, 0);

      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          if (edge_binary_image->GetPixel(column, row).R == 255) {
            result_image->SetPixel(column, row, EDGE_COLOR);
          }
        }
      }

      return result_image;
    }


    System::Drawing::Bitmap ^ConnectComponent(System::Drawing::Bitmap ^source_image) {
      if (!source_image) {
        return nullptr;
      }

      System::Drawing::Bitmap ^result_image = gcnew System::Drawing::Bitmap(source_image);

      std::vector<std::vector<int> > visited(source_image->Height, std::vector<int>(source_image->Width, false));
      std::vector<std::vector<int> > group(source_image->Height, std::vector<int>(source_image->Width, 0));
      int total_group_count = 0;

      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          if (!visited[row][column]) {
            visited[row][column] = true;
            System::Drawing::Color rgb_value = source_image->GetPixel(column, row);
            if (rgb_value.R || rgb_value.G || rgb_value.B) {
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

              System::Drawing::Color rgb_value = source_image->GetPixel(current_c, current_r);
              if (rgb_value.R || rgb_value.G || rgb_value.B) {
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
                  if (next_r >= 0 && next_r < source_image->Height && next_c >= 0 && next_c < source_image->Width) {
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

      for (size_t row = 0; row < source_image->Height; ++row) {
        for (size_t column = 0; column < source_image->Width; ++column) {
          System::Drawing::Color rgb_value = System::Drawing::Color::FromArgb(group_colors[group[row][column]].r_, group_colors[group[row][column]].g_, group_colors[group[row][column]].b_);
          result_image->SetPixel(column, row, rgb_value);
        }
      }

      // Homework requirement
      std::cout << "Group count : " << total_group_count << "\n";

      return result_image;
    }

  };
}