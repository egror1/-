# Подготовка к лабораторной работе 1

Минервин Егор, ЗРС2501.

Ветка lab_0 содержит Hello World, CMake и пример для GDB из работы 0.3.

```sh
cmake -S . -B build
cmake --build build
cmake --build build --target clang-format
./build/hello_world
cmake -S . -B build-debug -DENABLE_DEBUG=ON
cmake --build build-debug
cmake -S debugger -B debugger/build-debug
cmake --build debugger/build-debug
gdb -q -batch -x debugger/diagnostic.gdb debugger/build-debug/debugger_demo
```

При сборке используется C++20 и проверки -Wall -Wextra -Werror -Wpedantic; clang-tidy запускается CMake. Исходный пример отладки сохранён из методических указаний. После foo ожидаются a=5, s.a=11, s.b=15.25.

Источник: https://iu5git.github.io/cpp/
