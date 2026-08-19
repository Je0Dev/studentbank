# Constraints

These rules apply to every commit in this repository. Reviews must enforce them.

## Git workflow

- `main` is protected: **never commit directly to it**.
- **Every feature, test, or fix gets its own branch** created from an up-to-date
  `main`, then merged with a merge commit. Example:
  - `feat/<name>` for features,
  - `test/<name>` for new or updated tests,
  - `fix/<name>`, `docs/<name>`, `chore/<name>` for the rest.
- Commit messages use Conventional Commits (`feat:`, `test:`, `fix:`, `docs:`,
  `chore:`). See [CONTRIBUTING.md](CONTRIBUTING.md) for the full workflow.
- Tests must pass on a branch before it is merged into `main`.

## Merge approval

- **When changes are pushed to GitHub and the user has not explicitly said
  "merge", never merge to `main` automatically.**
- Instead, push the work on its own branch and leave it for the user to review
  and approve the merge on GitHub.
- If the user asks the agent to merge on their behalf, that explicit request
  is approval enough - the agent may then merge and push `main`.

## Code size

- Every source file must stay within **100-120 lines maximum**.
- If a file grows past the limit, split it (e.g. separate `ui_accounts.c` from
  `ui_students.c`, or a dedicated `ui_models.c` for GObject row types).
- Header files should stay small (declarations only, no logic).

## Functions

- Keep functions small, focused, and single-purpose. **No monolithic functions**
  (a function must not load data, run the business logic, and build the UI at once).
- Prefer several small helpers over one large function.
- A function that becomes hard to read should be split into named helpers.

## Comments

- Comment *why*, not what - the code should already show what it does.
- Every public function gets a short doc comment in the header.
- Keep comments brief and to the point.

## Simplicity

- Keep things simple and direct. Avoid clever one-liners and unnecessary abstraction.
- Use existing helpers (e.g. `validation_*`, `student_*`, `account_*`) instead of
  duplicating logic.
- No `/* TODO: ... */` or unfinished code may be committed.

## Tests

- Tests are required for **important features**: validation rules, account
  management, transactions, and access control. Not every function needs a
  test - small glue helpers and trivial UI wrappers do not.
- Put tests in `tests/`, registered in `CMakeLists.txt` and runnable via `ctest`.
- Pure logic lives in testable modules (`src/core/`, `src/persistence/`),
  separate from GTK4 UI code.
- All tests must pass before merging (`ctest --test-dir build`).

## Maintainability & scalability

- Code must be maintainable, scalable, and optimal where it matters:
  - no `O(n^2)` linked-list scans in hot loops without justification,
  - fixed arrays sized from `constants.h`,
  - deterministic and side-effect-free helper functions where possible.
- Build cleanly with `-Wall -Wextra` and no warnings.

## Style

- C99, standard library only, GTK4 for the GUI.
- Braces on the same line as the control statement (K&R style).
- `snake_case` for functions and variables, `UPPER_SNAKE` for constants,
  `GTK`/`g_*` names come from GTK4 itself.
- Order helpers top-down: static helpers above the public functions that use them.
