add_executable(
    WaveTest
    SineWaveTest.cpp
    ZonesTest.cpp
  )

target_link_libraries(
    WaveTest
    PRIVATE
        GTest::GTest
        GTest::Main
  )

gtest_discover_tests(WaveTest)
