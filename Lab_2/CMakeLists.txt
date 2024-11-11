cmake_minimum_required(VERSION 3.23)
project(Lab_2)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)

# Добавление исходных файлов
set(SOURCES
        main.cpp
)

# Добавление заголовочных файлов


# Создание исполняемого файла
add_executable(AVLTreeExecutable ${SOURCES} ${HEADERS})