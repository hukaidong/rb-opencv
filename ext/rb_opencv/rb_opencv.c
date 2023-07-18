#include "rb_opencv.h"

VALUE rb_mRbOpencv;

RUBY_FUNC_EXPORTED void
Init_rb_opencv(void)
{
  rb_mRbOpencv = rb_define_module("RbOpencv");
}
