# CMake generated Testfile for 
# Source directory: /home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc
# Build directory: /home/ratty/Downloads/pocketsphinx-main/examples/test/unit/test_alloc
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_ckd_alloc "/home/ratty/Downloads/pocketsphinx-main/examples/test_ckd_alloc")
set_tests_properties(test_ckd_alloc PROPERTIES  _BACKTRACE_TRIPLES "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;19;add_test;/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;0;")
add_test(test_listelem_alloc "/home/ratty/Downloads/pocketsphinx-main/examples/test_listelem_alloc")
set_tests_properties(test_listelem_alloc PROPERTIES  _BACKTRACE_TRIPLES "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;20;add_test;/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;0;")
add_test(test_ckd_alloc_catch "/home/ratty/Downloads/pocketsphinx-main/examples/test_ckd_alloc_catch")
set_tests_properties(test_ckd_alloc_catch PROPERTIES  _BACKTRACE_TRIPLES "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;21;add_test;/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;0;")
add_test(test_ckd_alloc_fail "/home/ratty/Downloads/pocketsphinx-main/examples/test_ckd_alloc_fail")
set_tests_properties(test_ckd_alloc_fail PROPERTIES  WILL_FAIL "TRUE" _BACKTRACE_TRIPLES "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;22;add_test;/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;0;")
add_test(test_ckd_alloc_abort "/usr/bin/bash" "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/test_ckd_alloc_abort.sh")
set_tests_properties(test_ckd_alloc_abort PROPERTIES  ENVIRONMENT "CMAKE_BINARY_DIR=/home/ratty/Downloads/pocketsphinx-main/examples" _BACKTRACE_TRIPLES "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;24;add_test;/home/ratty/Downloads/pocketsphinx-main/test/unit/test_alloc/CMakeLists.txt;0;")
