#include "opencv_cwrap.h"
#include <opencv2/opencv.hpp>

void* cv_new_mat(int rows, int cols, int type, int scalar_r, int scalar_g, int scalar_b) {
    cv::Mat* mat = new cv::Mat(rows, cols, type, cv::Scalar(scalar_r, scalar_g, scalar_b));
    return (void*)mat;
}

void cv_delete_mat(void* mat) {
    delete (cv::Mat*)mat;
}

int cv_fourcc(char c1, char c2, char c3, char c4) {
    return cv::VideoWriter::fourcc(c1, c2, c3, c4);
}

void* cv_new_video_writer(const char* filename, int fourcc, double fps, int width, int height) {
    cv::VideoWriter* writer = new cv::VideoWriter(filename, fourcc, fps, cv::Size(width, height));
    if (!writer->isOpened()) {
        delete writer;
        return nullptr;
    }
    return (void*)writer;
}

void cv_delete_video_writer(void* writer) {
    delete (cv::VideoWriter*)writer;
}

void cv_video_write_frame(void* writer, void* mat) {
    ((cv::VideoWriter*)writer)->write(*(cv::Mat*)mat);
}

void cv_video_release(void* writer) {
    ((cv::VideoWriter*)writer)->release();
}

void cv_circle(void* mat, int x, int y, int radius, int r, int g, int b, int thickness) {
    cv::circle(*(cv::Mat*)mat, cv::Point(x, y), radius, cv::Scalar(r, g, b), thickness);
}

void cv_polyline(void* mat, void* coords, int len_coords, int is_closed, int r, int g, int b, int thickness, int line_type) {
    cv::Mat* m = (cv::Mat*)mat;
    int* c = (int*)coords;
    std::vector<cv::Point> pts;
    for (int i = 0; i < len_coords; i += 2) {
        pts.push_back(cv::Point(c[i], c[i + 1]));
    }
    cv::polylines(*m, pts, (bool)is_closed, cv::Scalar(r, g, b), thickness, line_type);
}

void cv_fill_convex_poly(void* mat, void* coords, int len_coords, int r, int g, int b, int line_type) {
    cv::Mat* m = (cv::Mat*)mat;
    int *c = (int*)coords;
    std::vector<cv::Point> pts;
    for (int i = 0; i < len_coords; i += 2) {
        pts.push_back(cv::Point(c[i], c[i + 1]));
    }
    cv::fillConvexPoly(*m, pts, cv::Scalar(r, g, b), line_type);
}

void cv_imshow(const char* winname, void* mat) {
    cv::imshow(winname, *(cv::Mat*)mat);
    cv::waitKey(1);
}

void cv_test()
{
    cv::Mat image(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Point p1{100, 100};
    cv::Point p2{100, 200};
    cv::Point p3{200, 200};
    cv::Point p4{200, 100};
    cv::fillConvexPoly(image, std::array<cv::Point, 4>{p1, p2, p3, p4}, cv::Scalar(0, 0, 0), cv::LINE_AA);
    cv::imshow("image", image);
    cv::waitKey(0);
}
