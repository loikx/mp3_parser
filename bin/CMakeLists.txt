add_executable(lab7 main.cpp ../lib/MP3parser/mp3Parser.cpp ../lib/MP3parser/mp3Parser.h)

target_link_libraries(${PROJECT_NAME} PRIVATE Utils)
target_link_libraries(${PROJECT_NAME} PRIVATE Header)
target_link_libraries(${PROJECT_NAME} PRIVATE Builder)
target_link_libraries(${PROJECT_NAME} PRIVATE Frame)
target_include_directories(lab7 PRIVATE ${PROJECT_SOURCE_DIR})