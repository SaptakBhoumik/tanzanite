clang++ `pkg-config --cflags --libs gtk4` compiler/errors/*.cpp compiler/**/**/*.cpp test.cpp renderer/**/*.cpp renderer/*.cpp -Icompiler/ -Irenderer/ -Icompiler/