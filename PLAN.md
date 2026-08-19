# student_bank — plan

A GTK4 student-accounts management system that fuses the two CLI projects
`cli_atm_system` and `cli_student_database_management_system` into one new
project. Students (from the DB) own bank accounts (from the ATM); account
types are tied to their profile — **Department**, **Erasmus**, or
**Notes/Study fund** — with department-gated access control.

## Workflow rule (always)

- Implement **one feature at a time**: finish it (test + code + verify) before
  starting the next one.
- **Test first**: write the unit test, see it fail, then implement the feature
  until the test passes.
- Verify each feature: build clean, `ctest` green, `wc -l` <= 120 on every
  file, zero `-Wall -Wextra` warnings.
- Commit each feature on its own, then move on. No commits to `main` unless
  the user explicitly says "merge".
- See [CONSTRAINTS.md](CONSTRAINTS.md) and [CONTRIBUTING.md](CONTRIBUTING.md).
- General GTK4 reference: <https://www.gtk.org/docs/>.

## Prerequisite

GTK4 is not installed on this machine (`sudo` needs a password, so it is run
by the user):

```bash
sudo dnf install gtk4-devel
```

Verify with `pkg-config --modversion gtk4` (expect 4.22.1). Display is live
on `:0` (Wayland), so the app can run.

## Project overview

```
merging/
├── CMakeLists.txt          # gtk4 via pkg-config, ctest, pure-module test target
├── CONSTRAINTS.md          # merged + adapted from bananakong/freshiki (GTK4/C99)
├── CONTRIBUTING.md         # merged + adapted (conventional commits, branch workflow)
├── LICENSE                 # MIT, "Copyright (c) 2026 student_bank contributors"
├── README.md
├── CHANGELOG.md            # Keep a Changelog, Unreleased section
├── AGENTS.md               # dev-machine notes + commands + gotchas
├── PLAN.md                 # this file
├── include/
│   ├── constants.h         # sizes, account types, department codes, paths
│   ├── student.h           # Student struct + CRUD/search prototypes
│   ├── account.h           # Account struct + account management prototypes
│   ├── transaction.h       # deposit/withdraw/balance prototypes
│   ├── auth.h              # session, authenticate_user, department access
│   └── storage.h           # save/load, backups, logging prototypes
├── src/
│   ├── core/               # pure, testable logic (no GTK)
│   │   ├── student.c       # create/find/delete/edit students
│   │   ├── search.c        # search by AM/name/department/partial, list all
│   │   ├── account.c       # create/add/remove/find accounts, number generation
│   │   ├── transaction.c   # deposit, withdraw, balance checks
│   │   ├── auth.c          # user authentication + department access matrix
│   │   └── validation.c    # AM/name/PIN validation, name formatting, sanitize
│   ├── persistence/        # pure, testable (no GTK)
│   │   ├── storage.c       # save_database / load_database (binary)
│   │   ├── backup.c        # create_backup / rotate_backups
│   │   └── logging.c       # log_message / log_error / log_audit
│   └── ui/                 # GTK4 layer
│       ├── main.c          # GtkApplication entry point
│       ├── ui_window.c     # header bar + GtkStack (Dashboard/Students/Accounts/Login)
│       ├── ui_dashboard.c  # stats overview (records, balance totals, services)
│       ├── ui_students.c   # GtkColumnView list + add/edit/delete + search
│       ├── ui_accounts.c   # account list, deposit/withdraw dialogs
│       ├── ui_login.c      # admin/department login dialog
│       └── ui_dialogs.c    # shared dialog builders (prompt, confirm, input)
└── tests/                  # ctest harness (bananakong-style test_util.h)
    ├── test_util.h
    ├── test_main.c
    ├── test_student.c
    ├── test_account.c
    ├── test_transaction.c
    └── test_auth.c
```

## Fused data model

- **Student** (from student DB): `AM[6]` (department encoded in 2nd digit),
  `name`, `surname`, `department` (1=Mech, 2=Elec, 3=Civil), `is_erasmus`,
  `notes[256]`, linked list of accounts.
- **Account** (from ATM): `number` (unique), `pin[5]`, `balance`, `type`
  (`ACCT_DEPARTMENT`, `ACCT_ERASMUS`, `ACCT_NOTES`), owner pointer.
- **Access** (from student DB): admin + per-department users gate writes;
  PIN login per account gates transactions.

## Feature mapping

| From cli_atm_system              | From cli_student_database_management_system |
|----------------------------------|----------------------------------------------|
| PIN login                        | Student CRUD + search (AM/name/dept/regex)   |
| Deposit / withdraw / balance     | Department-based access control              |
| Multiple accounts per customer   | Audit logging                                |
| Admin view of all accounts       | Auto-backups with rotation                   |
| Data persistence (CSV)           | System statistics                            |

New fused behavior: account **type** is derived from the student profile
(department / Erasmus status / notes) instead of free-form accounts.

## Implementation order

### 1. Scaffold (docs + build first)
- [x] Write `CMakeLists.txt`: C99, `-Wall -Wextra`, gtk4 via pkg-config;
      `student_bank` executable; `test_student_bank` target compiling only
      `src/core/*.c` + `src/persistence/*.c` (no GTK); `enable_testing()`.
- [x] Write `include/constants.h` and the remaining headers.
- [x] Write `CONSTRAINTS.md`, `CONTRIBUTING.md`, `LICENSE` (adapted from
      bananakong + freshiki), plus `README.md`, `CHANGELOG.md`, `AGENTS.md`.
- [x] `tests/test_util.h` + `tests/test_main.c` harness (copy pattern).

### 2. Core modules, test-first
- [x] `validation.c`: `validate_am`, `validate_name`, `validate_pin`,
      `format_name`, `sanitize_input` → `tests/test_validation.c`
- [x] `student.c`: create/find/delete/edit + duplicate AM check →
      `tests/test_student.c`
- [x] `account.c`: create/add/remove/find, unique number, max-accounts →
      `tests/test_account.c`
- [x] `transaction.c`: deposit/withdraw/insufficient funds →
      `tests/test_transaction.c`
- [x] `auth.c`: admin + department credentials, access matrix →
      `tests/test_auth.c`
- [x] `search.c`: AM/name/department/partial search + sorted list
- [x] Every step: write test → see it fail → implement → `ctest` green.

### 3. Persistence
- [x] `storage.c`: binary save/load of students + accounts
- [x] `backup.c`: timestamped backups, rotation to `MAX_BACKUPS`
- [x] `logging.c`: mutex-free structured log (INFO/WARN/ERROR/AUDIT)

### 4. GTK4 UI
- [x] `main.c`: `GtkApplication`, activate → `ui_window`
- [x] `ui_window.c`: header bar + `GtkStackSwitcher` over the four views
- [x] `ui_login.c` + `ui_dialogs.c`: auth dialog, shared prompt/confirm dialogs
- [x] `ui_dashboard.c`: totals + service status readouts
- [x] `ui_students.c`: `GtkColumnView` table, add/edit/delete/search dialogs
- [x] `ui_accounts.c`: accounts view, deposit/withdraw dialogs
- [x] Wire sessions: department access gates write ops; PIN gates transactions.

### 5. Verify & ship
- [x] `cmake -B build && cmake --build build` clean, zero `-Wall -Wextra`
      warnings
- [x] `ctest --test-dir build --output-on-failure` all green
- [x] `wc -l` <= 120 on every source file
- [x] Launch `./build/student_bank` on `:0`, smoke-test login + one
      transaction + one student edit

## Open items

- [x] Final project/binary name (`student_bank` here — confirmed by user).
- [ ] Whether `notes` means free-text study notes or a funded account note; the
      account type exists regardless, so this stays flexible.