add_library(
    algorithms INTERFACE
    Heap.hpp
    HeapSort.hpp
    Iterators.hpp
    MergeSort.hpp
    QuickSort.hpp
    Utils.hpp
  )

target_include_directories(
    algorithms
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
  )

add_library(phase::algorithms ALIAS algorithms)
