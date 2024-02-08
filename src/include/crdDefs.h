/**************************************************************************************
*
* Code Confidential and Proprietary.
* Dissemination or reproduction of this information is forbidden.
* 2010 - 2023 Code. All Rights Reserved.
*
**************************************************************************************/
#ifndef CRD_DEFS_H
#define CRD_DEFS_H

#define CD_VERSION_MAJOR                    23
#define CD_VERSION_MINOR                    1
#define CD_VERSION_REVISION                 1
#define CD_VERSION_STRING                   "cd(23.1.1)"

/* P for Property, V for Value */

/* Properties - Image */
#define P_IMAGE_ADDRESS                     0
#define P_IMAGE_WIDTH                       1
#define P_IMAGE_HEIGHT                      2
#define P_IMAGE_BUF_WIDTH                   3
#define P_IMAGE_TYPE                        4
#define P_IMAGE_ADDRESS_EX                  5

#define P_CODE_RESERVED_1                   8
#define P_CODE_RESERVED_2                   9

/* Properties - Callback */
#define P_CALLBACK_RESULT                   10
#define P_CALLBACK_PROGRESS                 11
#define P_CALLBACK_STATUS                   12
#define P_CALLBACK_RESULT_EX                13
#define P_CALLBACK_RESULT_OBJ               14

#define P_CODE_RESERVED_3                   20
#define P_AGC_BRIGHTNESS_ONLY               21
#define P_AGC_BRIGHTNESS_QUALITY            22
#define P_CODE_RESERVED_14                  23
#define P_REGION_DETECT_BARCODE_1DEDGE      24
#define P_REGION_DETECT_BARCODE_FFT         25
#define P_REGION_DETECT_BARCODE_1DEDGEFFT   26
#define P_REGION_DETECT_CELLPHONE           27
#define P_AGC_COMPOUND_GAIN_INITIAL         28
#define P_AGC_COMPOUND_GAIN                 29
#define P_AGC_DESIRED_LIGHT                 30
#define P_CODE_RESERVED_19                  31
#define P_CODE_RESERVED_20                  32
#define P_CODE_RESERVED_21                  33
#define P_CODE_RESERVED_22                  34
#define P_VERIFICATION_TYPE                 35
#define P_CODE_RESERVED_23                  36
#define P_CODE_RESERVED_24                  37
#define P_CODE_RESERVED_25                  38
#define P_REGION_DETECT_SPECULAR            39
#define P_AGC_CURVE_ROI                     40
#define P_CODE_RESERVED_26                  41
#define P_CODE_RESERVED_27                  42
#define P_PREPROCESS_TYPE                   43
#define P_AGC_DESIRED_LIGHT_QUICK_DECODE    44
#define P_MOTION_DETECT_TYPE                45
#define P_ILLUMINATION_TYPE                 46
#define P_MOTION_FIRST_IMAGE                47
#define P_QUALITY_CELLPHONE_DETECTION       48
#define P_MOTION_SENSITIVITY                49
#define P_AGC_IMAGE_ADDRESS                 50
#define P_AGC_IMAGE_WIDTH                   51
#define P_AGC_IMAGE_HEIGHT                  52
#define P_AGC_IMAGE_BUF_WIDTH               53
#define P_TRIGGER_COUNT                     54
#define P_CODE_RESERVED_39                  87
#define P_CODE_RESERVED_38                  88
#define P_CODE_RESERVED_37                  89
#define P_CODE_RESERVED_36                  90
#define P_CODE_RESERVED_35                  91
#define P_CODE_RESERVED_34                  92
#define P_CODE_RESERVED_33                  93
#define P_CODE_RESERVED_32                  94
#define P_CODE_RESERVED_31                  95
#define P_CODE_RESERVED_30                  96
#define P_ENABLE_1D_METRIC                  97
#define P_CODE_RESERVED_29                  98
#define P_CODE_RESERVED_28                  99

/* Properties - Symbology Types */
#define P_ENABLE_GC                         100
#define P_ENABLE_DM                         101
#define P_ENABLE_QR_2005                    102
#define P_ENABLE_AZ                         103
#define P_ENABLE_MAXICODE                   104
#define P_ENABLE_PDF                        105
#define P_ENABLE_MICRO_PDF                  106
#define P_ENABLE_CCA                        107
#define P_ENABLE_CCB                        108
#define P_ENABLE_CCC                        109
#define P_ENABLE_C128                       110
#define P_ENABLE_C39                        111
#define P_ENABLE_C93                        112
#define P_ENABLE_I25                        113
#define P_ENABLE_CBAR                       114
#define P_ENABLE_UPCA                       115
#define P_ENABLE_UPCE                       116
#define P_ENABLE_EAN13                      117
#define P_ENABLE_EAN8                       118
#define P_ENABLE_DB_14                      119
#define P_ENABLE_DB_14_STACKED              120
#define P_ENABLE_DB_LIMITED                 121
#define P_ENABLE_DB_EXPANDED                122
#define P_ENABLE_DB_EXPANDED_STACKED        123
#define P_ENABLE_ALL_COMMON1D               124
#define P_ENABLE_ALL_2D                     125
#define P_ENABLE_ALL_2D_COMMON1D            126
#define P_ENABLE_HX                         127
#define P_ENABLE_MICRO_QR                   128
#define P_ENABLE_QR_MODEL1                  129
#define P_ENABLE_GM                         130

#define P_ENABLE_CUSTOM_NC                  131
#define P_ENABLE_CUSTOM_02                  132
#define P_ENABLE_DC                         133
#define P_ENABLE_ALL_EXTENDED               134     /* Extended symbologies that are not common 1D or 2D */

#define P_ENABLE_C11                        140
#define P_ENABLE_C32                        141
#define P_ENABLE_PLE                        142
#define P_ENABLE_MSI_PLE                    143
#define P_ENABLE_TLP                        144
#define P_ENABLE_TRI                        145
#define P_ENABLE_PHA                        146
#define P_ENABLE_M25                        147
#define P_ENABLE_S25                        148
#define P_ENABLE_C49                        149
#define P_ENABLE_C16K                       150
#define P_ENABLE_CBLK                       151
#define P_ENABLE_HK25                       152
#define P_ENABLE_POSTNET                    153
#define P_ENABLE_PLANET                     154
#define P_ENABLE_INTEL_MAIL                 155
#define P_ENABLE_UPU                        156
#define P_ENABLE_AUSTRA_POST                157
#define P_ENABLE_DUTCH_POST                 158
#define P_ENABLE_JAPAN_POST                 159
#define P_ENABLE_ROYAL_MAIL                 160
#define P_ENABLE_KOREA_POST                 161
#define P_ENABLE_NEC25                      162
#define P_ENABLE_IATA25                     163
#define P_ENABLE_CANADA_POST                164
#define P_ENABLE_BC412                      165
#define P_ENABLE_TLC39                      166

/* Properties - Symbology Checksum Options */
#define P_CHECKSUM_C39                      200
#define P_CHECKSUM_I25                      201
#define P_CHECKSUM_CBAR                     202

/* Properties - Symbology Configuration Options */
#define P_FULL_ASCII_C39                    203
#define P_MIN_LENGTH_I25                    204
#define P_DM_RECT                           205
#define P_UPC_SUPPLEMENT_ENABLE_ALL         206
#define P_UPC_E_EXPANSION                   207
#define P_POLARITY_LINEAR                   208
#define P_POLARITY_DM                       209
#define P_POLARITY_QR                       210
#define P_POLARITY_AZ                       211
#define P_POLARITY_GC                       212             /* Not yet implemented */
#define P_POLARITY_HX                       213             /* Not yet implemented */
#define P_POLARITY_GM                       214
#define P_POLARITY_DC                       215
#define P_POLARITY_RESERVED3                216             /* Reserved for future */

#define P_MIRROR_DM                         217
#define P_MIRROR_QR                         218
#define P_MIRROR_AZ                         219
#define P_MIRROR_GC                         220
#define P_MIRROR_HX                         221             /* Not yet implemented */
#define P_MIRROR_GM                         222
#define P_MIRROR_DC                         223
#define P_MIRROR_RESERVED3                  224             /* Reserved for future */

#define P_DM_RECT_EXTENDED                  225

/* Properties - Extended Symbology Checksum Options */
#define P_CHECKSUM_C11                      239
#define P_CHECKSUM_MSI_PLE                  241
#define P_STRIPCHAR_MSI_PLE                 242
#define P_PHARMA_MIN_NUM_BARS               243
#define P_PHARMA_MAX_NUM_BARS               244
#define P_PHARMA_MIN_VALUE                  245
#define P_PHARMA_MAX_VALUE                  246
#define P_PHARMA_REV_DEC                    247
#define P_PHARMA_COLOR_BARS                 248
#define P_STRIPCHAR_C11                     249

/* Properties -Symbology Output Options */
#define P_UPCA_TO_EAN13                     250
#define P_EAN8_TO_EAN13                     251
#define P_BOOKLAND_TO_ISBN                  252
#define P_BOOKLAND_TO_ISSN                  253
#define P_STRIP_NUM_SYS_UPCA                254
#define P_STRIP_NUM_SYS_UPCE                255
#define P_STRIP_CHAR_UPCA                   256
#define P_STRIP_CHAR_UPCE                   257
#define P_STRIP_CHAR_EAN13                  258
#define P_STRIP_CHAR_EAN8                   259
#define P_STRIP_STARTSTOP_CBAR              260
#define P_SEND_STARTSTOP_C39                261
#define P_SEND_STARTSTOP_TRI                262
#define P_DM_FOCUS_QUALITY                  263
#define P_OUTPUT_AIM_SYMB_ID                264
#define P_SEND_CBAR_ABC                     265
#define P_SEND_CBAR_CX                      266
#define P_REVERSE_TRIOPTIC                  267
#define P_SEND_ECC_CODEWORDS                268
#define P_TELEPEN_OUTPUT_ASCII              269
#define P_SEND_UPC_AIM_MODIFIER             270
#define P_BC412_REV_DEC                     271
#define P_SEND_ROYAL_MAIL_CHECK_CHAR        272
#define P_STRIP_AUSTRALIA_POST_CHECK_CHARS  273
#define P_OUTPUT_VERIFONE                   274
#define P_OUTPUT_GILBARCO                   275
#define P_OUTPUT_NCR                        276
#define P_OUTPUT_WINCNIX                    277

#define P_SUPPLEMENT_2_DIGIT_UPCA           278
#define P_SUPPLEMENT_2_DIGIT_UPCE           279
#define P_SUPPLEMENT_2_DIGIT_EAN13          280
#define P_SUPPLEMENT_2_DIGIT_EAN8           281
#define P_SUPPLEMENT_5_DIGIT_UPCA           282
#define P_SUPPLEMENT_5_DIGIT_UPCE           283
#define P_SUPPLEMENT_5_DIGIT_EAN13          284
#define P_SUPPLEMENT_5_DIGIT_EAN8           285
#define P_SUPPLEMENT_ADD_SPACE_UPCA         286
#define P_SUPPLEMENT_ADD_SPACE_UPCE         287
#define P_SUPPLEMENT_ADD_SPACE_EAN13        288
#define P_SUPPLEMENT_ADD_SPACE_EAN8         289
#define P_SUPPLEMENT_REQUIRED_UPCA          290
#define P_SUPPLEMENT_REQUIRED_UPCE          291
#define P_SUPPLEMENT_REQUIRED_EAN13         292
#define P_SUPPLEMENT_REQUIRED_EAN8          293
#define P_UPC_E1_ENABLED                    294
#define P_STRIP_NUM_SYS_0_UPCA              295
#define P_REJECT_PARTIAL_I25                296
#define P_STRIP_JAPAN_POST_CHECK_CHARS      297
#define P_COMPOSITE_REQUIRED_UPC_EAN        298

/* Properties - Performance */
#define P_OPERATION_MODE                    300
#define P_STOP_DECODE                       301
#define P_AGC_ROI_LEFT                      302
#define P_AGC_ROI_TOP                       303
#define P_AGC_ROI_WIDTH                     304
#define P_AGC_ROI_HEIGHT                    305
#define P_BC_ROI_LEFT                       306
#define P_BC_ROI_TOP                        307
#define P_BC_ROI_WIDTH                      308
#define P_BC_ROI_HEIGHT                     309
#define P_TIME_LIMIT_DECODE                 310
#define P_SECURITY_LEVEL                    311
#define P_TIME_OUT_FACTOR                   312
#define P_TARGET_TOLERANCE_PERCENT          313
#define P_TARGET_LOCATION                   314         /* ((x << 16) + y) */
#define P_CODE_SEARCH_SPEED                 315
#define P_DPM_WINDOWING                     316
#define P_TIME_LIMIT_QUICK_DECODE           317
#define P_TIME_LIMIT_LOCATE                 318
#define P_CUSTOM_ROBUST_INDEX               319
#define P_TIME_LIMIT_CUSTOM_ROBUST          320

#define P_ENSURE_ROI                        350
#define P_BASIC_ETCH_DPM                    351
#define P_BASIC_DOTS_DPM                    352
#define P_CELL_IMAGE                        353
#define P_LOW_CONTRAST                      355
#define P_CODE_RESERVED_5                   356
#define P_CODE_RESERVED_6                   357
#define P_CODE_RESERVED_11                  358
#define P_DPM_DOTPEEN_DL                    359
#define P_DPM_DOTPEEN_LD                    360
#define P_DPM_LASER_CHEM_ETCH               361
#define P_DPM_DOTPEEN_AND_ETCH              362

#define P_DPM_SMALL_MIRRORED                365
#define P_QUICK_DECODE                      366
#define P_NO_CR8000_SPEEDUP                 367
#define P_NATIVE_DUAL_FIELD                 368
#define P_MULTI_ALIGNED                     369
#define P_BASIC_INKJET_DPM                  370
#define P_BASIC_HANDHELD_DPM                371

#define P_MULTICODE_SPEEDUP                 373         /* speed up multicode image decoding */

#define P_CUSTOM_PERFORMANCE                375
#define P_DETECTOR_DENSITY                  376

#define P_VERSION_STRING_LENGTH             398
#define P_VERSION_STRING                    399

/* Properties - Results */
#define P_RESULT_CORNERS                    400
#define P_RESULT_CENTER                     401
#define P_RESULT_SYMBOL_TYPE                402
#define P_RESULT_LENGTH                     403
#define P_RESULT_STRING                     404
#define P_RESULT_SYMBOL_MODIFIER            405
#define P_RESULT_LINKAGE                    406
#define P_RESULT_QUALITY                    407
#define P_RESULT_ECC_ERROR                  408
#define P_RESULT_ECC_ERASURE                409
#define P_RESULT_DELTA_TIME                 410
#define P_RESULT_TOTAL_TIME                 411
#define P_RESULT_SYMBOL_TYPE_EX             412
#define P_RESULT_LOCATE_TIME                413
#define P_RESULT_DECODE_TIME                414
#define P_CODE_RESERVED_7                   415
#define P_CODE_RESERVED_8                   416
#define P_CODE_RESERVED_9                   417
#define P_CODE_RESERVED_10                  418
#define P_RESULT_DECODE_OUTPUT_FORMAT       419
#define P_RESULT_STATUS                     420
#define P_RESULT_NUM_CODEWORDS              421
#define P_RESULT_CODEWORDS_BEFORE_ECC       422
#define P_RESULT_CODEWORDS_AFTER_ECC        423
#define P_RESULT_NUM_CODEWORDS_BLOCKS       424
#define P_RESULT_NUM_CODEWORDS_LONG_BLOCKS  425
#define P_RESULT_NUM_DATA_CODEWORDS         426
#define P_RESULT_NUM_EC_CODEWORDS           427
#define P_RESULT_STRUC_APPEND_POSITION      428
#define P_RESULT_STRUC_APPEND_TOTAL         429
#define P_RESULT_STRUC_APPEND_PARITY        430
#define P_RESULT_MISC_PROPERTY              431
#define P_RESULT_AGC_LIGHT_LEVEL            432
#define P_RESULT_SYMBOL_HEIGHT_WIDTH        433
#define P_RESULT_SYMBOL_ROWS_COLUMNS        434
#define P_RESULT_SYMB_POS_ID_STRING         435     /* Prefix string for P_OUTPUT_XYZ properties */
#define P_RESULT_SYMB_POS_ID_LENGTH         436
#define P_RESULT_FORMATTED_STRING           437     /* Prefix string + data (either original or parsed/formatted) */
#define P_RESULT_FORMATTED_LENGTH           438
#define P_RESULT_AGC_VALUE                  439
#define P_RESULT_LICENSE_UNLOCK_TIME        440

/* Properties - Optional Verification Results */
#define P_RESULT_GRADE_FINAL                470
#define P_RESULT_GRADE_REFERENCE_DECODE     471
#define P_RESULT_GRADE_CONTRAST             472
#define P_RESULT_SCORE_CONTRAST             473
#define P_RESULT_GRADE_MODULATION           474
#define P_RESULT_GRADE_REFLECTANCE_MARGIN   475
#define P_RESULT_GRADE_FIXED_PATTERN_DAMAGE 476
#define P_RESULT_GRADE_AXIAL_NONUNIFORMITY  477
#define P_RESULT_SCORE_AXIAL_NONUNIFORMITY  478
#define P_RESULT_GRADE_GRID_NONUNIFORMITY   479
#define P_RESULT_SCORE_GRID_NONUNIFORMITY   480
#define P_RESULT_GRADE_UNUSED_ERROR_CORRECT 481
#define P_RESULT_SCORE_UNUSED_ERROR_CORRECT 482
#define P_RESULT_APERTURE_SIZE_USED         483
#define P_RESULT_SCORE_MODULE_SIZE_PIXELS   484
#define P_RESULT_SCORE_PRINT_GROWTH_X       485
#define P_RESULT_SCORE_PRINT_GROWTH_Y       486

/* Properties - Optional Graphical Verification Results */
#define P_RESULT_GRAPH_MATRIX_SIZE          490
#define P_RESULT_GRAPH_MATRIX_ORIGINAL      491
#define P_RESULT_GRAPH_MATRIX_CORRECTED     492
#define P_RESULT_GRAPH_CODEWORD_MODULATION  493
#define P_RESULT_GRAPH_CODEWORD_MOD_INDEX   494
#define P_RESULT_GRAPH_MATRIX_COORDINATES   495

/* Properties - Optional Results (need to be enabled) */
#define P_RESULT_TARGETING_STATUS           498     /* CR4300 5MP mode: 0=ok, 1=tooBright, 2=tooDark */
#define P_RESULT_FOCUS_QUALITY              499

#define P_MIN_LENGTH_C39                    500
#define P_MIN_LENGTH_CBAR                   501
#define P_MIN_LENGTH_C128                   502
#define P_MIN_LENGTH_C93                    503

/* Properties - This item size (debugging only) */
#define P_THIS_SIZE                         504
/* Properties - Decimated image address */
#define P_DECI_IMAGE_ADDRESS                505
/* Properties - Convoluted image properties */
#define P_CONV_IMAGE_ADDRESS                506
#define P_CONV_IMAGE_TYPE                   507

#define P_ALLOW_HIGH_WIDE_NARROW_RATIO_CBAR 508
#define P_PICK_LIST_TYPE                    509

#define P_MIN_LENGTH_IATA25                 510
#define P_MIN_LENGTH_MSI                    511
#define P_ORIENTATION_BOUNDS                512     /* ((delta << 10) + angle) */
#define P_QUALITY_THRESHOLD                 513

/* Code 2 of 5 Checksum Options */
#define P_CHECKSUM_S25                      514
#define P_CHECKSUM_IATA25                   515
#define P_CHECKSUM_M25                      516
#define P_CHECKSUM_NEC25                    517
#define P_CHECKSUM_HK25                     518

#define P_MAX_LENGTH_I25                    519
#define P_MAX_LENGTH_C39                    520
#define P_MAX_LENGTH_CBAR                   521
#define P_MAX_LENGTH_C128                   522
#define P_MAX_LENGTH_C93                    523
#define P_MAX_LENGTH_IATA25                 524
#define P_MAX_LENGTH_MSI                    525

#define P_MINIMUM_BAR_HEIGHT                540
#define P_MAXIMUM_BAR_HEIGHT                541
#define P_MINIMUM_SIZE_DM                   542     /* barcode size in pixels */
#define P_MAXIMUM_SIZE_DM                   543     /* barcode size in pixels */
#define P_MIN_FIXED_PATTERN_QUALITY_DM      544
#define P_MIN_NUM_MODULE_DM                 545
#define P_MAX_NUM_MODULE_DM                 546
#define P_MAX_MODULE_SIZE_DM                547     /* Max module size is pixels */
#define P_MAX_LINE_FILTER_SIZE              548     /* Reject non-barcode lines in image: 0 - 32 pixels */

#define P_MIN_1X_SIZE_C39                   590
#define P_MIN_1X_SIZE_UPC                   591
#define P_MIN_1X_SIZE_C128                  592
#define P_MIN_1X_SIZE_CBAR                  593
#define P_MIN_1X_SIZE_I25                   594
#define P_MIN_1X_SIZE_MSI                   595

#define P_OUTPUT_NCR2                       600
#define P_SYMB_POS_POSITION                 601

#define P_1D_DECODE_SEGMENT                 603
#define P_RESULT_SEGMENT_TYPE               604
#define P_SEGMENT_MIN_CHAR                  605
#define P_MULTICODE_OUTPUT_ORDER            606
#define P_MULTICODE_OUTPUT_BY_PRIORITY      607
#define P_PRECISE_CORNER_QR                 608
#define P_DUPLICATE1D_GAP_FACTOR            609
#define P_CUSTOMER_UNIQUE                   610     /* Setting for optimizing unique customer application */
#define P_DEBLUR1D_NO_MISREAD               611
#define P_ENABLE_CUSTOM_DM_A                612
#define P_REJECT_UPCE_PARTIAL_EAN13         613
#define P_REDUCE_UPC_AGGRESSIVENESS         614     /* No longer needed for 23.1.1+. Kept for backward compatibility */
#define P_NO_FILTER_DECODE                  615
#define P_SET_1D_DPM                        616
#define P_SET_1D_ALTERNATE                  617

/* Values of properties */
#define V_SYMB_GC                           (1 << 0)
#define V_SYMB_DM                           (1 << 1)
#define V_SYMB_QR_2005                      (1 << 2)
#define V_SYMB_AZ                           (1 << 3)
#define V_SYMB_MC                           (1 << 4)
#define V_SYMB_PDF                          (1 << 5)
#define V_SYMB_MPDF                         (1 << 6)
#define V_SYMB_CCA                          (1 << 7)
#define V_SYMB_CCB                          (1 << 8)
#define V_SYMB_CCC                          (1 << 9)
#define V_SYMB_C39                          (1 << 10)
#define V_SYMB_I25                          (1 << 11)
#define V_SYMB_CBAR                         (1 << 12)
#define V_SYMB_C128                         (1 << 13)
#define V_SYMB_C93                          (1 << 14)
#define V_SYMB_UPCA                         (1 << 15)
#define V_SYMB_UPCE                         (1 << 16)
#define V_SYMB_EAN13                        (1 << 17)
#define V_SYMB_EAN8                         (1 << 18)
#define V_SYMB_DB_14                        (1 << 19)
#define V_SYMB_DB_14_STA                    (1 << 20)
#define V_SYMB_DB_LIM                       (1 << 21)
#define V_SYMB_DB_EXP                       (1 << 22)
#define V_SYMB_DB_EXP_STA                   (1 << 23)
#define V_SYMB_HX                           (1 << 24)
#define V_SYMB_QR_MICRO                     (1 << 25)
#define V_SYMB_QR_MODEL1                    (1 << 26)
#define V_SYMB_GM                           (1 << 27)
#define V_SYMB_DC                           (1 << 28)

#define V_SYMB_CUSTOM_NC                    (1 << 30)
#define V_SYMB_QR_CONFIG_CODE               (1 << 29)

/* Values of properties of symbologyEx */
#define V_SYMB_EXTENDED                     0
#define V_SYMB_C11                          (1 << 0)
#define V_SYMB_C32                          (1 << 1)
#define V_SYMB_PLE                          (1 << 2)
#define V_SYMB_MSI_PLE                      (1 << 3)
#define V_SYMB_TLP                          (1 << 4)
#define V_SYMB_TRI                          (1 << 5)
#define V_SYMB_PHA                          (1 << 6)
#define V_SYMB_M25                          (1 << 7)
#define V_SYMB_S25                          (1 << 8)        /* Standard 2of5 with 3 Start/Stop bars */
#define V_SYMB_C49                          (1 << 9)
#define V_SYMB_C16K                         (1 << 10)
#define V_SYMB_CBLK                         (1 << 11)
#define V_SYMB_POSTNET                      (1 << 12)
#define V_SYMB_PLANET                       (1 << 13)
#define V_SYMB_INTEL_MAIL                   (1 << 14)
#define V_SYMB_AUSTRA_POST                  (1 << 15)
#define V_SYMB_DUTCH_POST                   (1 << 16)
#define V_SYMB_JAPAN_POST                   (1 << 17)
#define V_SYMB_ROYAL_MAIL                   (1 << 18)
#define V_SYMB_UPU                          (1 << 19)
#define V_SYMB_KOREA_POST                   (1 << 20)
#define V_SYMB_HK25                         (1 << 21)
#define V_SYMB_NEC25                        (1 << 22)
#define V_SYMB_IATA25                       (1 << 23)        /* Standard 2of5 with 2 Start/Stop bars */
#define V_SYMB_CANADA_POST                  (1 << 24)
#define V_SYMB_BC412                        (1 << 25)
#define V_SYMB_TLC39                        (1 << 26)

#define V_SYMB_PRO1                         (1 << 31)

/* Values of miscellaneous results properties */
#define V_MISC_MIRROR_IMAGE                 (1 << 0)
#define V_MISC_LIGHT_ON_DARK                (1 << 1)

#define V_POLARITY_DARK_ON_LIGHT            1
#define V_POLARITY_LIGHT_ON_DARK            -1
#define V_POLARITY_EITHER                   0

/* True / False */
#define  V_FALSE                            0
#define  V_TRUE                             1

/* Parse/Format output */
#define P_FORMAT_OUTPUT_OPTION              800

#define P_FORMAT_OUTPUT_CONFIG_STRING_DLP   803             /* for Drives's License Parsing, string must be null terminated */
#define P_FORMAT_OUTPUT_CONFIG_STRING_DFT   804             /* for Data Formatting, string should be null terminated */
#define P_FORMAT_OUTPUT_CONFIG_STRING_SMR   805             /* for String Match/Replacing, string should be null terminated */
#define P_FORMAT_OUTPUT_CONFIG_STRING_GSP   806             /* for GS1 Parsing, string should be null terminated */
#define P_FORMAT_OUTPUT_CONFIG_STRING_UDI   807             /* for UDI, string should be null terminated */
#define P_FORMAT_OUTPUT_CONFIG_STRING_ISO   808             /* for ISO15434, string should be null terminated */
#define P_FORMAT_OUTPUT_CONFIG_STRING_ISO1  809             /* for ISO15343 & 15418, string should be null terminated */
#define P_FORMAT_OUTPUT_OPTION_FORMATTING   810
#define P_FORMAT_OUTPUT_OPTION_VALIDATION   811
#define P_FORMAT_OUTPUT_CONFIG_STRING_UID   812             /* for UID, string should be null terminated */

/* Error Codes */
#define ERR_INVALID_HANDLE                  900
#define ERR_INSUFFICIENT_MEMORY             901
#define ERR_INVALID_PROPERTY                902
#define ERR_INVALID_VALUE                   903
#define ERR_RESERVED_1                      904
#define ERR_NO_SYMBOLOGY_ENABLED            905
#define ERR_MULTICODE_UNSUPPORTED           906
#define ERR_RESERVED_2                      907
#define ERR_RESERVED_3                      908
#define ERR_UNLICENSED_PROPERTY             909
#define ERR_DEBUG_USB_KEY_NOT_FOUND         910
#define ERR_VERIFIER_UNSUPPORTED            911
#define ERR_LICENSE_COUNT_EXCEEDED          912
#define ERR_ROI_TOO_BIG_For_PreProcZoomROI  913

/* The decoder returns when the number of the specified barcodes are decoded, or the decoder is */
/* stopped by the caller in the progress callback, or the decoding time limit has been reached  */
#define DECODE_QUIT                         999
#define SEARCH_COMPLETED                    0

#if defined(CR8000)
/* Legacy method for PublicSectorParsing support */
#define FORMAT_OUTPUT_CONFIG_STRING_LEN     2000     /* up to 2000 characters, this matches with the length defined in decode.c */
#endif

/* Decoder Locating, Decoding, and Data Manipulation Status */
enum
{
    StatusNoBarcodeFound                    = 0,
    StatusFoundSection1D                    = 1, 
    StatusFoundStartStop1D                  = 2,
    StatusFoundStartStopPDF                 = 3,
    StatusFoundAztec                        = 4,
    StatusFoundDataMatrix                   = 5,
    StatusFoundQRCode                       = 6,
    StatusFoundMaxiCode                     = 7,
    StatusFoundHanXin                       = 8,
    StatusFoundGoCode                       = 9,
    StatusFoundGridMatrix                   = 10,
    StatusFoundCellphone                    = 20,     /* 20-29 cellphone */
    StatusFoundMotion                       = 30,     /* 30-39 motion */
    StatusFailureDecode1D_PDF               = 60,
    StatusFailureDecodeAztec                = 61,
    StatusFailureDecodeDM                   = 62,
    StatusFailureDecodeQR                   = 63,
    StatusFailureDecodeMicroQR              = 64,
    StatusDecodedButRejected                = 80,
    StatusDecodedButRejectedByCount         = 81,
    StatusDecodeSuccess                     = 100,
    StatusConfigurationCode                 = 101,
    StatusDecodeSuccessHigh                 = 199,
    StatusMSSucessActionNoData              = 200,  /* Match String with success action as no data */
    StatusCfgStrStrtoNum                    = 201,  /* ERROR:Config_String-A_String_To_A_Number */
    StatusCfgStrSubStrOrLenZero             = 202,  /* ERROR:Config_String-Sub_String Or Data_Length-Zero */
    StatusCfgStrNotExist                    = 203,  /* ERROR:Config_String-Do_Not_Exist */
    StatusCfgStrTooShort                    = 204,  /* ERROR:Config_String-Too_Short */
    StatusCfgStrNumSegment                  = 205,  /* ERROR:Config_String-Num_Segment */
    StatusCfgStrSegmentLen                  = 206,  /* ERROR:Config_String-Segment_Length */
    StatusCfgStrResActFmt                   = 207,  /* ERROR:Config_String-Result_Action_Format */
    StatusCfgStrMatchStringWildcard         = 208,  /* ERROR:Config_String-Match_String_Wildcard */
    StatusCfgStrSearchLenTooShort           = 209,  /* ERROR:Config_String-Search_Length_Too_Short */
    StatusCfgStrMatchOver500Loc             = 210,  /* ERROR:Config_String-Match_Over_500_Locations (Maximum Allowed 500) */
    StatusCfgStrAiListNoEnd                 = 211,  /* ERROR:Config_String-AI_List_Without_; */
    StatusCfgStrResActOutRange              = 212,  /* ERROR:Config_String-Result_Actions_Out_Of_Range */
    StatusCfgStrSeparatorTooLong            = 213,  /* ERROR:Config_String-Separator_Too_Long */
    StatusCfgStrDateFmt                     = 214,  /* ERROR:Config_String-Date_Format */
    StatusCfgStrStartNonEscapeChar          = 215,  /* ERROR:Config_String-Start_Non_Escape_Char */
    StatusCfgStrNoValidAi                   = 216,  /* ERROR:Config_String-No_Valid_AI */
    StatusMSSymbologyNotMatch               = 217,  /* ERROR:Condition-Symbology_Not_Match */
    StatusMSDataLengthNotMatch              = 218,  /* ERROR:Condition-Data_Length_Not_Match */
    StatusMSDataNotMatch                    = 219,  /* ERROR:Data_Not_Match */
    StatusGS1ProgramAiIssues                = 220,  /* ERROR:GS1-Programming issues without ',' */
    StatusGS1NotDataCarrier                 = 221,  /* ERROR:GS1-Not_Data_Carrier */
    StatusGS1NoValidAI                      = 222,  /* ERROR:GS1_Data-No_Valid_AI */
    StatusGS1PartialValidAI                 = 223,  /* ERROR:GS1_Data-Partial_Valid_AI */
    StatusGS1DataAiNotMatchWithConfigAi     = 224,  /* ERROR:GS1-Data_AI_Not_Match_With_Config_AI */
    StatusUdiNoDataCarrier                  = 225,  /* ERROR:UDI-Not_A_UDI_Data_Carrier */
    StatusUdiNo25s25pHibccIsoFmt            = 226,  /* ERROR:No_25S_25P_in_HIBCC_UDI_ISO_Format */
    StatusUdiUnknIacInIsoFmt                = 227,  /* ERROR:Unknown_Issuing_Agency_Code_in_ISO_Format */
    StatusUdiSupplierLicInIsoFmt            = 228,  /* ERROR:Supplier_LIC_in_ISO_Format */
    StatusUdiSnTooLongInIsoFmt              = 229,  /* ERROR:Serial_Number_Too_Long_in_ISO_Format */
    StatusUdiUnitOfMeasure                  = 230,  /* ERROR:Unit_of_Measure */
    StatusUdiJulianDateFmt                  = 231,  /* ERROR:Julian_Date_Format */
    StatusUdiDollarSignCases                = 232,  /* ERROR:$_Cases */
    StatusUdi2ndDataFmtDDPlus               = 233,  /* ERROR:Secondary_Data_formats_$$+ */
    StatusUdi2ndDataFmtDD                   = 234,  /* ERROR:Secondary_Data_formats_$$ */
    StatusUdi14dDate                        = 235,  /* ERROR:14D_Date */
    StatusUdi16dDate                        = 236,  /* ERROR:16D_Date */
    StatusUdiSuppDiNoS14d16d                = 237,  /* ERROR:Supplemental_DI_Not_S_14D_16D */
    StatusUdiNonHibcc                       = 238,  /* ERROR:Non_HIBCC_UDI */
    StatusUdiGs1WrongGtinLen                = 239,  /* ERROR:GS1_UDI_Wrong_GTIN_Length */
    StatusUdiGs1WrongDataInDatabar          = 240,  /* ERROR:GS1_UDI_Wrong_Data_in_GS1_DataBar */
    StatusUdiGs1Dm128No01Begin              = 241,  /* ERROR:GS1-128/DataMatrix_Without_01_at_Beginning */
    StatusUdiGs1Dm128Ai10TooLong            = 242,  /* ERROR:GS1-128/DataMatrix_AI10_Too_Long */
    StatusUdiGs1Dm128Ai21TooLong            = 243,  /* ERROR:GS1-128/DataMatrix_AI21_Too_Long */
    StatusUdiGs1Dm128No10_17_21_11          = 244,  /* ERROR:GS1-128/DataMatrix_Non_10_17_21_11_AI */
    StatusUdiInvIccbbaCompoundNum           = 245,  /* ERROR:INVALID_ICCBBA_COMPOUND_NUMBER */
    StatusUdiIccbbaPpicLenTooShort          = 246,  /* ERROR:ICCBBA_PPIC_LENGTH_TOO_SHORT */
    StatusUdiIccbbaPpic1_5                  = 247,  /* ERROR:ICCBBA_PPIC_1_5 */
    StatusUdiIccbbaPpic6_16                 = 248,  /* ERROR:ICCBBA_PPIC_6_16 */
    StatusUdiIccbbaSnLenTooShort            = 249,  /* ERROR:ICCBBA_SERIAL_NUMBER_LENGTH_TOO_SHORT */
    StatusUdiIccbbaSn                       = 250,  /* ERROR:ICCBBA_SERIAL_NUMBER */
    StatusUdiIccbbaSn000000                 = 251,  /* ERROR:ICCBBA_SERIAL_NUMBER_000000 */
    StatusUdiIccbbaExpDateTooShort          = 252,  /* ERROR:ICCBBA_EXPIRATION_DATE_LENGTH_TOO_SHORT */
    StatusUdiIccbbaExpDate                  = 253,  /* ERROR:ICCBBA_EXPIRATION_DATE */
    StatusUdiIccbbaMfgDateTooShort          = 254,  /* ERROR:ICCBBA_MANUFACTURING_DATE_LENGTH_TOO_SHORT */
    StatusUdiIccbbaMfgDate                  = 255,  /* ERROR:ICCBBA_MANUFACTURING_DATE */
    StatusUdiIccbbaDonationIdCode           = 256,  /* ERROR:ICCBBA_DONATION_ID_CODE */
    StatusUdiIccbbaLotBatTooShort           = 257,  /* ERROR:ICCBBA_LOT_BATCH_LENGTH_TOO_SHORT */
    StatusUdiIccbbaLotBat                   = 258,  /* ERROR:ICCBBA_LOT_BATCH */
    StatusUdiIccbbaPossibleTooLong          = 259,  /* ERROR:ICCBBA_POSSIBLE_LENGTH_TOO_LONG */
    StatusUdiIccbbaNoDi                     = 260,  /* ERROR:NO_ICCBBA_DI */
    StatusIso15434MsgTrailer                = 261,  /* ERROR:ISO15434_Message_Trailer */
    StatusIso15434FmtTrailer                = 262,  /* ERROR:ISO15434_Format_Trailer */
    StatusIso15434DataElmSeparator          = 263,  /* ERROR:ISO15434_Data_Element_Separator */
    StatusIso15434FmtId00_99                = 264,  /* ERROR:ISO15434_Format_Indicator_Only_Support_BETWEEN_00_AND_99 */
    StatusIso15434FmtIdWrong                = 265,  /* ERROR:ISO15434_Format_Indicator_Wrong */
    StatusIso15434MsgHeader                 = 266,  /* ERROR:ISO15434_Message_Header */
    StatusIsoDiNameWrong                    = 267,  /* ERROR:DI_Name_Wrong */
    StatusIsoAiNameWrong                    = 268,  /* ERROR:AI_Name_Wrong */
    StatusIsoDiNotSupported                 = 269,  /* ERROR:DI_Not_Supported */
    StatusIsoNoRsAtEnd                      = 270,  /* ERROR:No_RS_at_End_in_ISO_Format */
    StatusIsoDataTooLong                    = 271,  /* ERROR:Data_Too_long_in_ISO_Format */
    StatusIsoAiCharSet82                    = 272,  /* ERROR:Data_AI_CharcterSet_82 */
    StatusIsoDataNonAlphaNumericP           = 273,  /* ERROR:Data_Alphanumeric_Non_0-9_A-Z_#_-_/ */
    StatusIsoDataNonAlphaNumeric            = 274,  /* ERROR:Data_Alphanumeric_Non_0-9_A-Z */
    StatusIsoDataNonNumeric                 = 275,  /* ERROR:Data_Non_numeric_0-9 */
    StatusIsoVaryLenWithoutGs               = 276,  /* ERROR:Data_Vary_Length_Without_GS */
    StatusConditionCharacters               = 277,  /* ERROR:Condition_Characters; condition sym & len have been defined before */
    StatusCfgStrConditionCharacters         = 278,  /* ERROR:Config_String_Condition_Characters */
    StatusDataFormatInfoItemNoData          = 279,  /* ERROR:Data_Format_Info_Item_No_Data */
    StatusMSSymbologyDefineOverflow         = 280,  /* ERROR:Config_String_Symbology_Define_Overflow */
    StatusCfgStrSeparator                   = 281,  /* ERROR:Config_String_Separator_error */
    StatusUdiSucessOnlyDiButNoDiData        = 282,  /* UDI_Success_Output_DI_Only_But_No_DI_Data */
    StatusUdiSucessOnlyPiButNoPiData        = 283,  /* UDI_Success_Output_PI_Only_But_No_PI_Data */
    StatusGS1OtherError                     = 284,  /* ERROR:GS1 other error (use ^B in configuration string to find out) */
    StatusGS1AI01CheckDigit                 = 285,   /* ERROR:GS1_Data-AI01_Check_Digit */
    StatusUidWithout17v12v18v7v3v           = 290,  /* ERROR:UID_Without_17V/12V/18V/7V/3V */ /* UID stars at 290*/
    StatusUid17vWithout1p1t                 = 291,  /* ERROR:UID_17V_Without_1P/1T */
    StatusUid17v1p1tWithoutS                = 292,  /* ERROR:UID_17V_1P/1T_Without_S */
    StatusUid12vWithout1p1t                 = 293,  /* ERROR:UID_12V_Without_1P/1T */
    StatusUid12v1p1tWithoutS                = 294,  /* ERROR:UID_12V_1P/1T_Without_S */
    StatusUid18vWithout1p1t                 = 295,  /* ERROR:UID_18V_Without_1P/1T */
    StatusUid18v1p1tWithoutS                = 296,  /* ERROR:UID_18V_1P/1T_Without_S */
    StatusUid7lWithout1p1t                  = 297,  /* ERROR:UID_7L_Without_1P/1T */
    StatusUid7l1p1tWithoutS                 = 298,  /* ERROR:UID_7L_1P/1T_Without_S */
    StatusUid3vWithout1p1t                  = 299,  /* ERROR:UID_3V_Without_1P/1T */
    StatusUid3v1p1tWithoutS                 = 300,  /* ERROR:UID_3V_1P/1T_Without_S */
    StatusUidWithoutMfrSplCagDunEuc         = 301,  /* ERROR:UID_Without_MFR/SPL/CAG/DUN/EUC */
    StatusUidWithoutPnoLtn                  = 302,  /* ERROR:UID_Without_PNO/LTN */
    StatusUidLtnWithoutSeqBill              = 303,  /* ERROR:UID_LTN_Without_SEQ/BILL */
    StatusUidLtnBillWithoutSeq              = 304,  /* ERROR:UID_LTN_BILL_Without_SEQ */
    StatusUidPnoWithoutSeqLot               = 305,  /* ERROR:UID_PNO_Without_SEQ/LOT */
    StatusUidPnoLotWithoutSeq               = 306,  /* ERROR:UID_PNO_LOT_Without_SEQ */
    StatusUidAiWithout8002_4                = 307,  /* ERROR:UID_AI_Without_8002/8003/8004 */
    StatusUidWrongSymbology                 = 308   /* ERROR:UID_Wrong_Symbology */
};

/* GS1 Composite linkage types */
enum LinkageType
{
    LinkageNone  =   0,
    LinkageCCAB  =   1,
    LinkageCCC   =   2,
};

enum PharmacodeSetting
{
    MinPharNumBars  =   3,
    MaxPharNumBars  =   16,
    MinPharValue    =   7,
    MaxPharValue    =   131070
};

#if defined(CR8000)
/* For legacy support */
enum PublicSectorBehavior
{
    PubSecParsing           =   0x01,   /* bit: on - parse; off - do not parse; */
    PubSecOutputFormat      =   0x02,   /* bit: on - JSON format; off - parsed output; */
    PubSecSimpleAgeVerify   =   0x08,   /* bit: on - enable Simple Age Verification; PubSecOutputFormat has no effect on this mode */
    PubSecSuccessAndRaw     =   0x10,   /* bit: on - parsed data + raw data; format: [4 chars for parsed data length][parsed data][raw data] */
    DataFormatting          =   0x100,  /* do data formatting, to be deprecated */
    MatchString             =   0x200,  /* do match string */
    GS1Validation           =   0x300,  /* do GS1 validation */
    UDIValidation           =   0x400,  /* do UDI validation */
    ISO15434Validation      =   0x500,  /* do ISO/IEC 15434 validation */
    ISO15434_15418Validation=   0x600,  /* do ISO/IEC 15434 & 15418 validation */
    UIDValidation           =   0x700,  /* do UID validation */
    DataFormattingAfterDV   =   0x1000  /* values (0x1000 + one of above value) will do one of above feature then Data Formatting */
                                        /* values 0x1100 & 0x1000 are the same effect as 0x100 to do one Data Formatting operation */
};
#endif

enum DataValidationType
{
    ValidationDisabled,
    ValidationPubSecParsing,            /* Driver license & Military ID parsing*/
    ValidationPubSecJSON,               /* DL parsing with JSON output */
    ValidationPubSecSimpleAgeVerify,    /* Simple Age Verification (Code internal use) */
    ValidationMatchString,              /* match string validation */
    ValidationGS1,                      /* GS1 parsing/validation */
    ValidationUDI,                      /* UDI parsing/validation */
    ValidationISO15434,                 /* ISO/IEC 15434 validation */
    ValidationISO15434_15418,           /* ISO/IEC 15434 & 15418 validation */
    ValidationUID                       /* UID parsing/validation */
};

enum ImageAnalysisType
{
    AgcBrightnessOnly,                  /* for decoder DPM AGC */
    AgcBrightnessQuality,               /* for decoder DPM AGC */
    RegionDetectBarcodeEdge1D,
    RegionDetectBarcodeFFT,
    RegionDetectBarcodeEdge1DFFT,
    RegionDetectCellphone,
    AgcCompoundGainInitial,             /* for decoder AGC */
    AgcCompoundGain,                    /* for decoder AGC */
    RegionDetectSpecular,               /* for cellphone and other specular types detection */
    AgcCurveRoi                         /* for curved surface AGC ROI */
};

enum DecodeSecurityLevel
{
    SecurityLevel0,                     /* Default, most agressive decoding */
    SecurityLevel1,                     /* Reduced aggressiveness for poor quality 1D barcode */
    SecurityLevel2,                     /* Lowest aggressiveness to avoid misdecode of poor quality 1D barcode */
    SecurityLevel3,                     /* Conditions signal quality for CR4300 mode */
    SecurityLevel11 = 11,               /* Reduced aggressiveness for low resolution 1D barcode */
    SecurityLevel12,                    /* Lowest aggressiveness to avoid misdecode of low resolution 1D barcode */
    SecurityLevel61 = 61                /* No longer needed in 23.1.1+. Kept for backward compatibility */
};

enum CodeSearchSpeed
{
    CodeSearchSpeed0,                   /* Default, can find low contrast barcode */         
    CodeSearchSpeed1,                   /* Faster, less damage tolerant, can find low contrast barcode */
    CodeSearchSpeed2,                   /* Faster than above, even less damage tolerance, find medium contrast barcode */
    CodeSearchSpeed3                    /* Faster than above, same damage tolerance, find high contrast barcode only */
};

enum ImageType
{
    Image8BitGrayScale,                 /* Default, 8-bit gray scale */
    ImageBGRA_CFLEX_B,                  /* BGRA format for internel use */
    ImageBGRA_CFLEX_G,                  /* BGRA format for internel use */
    ImageBGRA_Standard,                 /* BGRA format for internel use */
    ImageMJPG                           /* Jpeg compressed image */
};

enum TargetingStatus
{
    TargetingBrightnessOK,
    TargetingTooBright,
    TargetingTooDark,
    TargetingMediumDark,
    TargetingPitchDark
};

/* For CT8200 image processor */
enum PreProcessType
{
    PreProcNone,                        /* disabled */
    PreProcLowPass1,                    /* type 1 lowpass filtering used for CustomShortRange, P_HIGH_RES_SINGLE_LENS */
    PreProcLowPass2,                    /* type 2 lowpass filtering used for CustomShortRange, P_HIGH_RES_SINGLE_LENS */
    PreProcEnhanceDpmImage,             /* enhance dot peen or laser images for CR6000 */
    PreProcEnhanceCurvedDotPeen,        /* enhanced curved dot peen image for CR6000 */       
    PreProcDeblur1dMethod1,             /* Deblur blurry 1D images captured from mobile phone */
    PreProcDeblur1dMethod2,             /* Deblur blurry 1D images captured from imaging device */
    PreProcZoomROI,                     /* Replaced P_ZOOM_ROI */
    PreProcDecimate,                    /* Replaces P_CUSTOM_DECIMATE */
    PreProcErodeDark,
    PreProcDilatLight,
    preProcLowPass,
    PreProcEnhanceContrast              /* Replaces P_ENHANCE_CONTRAST */
};

enum SymbPosIdPosition
{
    SymbPosIdPosition0,                     /* at the beignning of the output message */
    SymbPosIdPosition1,                     /* after prefix but before the decoded data */
    SymbPosIdPosition2                      /* any position configured by data formatting*/
};

enum VerificationType
{
    VerificationNone,                       /* No verification */
    VerificationISO15415,                   /* ISO/IEC 15415 */
    VerificationAIMDPM                      /* ISO/IEC TR 29158 */
};

enum CustomPerformance
{
    CustomPerfNone,
    CustomShortRange,                       /* was set via P_HIGH_RES_SINGLE_LENS */
    CustomRobust,                           /* was set via P_CUSTOM_ROBUST */
    CustomExtendedRange2D,                  /* extended range for 2D decoding */
    Reserved
};

typedef struct CRPOINT
{
    int     x;
    int     y;
} CRPOINT;

typedef struct DECODE_RESULTS
{
    unsigned char *data;
    int     dataLength;
    int     symbolType;
    int     symbolTypeEx;
    int     symbolModifier;
    int     numEccError;
    int     numEccErasure;
    int     quality;
    int     focusQuality;
    int     linkage;
    int     status;
    int     deltaTime;
    int     totalTime;
    CRPOINT bound[4];
    CRPOINT center;
    int     strucAppendPosition;
    int     strucAppendTotal;
    int     strucAppendParity;
    int     miscProperty;
    int     heightWidth;
    int     numRowsColumns;
    int     numCodewords;
    int     numCwBlocks;
    int     numCwLongBlocks;
    int     numDataCw;
    int     numEcCw;
    unsigned char *eccBefore;
    unsigned char *eccAfter;
    int     reserved[10];
} DecodeResults;

#if defined (CORTEXDECODER_DLL)
#define _STDCALL __stdcall
#else
#define _STDCALL
#endif

typedef int (_STDCALL *CRDCB)(int);
typedef int (_STDCALL *CRDCBEX)(int, void *);

#define DLLDEF(x)   extern x _STDCALL

#if defined (__cplusplus)
extern "C" {
#endif

/* Function Prototypes */
DLLDEF(int) CRD_Create(void);
DLLDEF(int) CRD_Decode(int handle);
DLLDEF(int) CRD_Destroy(int handle);
DLLDEF(int) CRD_Get(int handle, int property, void *value);
DLLDEF(int) CRD_Set(int handle, int property, void *value);
DLLDEF(int) CRD_SetCB(int handle, int property, CRDCB value);
DLLDEF(int) CRD_SetInt(int handle, int property, int value);

DLLDEF(int) CRD_InitMulti(int handle, unsigned char* pDataBuf, DecodeResults* pDecodeResults,
                int dataBufSize, int symbolBufSize);
DLLDEF(int) CRD_DecodeMulti(int handle, int *progressCount, int numSymbols);
DLLDEF(int) CRD_InitVerifier(int handle, int maxImageWidth, int maxImageHeight, int maxBarcodeRows, int maxBarcodeCols);
DLLDEF(int) CRD_FreeVerifier(int handle);
DLLDEF(int) CRD_InitMultiEx(int handle, int maxNumSymbols);
DLLDEF(int) CRD_DecodeMultiEx(int handle, int numSymbols);
DLLDEF(int) CRD_GetIntMulti(int handle, int property, int index);
DLLDEF(int) CRD_GetBytesMulti(int handle, int property, int length, int index, void *value);
DLLDEF(int) CRD_GetIntsMulti(int handle, int property, int length, int index, void *value);

#if defined (__cplusplus)
}
#endif

/* Deprecated properties*/
#define SUPPORT_DEPRECATED_PROPERTIES
#ifdef SUPPORT_DEPRECATED_PROPERTIES

#define P_ENABLE_QR                         102         /* replaced by P_ENABLE_QR_2005 */
#define P_ENABLE_ALL_1D                     124         /* replaced by P_ENABLE_ALL_COMMON1D */
#define P_ENABLE_ALL                        126         /* replaced by P_ENABLE_ALL_2D_COMMON1D */
#define P_I25_LENGTH                        204         /* replaced by P_MIN_LENGTH_I25 */
#define P_UPC_SUPPLEMENT                    206         /* replaced by P_UPC_SUPPLEMENT_ENABLE_ALL*/
#define P_CHECKSUM_C25                      240         /* replaced by P_CHECKSUM_S25, P_CHECKSUM_IATA25, etc. */
#define P_SEND_AIM_SYMB_ID                  264         /* replaced by P_OUTPUT_AIM_SYMB_ID */
#define P_DECODE_TIME_LIMIT                 310         /* replaced by P_TIME_LIMIT_DECODE */
#define P_DPM_TIMING_FACTOR                 312         /* replaced by P_TIME_OUT_FACTOR */ */
#define P_MULTICODE_SPEED                   315         /* replaced by P_CODE_SEARCH_SPEED */
#define P_CUSTOM_ROBUST                     354         /* replaced by P_CUSTOM_PERFORMANCE CustomRobust */
#define P_DPM_DOTPEEN2_LD                   362         /* replaced by P_DPM_DOTPEEN_AND_ETCH */ 
#define P_ZOOM_ROI                          363         /* replaced by P_PREPROCESS_TYPE PreProcZoomROI */
#define P_ENHANCE_CONTRAST                  364         /* replaced by P_PREPROCESS_TYPE PreProcEnhanceContrast */
#define P_HIGH_RES_SINGLE_LENS              372         /* replaced by P_CUSTOM_PERFORMANCE CustomShortRange */
#define P_CUSTOM_DECIMATE                   374         /* replaced by P_PREPROCESS_TYPE PreProcDecimate */
#define P_1D_DEBLUR_FILTER                  602         /* replaced by P_PREPROCESS_TYPE PreProcDeblur1dMethod1/2 */
#define P_REDUCE_UPC_AGGRESSIVENESS         614         /* No longer needed for 23.1.1+. Kept for backward compatibility */
#define P_FORMAT_OUTPUT_CONFIG_STRING_LEN   801         /* replaced by P_FORMAT_OUTPUT_CONFIG_STRING_DLP etc. */
#define P_FORMAT_OUTPUT_CONFIG_STRING       802         /* replaced by P_FORMAT_OUTPUT_CONFIG_STRING_DLP etc. */

#define V_SYMB_QR                           (1 << 2)    /* replaced by V_SYMB_QR_2005 */
#define V_SYMB_MQR                          (1 << 25)   /* replaced by V_SYMB_QR_MICRO */

enum DeblurFilter                                       /* replaced with PreProcessType */
{
    DeblurNone,
    Deblur1dMethod1,
    Deblur1dMethod2
};
#endif /* SUPPORT_DEPRECATED_PROPERTIES */

#endif
