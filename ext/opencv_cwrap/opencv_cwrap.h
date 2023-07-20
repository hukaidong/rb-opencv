//
// Created by kaidong on 7/18/23.
//

#ifndef OPENCV_CWRAP_OPENCV_CWRAP_H
#define OPENCV_CWRAP_OPENCV_CWRAP_H

#ifdef __cplusplus
extern "C" {
#endif
  const int CV_8UC3 = 16;

  void* cv_new_mat(int rows, int cols, int type, int r, int g, int b);
  void cv_delete_mat(void* mat);
  int cv_fourcc(char c1, char c2, char c3, char c4);
  void* cv_new_video_writer(const char* filename, int fourcc, double fps, int width, int height);
  void cv_delete_video_writer(void* writer);
  void cv_video_write_frame(void* writer, void* mat);
  void cv_video_release(void* writer);
  void cv_imshow(const char* winname, void* mat);
  void cv_circle(void* mat, int x, int y, int radius, int r, int g, int b, int thickness, int line_type);
  void cv_polyline(void* mat, void* coords, int len_coords, int is_closed, int r, int g, int b, int thickness, int line_type);
  void cv_fill_convex_poly(void* mat, void* coords, int len_coords, int r, int g, int b, int line_type);
  void cv_test();

#ifdef __cplusplus
};
#endif

#endif //OPENCV_CWRAP_OPENCV_CWRAP_H
