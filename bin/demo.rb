#!/usr/bin/env ruby
# frozen_string_literal: true

require "bundler/setup"
require "rb_opencv"

video_writer = CV::Backend.new_video_writer("output.avi", "X264", 30, 480, 640)
300.times do |i|
  mat = CV::Backend.new_mat(640, 480, 16, 0, 0, 0)
  CV::Backend.fill_convex_poly(mat, [[10, 10], [10, 20+i], [20+i, 20+i], [20+i, 10]], 255, 255, 255, 16)
  CV::Backend.show_mat(mat)
  CV::Backend.video_write(video_writer, mat)
end
CV::Backend.video_release(video_writer)
