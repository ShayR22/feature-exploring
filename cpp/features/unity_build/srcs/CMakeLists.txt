
cmake_minimum_required(VERSION 3.22)
project(custom_classes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2 -ftime-trace")

set(SRCS
    custom_class_84.cpp custom_class_6.cpp custom_class_8.cpp custom_class_1.cpp custom_class_82.cpp custom_class_60.cpp custom_class_28.cpp custom_class_71.cpp custom_class_72.cpp custom_class_53.cpp custom_class_50.cpp custom_class_0.cpp custom_class_79.cpp custom_class_12.cpp custom_class_7.cpp custom_class_33.cpp custom_class_30.cpp custom_class_64.cpp custom_class_73.cpp custom_class_49.cpp custom_class_88.cpp custom_class_57.cpp custom_class_61.cpp custom_class_44.cpp custom_class_32.cpp custom_class_63.cpp custom_class_76.cpp custom_class_52.cpp custom_class_56.cpp custom_class_24.cpp custom_class_58.cpp custom_class_51.cpp custom_class_100.cpp custom_class_42.cpp custom_class_5.cpp custom_class_77.cpp custom_class_14.cpp custom_class_75.cpp custom_class_22.cpp custom_class_37.cpp custom_class_98.cpp custom_class_93.cpp custom_class_54.cpp custom_class_78.cpp custom_class_31.cpp custom_class_96.cpp custom_class_13.cpp custom_class_18.cpp custom_class_26.cpp custom_class_25.cpp custom_class_92.cpp custom_class_70.cpp custom_class_17.cpp custom_class_55.cpp custom_class_35.cpp custom_class_59.cpp custom_class_19.cpp custom_class_34.cpp custom_class_99.cpp custom_class_11.cpp custom_class_45.cpp custom_class_3.cpp custom_class_80.cpp custom_class_4.cpp custom_class_15.cpp custom_class_38.cpp custom_class_48.cpp custom_class_86.cpp custom_class_66.cpp custom_class_27.cpp custom_class_40.cpp custom_class_90.cpp custom_class_2.cpp custom_class_68.cpp custom_class_97.cpp custom_class_16.cpp custom_class_41.cpp custom_class_62.cpp custom_class_36.cpp custom_class_89.cpp custom_class_29.cpp custom_class_23.cpp custom_class_47.cpp custom_class_46.cpp custom_class_87.cpp custom_class_81.cpp custom_class_20.cpp custom_class_94.cpp custom_class_74.cpp custom_class_39.cpp custom_class_9.cpp custom_class_43.cpp custom_class_69.cpp custom_class_91.cpp custom_class_83.cpp custom_class_10.cpp custom_class_21.cpp custom_class_95.cpp custom_class_67.cpp custom_class_85.cpp custom_class_65.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})

target_precompile_headers(${PROJECT_NAME} PUBLIC
    <algorithm>
    <thread>
)

#set_target_properties(${PROJECT_NAME} PROPERTIES
#    UNITY_BUILD TRUE
#    UNITY_BUILD_MODE BATCH
#    UNITY_BUILD_BATCH_SIZE 10
#)


# set_target_properties(${PROJECT_NAME} PROPERTIES
#     UNITY_BUILD_MODE GROUP
# )
# set_source_files_properties(custom_class_0.cpp custom_class_1.cpp custom_class_2.cpp custom_class_3.cpp custom_class_4.cpp custom_class_5.cpp custom_class_6.cpp custom_class_7.cpp custom_class_8.cpp custom_class_9.cpp
#     PROPERTIES UNITY_GROUP "bucket0"
# )
# set_source_files_properties(custom_class_10.cpp custom_class_11.cpp custom_class_12.cpp custom_class_13.cpp custom_class_14.cpp custom_class_15.cpp custom_class_16.cpp custom_class_17.cpp custom_class_18.cpp custom_class_19.cpp
#     PROPERTIES UNITY_GROUP "bucket1"
# )
# set_source_files_properties(custom_class_20.cpp custom_class_21.cpp custom_class_22.cpp custom_class_23.cpp custom_class_24.cpp custom_class_25.cpp custom_class_26.cpp custom_class_27.cpp custom_class_28.cpp custom_class_29.cpp
#     PROPERTIES UNITY_GROUP "bucket2"
# )
# set_source_files_properties(custom_class_30.cpp custom_class_31.cpp custom_class_32.cpp custom_class_33.cpp custom_class_34.cpp custom_class_35.cpp custom_class_36.cpp custom_class_37.cpp custom_class_38.cpp custom_class_39.cpp
#     PROPERTIES UNITY_GROUP "bucket3"
# )
# set_source_files_properties(custom_class_40.cpp custom_class_41.cpp custom_class_42.cpp custom_class_43.cpp custom_class_44.cpp custom_class_45.cpp custom_class_46.cpp custom_class_47.cpp custom_class_48.cpp custom_class_49.cpp
#     PROPERTIES UNITY_GROUP "bucket4"
# )
# set_source_files_properties(custom_class_50.cpp custom_class_51.cpp custom_class_52.cpp custom_class_53.cpp custom_class_54.cpp custom_class_55.cpp custom_class_56.cpp custom_class_57.cpp custom_class_58.cpp custom_class_59.cpp
#     PROPERTIES UNITY_GROUP "bucket5"
# )
# set_source_files_properties(custom_class_60.cpp custom_class_61.cpp custom_class_62.cpp custom_class_63.cpp custom_class_64.cpp custom_class_65.cpp custom_class_66.cpp custom_class_67.cpp custom_class_68.cpp custom_class_69.cpp
#     PROPERTIES UNITY_GROUP "bucket6"
# )
# set_source_files_properties(custom_class_70.cpp custom_class_71.cpp custom_class_72.cpp custom_class_73.cpp custom_class_74.cpp custom_class_75.cpp custom_class_76.cpp custom_class_77.cpp custom_class_78.cpp custom_class_79.cpp
#     PROPERTIES UNITY_GROUP "bucket7"
# )
# set_source_files_properties(custom_class_80.cpp custom_class_81.cpp custom_class_82.cpp custom_class_83.cpp custom_class_84.cpp custom_class_85.cpp custom_class_86.cpp custom_class_87.cpp custom_class_88.cpp custom_class_89.cpp
#     PROPERTIES UNITY_GROUP "bucket8"
# )
# set_source_files_properties(custom_class_90.cpp custom_class_91.cpp custom_class_92.cpp custom_class_93.cpp custom_class_94.cpp custom_class_95.cpp custom_class_96.cpp custom_class_97.cpp custom_class_98.cpp custom_class_99.cpp
#     PROPERTIES UNITY_GROUP "bucket9"
# )
