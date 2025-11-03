# CMake generated Testfile for 
# Source directory: /home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat
# Build directory: /home/ratty/Downloads/pocketsphinx-main/examples/test/unit/test_feat
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_test_feat.test "/usr/bin/bash" "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat/_test_feat.test")
set_tests_properties(_test_feat.test PROPERTIES  ENVIRONMENT "CMAKE_BINARY_DIR=/home/ratty/Downloads/pocketsphinx-main/examples" _BACKTRACE_TRIPLES "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat/CMakeLists.txt;26;add_test;/home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat/CMakeLists.txt;0;")
add_test(test_feat_live "/home/ratty/Downloads/pocketsphinx-main/examples/test_feat_live")
set_tests_properties(test_feat_live PROPERTIES  ENVIRONMENT "CMAKE_BINARY_DIR=/home/ratty/Downloads/pocketsphinx-main/examples" _BACKTRACE_TRIPLES "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat/CMakeLists.txt;28;add_test;/home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat/CMakeLists.txt;0;")
add_test(test_feat_fe "/home/ratty/Downloads/pocketsphinx-main/examples/test_feat_fe")
set_tests_properties(test_feat_fe PROPERTIES  ENVIRONMENT "CMAKE_BINARY_DIR=/home/ratty/Downloads/pocketsphinx-main/examples" _BACKTRACE_TRIPLES "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat/CMakeLists.txt;28;add_test;/home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat/CMakeLists.txt;0;")
add_test(test_subvq "/home/ratty/Downloads/pocketsphinx-main/examples/test_subvq")
set_tests_properties(test_subvq PROPERTIES  ENVIRONMENT "CMAKE_BINARY_DIR=/home/ratty/Downloads/pocketsphinx-main/examples" _BACKTRACE_TRIPLES "/home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat/CMakeLists.txt;28;add_test;/home/ratty/Downloads/pocketsphinx-main/test/unit/test_feat/CMakeLists.txt;0;")
