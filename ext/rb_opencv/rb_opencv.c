#include <stdio.h>
#include "opencv_cwrap.h"
#include "rb_opencv.h"

VALUE rb_mCV;
VALUE rb_cMat;
VALUE rb_cVideoWriter;

static const rb_data_type_t mat_data_type = {
  "WrappedMatData",
  {0, cv_delete_mat, 0,},
  0, 0,
  RUBY_TYPED_FREE_IMMEDIATELY,
};

static const rb_data_type_t video_writer_data_type = {
  "WrappedVideoWriterData",
  {0, cv_delete_video_writer, 0,},
  0, 0,
  RUBY_TYPED_FREE_IMMEDIATELY,
};

/*
  * call-seq:
  *   new_mat(width, height, type, r, g, b) -> mat
  *
  * Creates a new matrix.
  */
VALUE rb_new_mat(VALUE self, VALUE width, VALUE height, VALUE type, VALUE r, VALUE g, VALUE b) {
  void *mat = cv_new_mat(NUM2INT(width), NUM2INT(height), NUM2INT(type), NUM2INT(r), NUM2INT(g), NUM2INT(b));
  return TypedData_Wrap_Struct(rb_cMat, &mat_data_type, mat);
}

/*
  * call-seq:
  *   new_video_writer(filename, [fourcc], fps, width, height) -> video_writer
  *
  * Creates a new video writer.
  */
VALUE rb_new_video_writer(VALUE self, VALUE filename, VALUE fourcc, VALUE fps, VALUE width, VALUE height) {
  char c1, c2, c3, c4;
  if (NIL_P(fourcc)) {
    c1 = 'X';
    c2 = 'V';
    c3 = 'I';
    c4 = 'D';
  } else {
    VALUE fourcc_str = rb_funcall(fourcc, rb_intern("to_s"), 0);
    char *fourcc_cstr = StringValuePtr(fourcc_str);
    c1 = fourcc_cstr[0];
    c2 = fourcc_cstr[1];
    c3 = fourcc_cstr[2];
    c4 = fourcc_cstr[3];
  }
  void *video_writer = cv_new_video_writer(StringValuePtr(filename), cv_fourcc(c1, c2, c3, c4), NUM2DBL(fps), NUM2INT(width), NUM2INT(height));
  if (!video_writer) rb_raise(rb_eRuntimeError, "Could not open video writer");
  return TypedData_Wrap_Struct(rb_cVideoWriter, &video_writer_data_type, video_writer);
}

/*
  * call-seq:
  *   write(video_writer, mat) -> nil
  *
  * Writes a frame to a video writer.
  */
VALUE rb_video_write(VALUE self, VALUE video_writer, VALUE mat) {
  void *video_writer_ptr;
  TypedData_Get_Struct(video_writer, void, &video_writer_data_type, video_writer_ptr);
  void *mat_ptr;
  TypedData_Get_Struct(mat, void, &mat_data_type, mat_ptr);
  cv_video_write_frame(video_writer_ptr, mat_ptr);
  return Qnil;
}

/*
  * call-seq:
  *   video_release(video_writer) -> nil
  *
  * Releases a video writer.
  */
VALUE rb_video_release(VALUE self, VALUE video_writer) {
  void *video_writer_ptr;
  TypedData_Get_Struct(video_writer, void, &video_writer_data_type, video_writer_ptr);
  cv_video_release(video_writer_ptr);
  return Qnil;
}

/*
  * call-seq:
  *   circle(mat, x, y, radius, r, g, b, thickness, line_type) -> nil
  *
  * Draws a circle on a matrix.
  */
VALUE rb_circle(VALUE self, VALUE mat, VALUE x, VALUE y, VALUE radius, VALUE r, VALUE g, VALUE b, VALUE thickness, VALUE line_type) {
  void *mat_ptr;
  TypedData_Get_Struct(mat, void, &mat_data_type, mat_ptr);
  cv_circle(mat_ptr, NUM2INT(x), NUM2INT(y), NUM2INT(radius), NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(thickness), NUM2INT(line_type));
  return Qnil;
}

/*
  * call-seq:
  *   polyline(mat, points, is_closed, r, g, b, thickness, line_type) -> nil
  *
  * Draws a rectangle on a matrix.
  */
VALUE rb_polyline(VALUE self, VALUE mat, VALUE points, VALUE is_closed, VALUE r, VALUE g, VALUE b, VALUE thickness, VALUE line_type) {
  void *mat_ptr;
  TypedData_Get_Struct(mat, void, &mat_data_type, mat_ptr);
  int num_points = NUM2INT(rb_array_len(points));
  int *points_array = malloc(sizeof(int) * num_points * 2);
  for (int i = 0; i < num_points; i++) {
    VALUE point = rb_ary_entry(points, i);
    points_array[i * 2] = NUM2INT(rb_ary_entry(point, 0));
    points_array[i * 2 + 1] = NUM2INT(rb_ary_entry(point, 1));
  }
  cv_polyline(mat_ptr, points_array, num_points*2, NUM2INT(is_closed), NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(thickness), NUM2INT(line_type));
  free(points_array);
  return Qnil;
}

/*
  * call-seq:
  *   fill_convex_poly(mat, points, r, g, b, line_type) -> nil
  *
  * Draws a rectangle on a matrix.
  */
VALUE rb_fill_convex_poly(VALUE self, VALUE mat, VALUE points, VALUE r, VALUE g, VALUE b, VALUE line_type) {
  void *mat_ptr;
  TypedData_Get_Struct(mat, void, &mat_data_type, mat_ptr);
  int num_points = RARRAY_LEN(points);
  int *points_array = malloc(sizeof(int) * num_points * 2);
  for (int i = 0; i < num_points; i++) {
    VALUE point = rb_ary_entry(points, i);
    points_array[i * 2] = NUM2INT(rb_ary_entry(point, 0));
    points_array[i * 2 + 1] = NUM2INT(rb_ary_entry(point, 1));
  }
  cv_fill_convex_poly(mat_ptr, points_array, num_points*2, NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(line_type));
  free(points_array);
  return Qnil;
}

/*
  * call-seq:
  *   show_mat(mat) -> nil
  *
  * Shows a matrix.
  */
VALUE rb_show_mat(VALUE self, VALUE mat) {
  void *mat_ptr;
  TypedData_Get_Struct(mat, void, &mat_data_type, mat_ptr);
  cv_imshow("image", mat_ptr);
  return Qnil;
}

VALUE rb_opencv_test(VALUE self)
{
  cv_test();
  return Qnil;
}

RUBY_FUNC_EXPORTED void
Init_rb_opencv(void)
{
  rb_mCV = rb_define_module("CV");
  rb_mCV = rb_define_module_under(rb_mCV, "Backend");
  rb_cMat = rb_define_class_under(rb_mCV, "WrappedMatData", rb_cObject);
  rb_cVideoWriter = rb_define_class_under(rb_mCV, "WrappedVideoWriterData", rb_cObject);
  rb_define_module_function(rb_mCV, "test", RUBY_METHOD_FUNC(rb_opencv_test), 0);
  rb_define_singleton_method(rb_mCV, "new_mat", rb_new_mat, 6);
  rb_define_singleton_method(rb_mCV, "show_mat", rb_show_mat, 1);
  rb_define_singleton_method(rb_mCV, "new_video_writer", rb_new_video_writer, 5);
  rb_define_singleton_method(rb_mCV, "video_write", rb_video_write, 2);
  rb_define_singleton_method(rb_mCV, "video_release", rb_video_release, 1);
  rb_define_singleton_method(rb_mCV, "circle", rb_circle, 9);
  rb_define_singleton_method(rb_mCV, "polyline", rb_polyline, 8);
  rb_define_singleton_method(rb_mCV, "fill_convex_poly", rb_fill_convex_poly, 6);
}
