# frozen_string_literal: true

require "mkmf"
require 'fileutils'

# Makes all symbols private by default to avoid unintended conflict
# with other gems. To explicitly export symbols you can use RUBY_FUNC_EXPORTED
# selectively, or entirely remove this flag.
append_cflags("-fvisibility=hidden")

def prepare_opencv_cwrap
  FileUtils.cd(File.expand_path("../../opencv_cwrap/build", __FILE__)) do
    clean_target = Dir.glob("*")
    FileUtils.rm_rf(clean_target)

    puts "Building opencv_cwrap..."
    system("cmake ..")
    system("make")
    puts "Done."
  end
end

prepare_opencv_cwrap

find_header("opencv_cwrap.h", File.expand_path("../../opencv_cwrap", __FILE__))
find_library("opencv_cwrap", nil, File.expand_path("../../opencv_cwrap/build", __FILE__))

have_header("opencv_cwrap.h")
have_library("opencv_cwrap")

create_makefile("rb_opencv/rb_opencv")
