#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <mutex>
class BlobDetector {
     cv::Ptr<cv::SimpleBlobDetector> detector;
      cv::SimpleBlobDetector::Params params;
      std::mutex mutex;

public:
    double scaleFactor = 0.25;

    BlobDetector() {
        params.filterByColor = true;
        params.filterByArea = false;
        params.filterByCircularity = false;
        params.filterByConvexity = false;
        params.filterByInertia = false;
        
        params.blobColor = 255;
        detector = cv::SimpleBlobDetector::create(params);
    


    }

    void setThreshold(int threshold) {
        std::unique_lock<std::mutex> lock(mutex);
        params.blobColor = threshold;
        detector = cv::SimpleBlobDetector::create(params);
    }
    cv::Mat binarizeImage(const cv::Mat& image, int threshold) {
        cv::Mat binaryImage;
        cv::threshold(image, binaryImage, threshold, 255, cv::THRESH_BINARY);
        return binaryImage;
    }

    nlohmann::json detectBlobs( cv::Mat& image, int threshold) {

        cv::Mat imageGray;

        if(image.channels() > 1)
        {
            LOG_F(INFO, "Image is not grayscale, converting to grayscale");
            cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
                                LOG_F(INFO, "Image size: %d x %d", imageGray.cols, imageGray.rows);


        }
        else
        {
                    LOG_F(INFO, "Image size: %d x %d", image.cols, image.rows);

            imageGray = image.clone();

            cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
        }
        LOG_F(INFO, "Image size: %d x %d", imageGray.cols, imageGray.rows);


        

        // Create a new matrix to hold the resized image
        cv::Mat resizedImage;

        // Resize the image
        cv::resize(imageGray, resizedImage, cv::Size(), scaleFactor, scaleFactor);


        // Log the mean value
        imageGray = binarizeImage(resizedImage, threshold);

        
        LOG_F(INFO, "Threshold: %d", threshold);
        std::unique_lock<std::mutex> lock(mutex);

        std::vector<cv::KeyPoint> keypoints;
        detector->detect(imageGray, keypoints);
        lock.unlock();

        nlohmann::json blobs;
        for (const auto& keypoint : keypoints) {
            nlohmann::json blob;
            blob["x"] = keypoint.pt.x /scaleFactor;
            blob["y"] = keypoint.pt.y /scaleFactor;
            blob["size"] = keypoint.size;
            blobs.push_back(blob);

            // Draw rectangle around the keypoint
            cv::Point topLeft(keypoint.pt.x / scaleFactor - keypoint.size / 2 /scaleFactor , keypoint.pt.y /scaleFactor - keypoint.size / 2 /scaleFactor);
            cv::Point bottomRight(keypoint.pt.x /scaleFactor+ keypoint.size / 2/scaleFactor, keypoint.pt.y/scaleFactor + keypoint.size / 2/scaleFactor);
                        LOG_F(INFO, "Top Left: (%d, %d)", topLeft.x, topLeft.y);
            LOG_F(INFO, "Bottom Right: (%d, %d)", bottomRight.x, bottomRight.y);
            cv::rectangle(image, topLeft, bottomRight, cv::Scalar(0, 255, 0), 2);
        }


        return blobs;
    }
};
