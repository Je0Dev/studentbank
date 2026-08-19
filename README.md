# Student Bank

A GTK4 student-accounts management system, fused from the ideas of two older
CLI projects (`cli_atm_system` and `cli_student_database_management_system`).
Students hold bank accounts whose **type** is tied to their profile:
**Department**, **Erasmus**, or **Notes/Study fund** — with department-gated
access control and audit logging.

## Features

- **Student records** - add, edit, delete, and search students (AM, name,
  surname, department, Erasmus flag, notes).
- **Profile-based accounts** - each student owns accounts typed by their
  department, Erasmus status, or study notes.
- **Banking** - PIN-protected login per account, deposit, withdraw, balance.
- **Access control** - admin and per-department users gate write operations.
- **Persistence & backups** - binary data file with automatic timestamped
  backups and rotation.
- **Audit logging** - every sensitive operation is written to `student_bank.log`.
- **Dashboard** - live totals for students, accounts, and balances.

## Requirements

- CMake 3.16+ and a C99 compiler (GCC/Clang).
- GTK4 development headers (platform-specific, see below).

## Installation

### Linux (Fedora / RHEL)

```bash
sudo dnf install gtk4-devel cmake gcc
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

### Linux (Debian / Ubuntu)

```bash
sudo apt install libgtk-4-dev cmake gcc
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

### macOS

```bash
brew install gtk4 cmake
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

### Windows

1. Install [MSYS2](https://www.msys2.org) and open its UCRT64 shell.
2. Install the required packages:

   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gtk4 \
             mingw-w64-ucrt-x86_64-cmake \
             mingw-w64-ucrt-x86_64-gcc
   ```

3. Build and test:

   ```bash
   cmake -B build && cmake --build build
   ctest --test-dir build --output-on-failure
   ```

4. Run the app:

   ```bash
   ./build/student_bank.exe
   ```

## Run

```bash
./build/student_bank
```

Data is stored in `student_bank.dat`, logs in `student_bank.log`, and backups
in `backups/`.

## Credentials

| Role        | Username    | Password     | Access                            |
|-------------|-------------|--------------|-----------------------------------|
| Admin       | `admin`     | `securepass` | All departments                   |
| Mechanical  | `mech_user` | `mechpass`   | Department 11 only                |
| Electrical  | `elec_user` | `elecpass`   | Department 12 only                |
| Civil       | `civil_user`| `civilpass`  | Department 13 only                |

Student AM format: `1[1-3][001-999]` (second digit is the department code).

## Development

See [CONTRIBUTING.md](CONTRIBUTING.md) for the git workflow and
[CONSTRAINTS.md](CONSTRAINTS.md) for code rules. The roadmap lives in
[PLAN.md](PLAN.md).

## License

MIT. See [LICENSE](LICENSE).
