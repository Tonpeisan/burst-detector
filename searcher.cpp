#include "searcher.h"

void search_edit_points(string src, deque<double> *edit_points)
{
    //open
    VideoCapture cap(src);
    if(!cap.isOpened())
    {
        throw "Error:File can not open:" + src;
    }
    double framerate = cap.get(CAP_PROP_FPS);
    cout << "INFO:Frame rate is " << framerate << " fps" << endl;
    Mat frame;
    Mat resized_frame;

    Mat icon = imread("icon.png");
    resize(icon, icon, Size(20, 20));

    Mat gameset_image = imread("gemeset_template.png");

    int number_of_frame = 0;
    int last_detected = -framerate * 3;

    cout << "INFO:SEARCH START. It may take time." << endl;
    while (cap.read(frame))
    {
        resize(frame, resized_frame, Size(640, 360));
        Mat cropped_frame(resized_frame, Rect(75, 170, 500, 30));

        Mat matching_result;
        matchTemplate(cropped_frame, icon, matching_result, TM_CCOEFF_NORMED);

        Mat match_mask = Mat::zeros(matching_result.size(), CV_8UC1);
        match_mask.setTo(1, matching_result > 0.60);
        int stock = sum(match_mask)[0];
        if (stock > 0 && (number_of_frame - last_detected > framerate * 3))
        {
            double timestamp = number_of_frame / framerate;
            edit_points->push_back(timestamp);
            cout << "INFO:BURST DETECTED AT " << timestamp << " sec." << endl;
            last_detected = number_of_frame;
        }

        // detect gameset
        Mat cropped_frame_for_gameset(resized_frame, Rect(116, 44, 414, 204));
        matchTemplate(cropped_frame_for_gameset, gameset_image, matching_result, TM_CCOEFF_NORMED);
        match_mask = Mat::zeros(matching_result.size(), CV_8UC1);
        match_mask.setTo(1, matching_result > 0.60);
        stock = sum(match_mask)[0];
        if (stock > 0 && (number_of_frame - last_detected > framerate * 3))
        {
            double timestamp = number_of_frame / framerate;
            edit_points->push_back(timestamp);
            cout << "INFO:GAME SET IS DETECTED AT " << timestamp << " sec." << endl;
            last_detected = number_of_frame;
        }
        ++number_of_frame;
    }
    cout << "INFO:SEARCH COMPLETED" << endl;
}