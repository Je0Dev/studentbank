# AGENTS.md

GTK4 student-accounts manager in C99. Dev machine is Fedora 44 with
`gtk4-devel` 4.22 installed system-wide (shared lib only). The GUI runs here
via the Wayland session on `:0`. `gh` is authenticated to `Je0Dev`.

## Commands

- Configure/build: `cmake -B build && cmake --build build` (Release by
  default; sources are listed explicitly in `CMakeLists.txt`, no globbing)
- Tests: `ctest --test-dir build --output-on-failure`
- Run app: `./build/student_bank` (verifiable headlessly with
  `timeout 5 ./build/student_bank`, or with `GDK_BACKEND=x11` if needed)
- `build/` and `backups/` are gitignored staging areas

## Must-follow rules (full text: CONSTRAINTS.md, CONTRIBUTING.md)

- Never commit to `main`. Every change goes on its own branch (`feat/`,
  `test/`, `fix/`, `docs/`, `chore/`), merged with `--no-ff`.
- **Do not merge to or push `main` unless the user explicitly says "merge".**
  Otherwise push the branch and leave it for the user to approve.
- **One feature at a time**: test first (write test, see it fail, implement
  until green), then verify build + tests, then commit and move on.
- Conventional Commits; update `CHANGELOG.md` (Unreleased) with each push.
- Files <= 100-120 lines; no monolithic functions; no `TODO`; comment *why*;
  zero `-Wall -Wextra` warnings; build + tests green before merge.
- Tests are for **really important features** (validation, transactions,
  access control), not for everything implemented. Small glue helpers need no
  tests.

## Architecture

- `include/` - small headers, declarations only: `constants.h`, `student.h`,
  `account.h`, `transaction.h`, `auth.h`, `search.h`, `storage.h`, `ui.h`.
- `src/core/` - pure, testable logic (no GTK): `student.c`, `search.c`,
  `account.c`, `transaction.c`, `auth.c`, `validation.c`.
- `src/persistence/` - `storage.c` (binary save/load), `backup.c`,
  `logging.c`.
- `src/ui/` - GTK4 layer: `main.c` (GtkApplication), `ui_window.c`
  (header bar + GtkStack), `ui_models.c` (GObject row types for
  GtkColumnView), `ui_dialogs.c`, `ui_login.c`, `ui_dashboard.c`,
  `ui_students.c`, `ui_accounts.c`.
- Global state: `g_students` (head of student list), `g_session` (auth).

## Gotchas

- GTK4 `GtkColumnView` needs a `GListStore` of GObjects plus a
  `GtkSingleSelection`; row GObjects live in `ui_models.c`.
- The test target compiles only pure modules (`src/core/*.c` + `logging.c`);
  it must never pull in GTK headers. Add any new pure module to both targets
  in `CMakeLists.txt`.
- GTK4 signal handlers use `g_signal_connect`; always cast the user data with
  `G_OBJECT`/`gpointer` correctly to avoid crashes.
- Constant macros must not end in `_H` - they collide with header include
  guards. Use e.g. `MAX_NAME`, not `NAME_H`.
- Headers are included flat (`"student.h"`), not by relative path - the
  `include/` dir is on the include path via CMake.
