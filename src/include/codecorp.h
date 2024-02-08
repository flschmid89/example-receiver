#include <stdio.h>

#include <stdlib.h>
#include "crdDefs.h"
#include <vector>
#include <functional>
#include <atomic>
#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#define P_UNLOCK_INDIV_KEY 90


#define ERR_OEM_KEY_NOT_FOUND 904 // from crdConfig.h

int handle;
struct Result 
{
    int dataLength = 0;
    int symbolType = 0;
    int symbolModifier = 0;
    unsigned long decodeTime = 0;
    unsigned long totalTime = 0;
    char databuffer[1000] = "";
    CRPOINT bound[4];
    CRPOINT center;
};


cv::Point2i convertPoint (const CRPOINT& point)
{
    return cv::Point2i(point.x, point.y);
};

struct DecoderAppSettings {
    bool ENABLE_DM = true;
    bool ENABLE_C128 = true;
    bool ENABLE_C39 = true;
    bool ENABLE_I25 = true;
    bool ENABLE_CBAR = true;
    bool ENABLE_C93 = true;
    bool ENABLE_UPCA = true;
    bool ENABLE_EAN13 = true;
    bool ENABLE_EAN8 = true;
    bool ENABLE_UPCE = true;
    bool UPC_E_EXPANSION = true;
    bool UPC_SUPPLEMENT_ENABLE_ALL = true;
    bool ENABLE_DB_14 = true;
    bool ENABLE_DB_14_STACKED = true;
    bool ENABLE_DB_LIMITED = true;
    bool ENABLE_DB_EXPANDED = true;
}   ;





DecoderAppSettings decoderAppSettings;

struct DecoderAppsSettingsDescription {
    std::shared_ptr<bool> address;
    std::string name;
    std::string description;
    int id = 0;

};
std::vector<DecoderAppsSettingsDescription> decoderAppsSettingsDescriptions = {
    {std::make_shared<bool>(decoderAppSettings.ENABLE_DM), "ENABLE_DM", "Enable Data Matrix", P_ENABLE_DM},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_C128), "ENABLE_C128", "Enable Code 128", P_ENABLE_C128},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_C39), "ENABLE_C39", "Enable Code 39", P_ENABLE_C39},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_I25), "ENABLE_I25", "Enable Interleaved 2 of 5", P_ENABLE_I25},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_CBAR), "ENABLE_CBAR", "Enable Codabar", P_ENABLE_CBAR},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_C93), "ENABLE_C93", "Enable Code 93",P_ENABLE_C93},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_UPCA), "ENABLE_UPCA", "Enable UPC-A", P_ENABLE_UPCA},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_EAN13), "ENABLE_EAN13", "Enable EAN-13", P_ENABLE_EAN13},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_EAN8), "ENABLE_EAN8", "Enable EAN-8", P_ENABLE_EAN8},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_UPCE), "ENABLE_UPCE", "Enable UPC-E", P_ENABLE_UPCE},
    {std::make_shared<bool>(decoderAppSettings.UPC_E_EXPANSION), "UPC_E_EXPANSION", "Enable UPC-E expansion", P_UPC_E_EXPANSION},
    {std::make_shared<bool>(decoderAppSettings.UPC_SUPPLEMENT_ENABLE_ALL), "UPC_SUPPLEMENT_ENABLE_ALL", "Enable UPC supplement", P_UPC_SUPPLEMENT_ENABLE_ALL},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_DB_14), "ENABLE_DB_14", "Enable Databar", P_ENABLE_DB_14},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_DB_14_STACKED), "ENABLE_DB_14_STACKED", "Enable Databar stacked", P_ENABLE_DB_14_STACKED},
    {std::make_shared<bool>(decoderAppSettings.ENABLE_DB_LIMITED), "ENABLE_DB_LIMITED", "Enable Databar limited", P_ENABLE_DB_LIMITED},

};
std::vector<Result> results;
int numDecoded;

std::vector<Result> useEdgeDecoder(unsigned char *imgbuf, int width, int height);
std::vector<Result> useEdgeDecoder(cv::Mat &image);

int initEdgeDecoder(char *keyString);
int OnResult(int handle);
int OnProgress(int handle);
std::atomic<bool> running = false;

std::vector<std::function<void()>> callbackDecoder;
cv::Mat imgbuffer;
int tryDecoder()
{
    int i, j;
    char keyString[] = "326D2BC480C943F6B46C"; /* Key string to unlock decoder */

    numDecoded = 0;

    /* Initialize the decoder.  Once is sufficient */
    if (initEdgeDecoder(keyString) == -1)
    {
        return -1;
    }

    imgbuffer = cv::imread("/home/notavis/barcode-app/data-matrix-qr-codes-1_0.jpg", cv::IMREAD_GRAYSCALE);
    std::cout << "Image size: " << imgbuffer.cols << "x" << imgbuffer.rows << std::endl;
    int width = imgbuffer.cols;

    /* You can call the decoder many times by changing the image, ROI, */
    /* symbologies, etc., without having to reinitialize the decoder   */
    useEdgeDecoder(imgbuffer.data, imgbuffer.cols, imgbuffer.rows);

    if (numDecoded > 0)
    {
        for (auto &result: results)
        {
            printf("Decoding time = %lu ms\n", result.decodeTime);
                        printf("data length = %d\n", result.dataLength);
                        printf("result: ");

            for (i = 0; i < result.dataLength; i++)
            {
                printf("%c", result.databuffer[i]);
            }
            printf("\n\n");
            int lineThickness = 4;
            cv::circle(imgbuffer, convertPoint(result.center), 5, cv::Scalar(0, 0, 255), -1);
            cv::line(imgbuffer, convertPoint(result.bound[0]), convertPoint(result.bound[1]), cv::Scalar(0, 0, 255), lineThickness);
            cv::line(imgbuffer, convertPoint(result.bound[1]), convertPoint(result.bound[2]), cv::Scalar(0, 0, 255), lineThickness);
            cv::line(imgbuffer, convertPoint(result.bound[2]), convertPoint(result.bound[3]), cv::Scalar(0, 0, 255), lineThickness);
            cv::line(imgbuffer, convertPoint(result.bound[3]), convertPoint(result.bound[0]), cv::Scalar(0, 0, 255), lineThickness);

        }
       


    }
    else
    {
        /* Decode failed */
        printf("decode failure\n");
    }

    cv::imwrite("result.jpg", imgbuffer);

    /* Call this to release the resources before exiting the decoder */
    // CRD_Destroy(handle);

    return 0;
}

int initEdgeDecoder( char *keyString)
{
    handle = CRD_Create();
    if (handle == 0)
    {
        return -1;
    }



    /* Install the callback function */
    CRD_Set(handle, P_CALLBACK_RESULT, reinterpret_cast<void *>(OnResult));
    CRD_Set(handle, P_CALLBACK_PROGRESS, reinterpret_cast<void *>(OnProgress));

    int minSize = 0;
    int maxSize = 0;
    CRD_Get(handle, P_MINIMUM_SIZE_DM, &minSize);
    CRD_Get(handle, P_MAXIMUM_SIZE_DM, &maxSize);

    std::cout << "Minimum size: " << minSize << std::endl;
    std::cout << "Maximum size: " << maxSize << std::endl;

    /* Enable/disable additional symbologies (enable - 1, disable - 0) */

    /* Unlock the decoder using your key */
    CRD_Set(handle, P_UNLOCK_INDIV_KEY,keyString);

    return 0;
}



std::vector<Result> useEdgeDecoder(unsigned char *imgbuf, int width, int height)
{
   results.clear();
   for(auto &decoderAppsSettingsDescription: decoderAppsSettingsDescriptions)
    {
            CRD_Set(handle, decoderAppsSettingsDescription.id, decoderAppsSettingsDescription.address.get());

    }
    /*Set image properties */
    CRD_Set(handle, P_IMAGE_ADDRESS, imgbuf);
    CRD_SetInt(handle, P_IMAGE_WIDTH, width);
    CRD_SetInt(handle, P_IMAGE_HEIGHT, height);
    CRD_SetInt(handle, P_IMAGE_BUF_WIDTH, width);

    /* Set Region of Interest for decoding */
    CRD_SetInt(handle, P_BC_ROI_LEFT, 0);
    CRD_SetInt(handle, P_BC_ROI_TOP, 0);
    CRD_SetInt(handle, P_BC_ROI_WIDTH, width);
    CRD_SetInt(handle, P_BC_ROI_HEIGHT, height);

    /* Call decoder */
    if (CRD_Decode(handle) == ERR_OEM_KEY_NOT_FOUND)
        printf("Incorrect License Key!!!\n");

    return results;
}
std::vector<Result> useEdgeDecoder(cv::Mat &image)
{
    return useEdgeDecoder(image.data, image.cols, image.rows);
}

int OnResult(int handle)
{
    Result result;
    CRD_Get(handle, P_RESULT_LENGTH, &(result.dataLength));
    CRD_Get(handle, P_RESULT_STRING, &(result.databuffer)[0]);
    CRD_Get(handle, P_RESULT_CORNERS,&(result.bound)[0]);
    CRD_Get(handle, P_RESULT_CENTER, &(result.center));
    CRD_Get(handle, P_RESULT_SYMBOL_TYPE, &(result.symbolType));
    CRD_Get(handle, P_RESULT_SYMBOL_MODIFIER, &(result.symbolModifier));
    CRD_Get(handle, P_RESULT_DECODE_TIME, &(result.decodeTime));

    for (auto &callback : callbackDecoder)
    {
        callback();
    }
    results.push_back(std::move(result));
    LOG_F(INFO, "Decoding time = %lu ms", result.decodeTime);

    /* Setting the following will terminate the decoding.  If not set,   */
    /* the decoder will attempt to decode the next barcode in the image. */
    // CRD_SetInt(handle, P_STOP_DECODE, 1);

    /* Increment number of barcodes found */
    numDecoded++;

    return 0;
}

int OnProgress(int handle)
{
    /* This is called by the decoder every 1 ms. You can do something optional here */
    return 0;
}