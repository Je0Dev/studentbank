cmake_minimum_required(VERSION 3.16)
project(student_bank C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

set(CORE_SOURCES
    src/core/student.c
    src/core/search.c
    src/core/account.c
    src/core/transaction.c
    src/core/auth.c
    src/core/validation.c
)

set(PERSISTENCE_SOURCES
    src/persistence/storage.c
    src/persistence/backup.c
    src/persistence/logging.c
)

set(UI_SOURCES
    src/ui/main.c
    src/ui/ui_window.c
    src/ui/ui_models.c
    src/ui/ui_dialogs.c
    src/ui/ui_alerts.c
    src/ui/ui_login.c
    src/ui/ui_dashboard.c
    src/ui/ui_students.c
    src/ui/ui_student_actions.c
    src/ui/ui_student_edit_delete.c
    src/ui/ui_accounts.c
    src/ui/ui_account_actions.c
    src/ui/ui_account_add_delete.c
)

set(INCLUDE_DIRS include)

# --- application executable ---
add_executable(student_bank
    ${CORE_SOURCES}
    ${PERSISTENCE_SOURCES}
    ${UI_SOURCES}
)
target_include_directories(student_bank PRIVATE ${INCLUDE_DIRS})
target_link_libraries(student_bank PRIVATE PkgConfig::GTK4 m)

# --- tests (pure modules only, no GTK) ---
enable_testing()
add_executable(test_student_bank
    tests/test_main.c
    tests/test_validation.c
    tests/test_student.c
    tests/test_account.c
    tests/test_transaction.c
    tests/test_auth.c
    ${CORE_SOURCES}
    src/persistence/logging.c
)
target_include_directories(test_student_bank PRIVATE ${INCLUDE_DIRS} tests)
target_link_libraries(test_student_bank PRIVATE m)
add_test(NAME student_bank_tests COMMAND test_student_bank)
