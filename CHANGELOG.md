# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/), and
this project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

## [0.1.0] - 2026-08-19

### Added

- Fused GTK4 app combining student records (AM, name, department, Erasmus,
  notes) with bank accounts (PIN login, deposit/withdraw/balance).
- Account types tied to the student profile: Department, Erasmus, Notes.
- Department-based access control (admin + per-department users).
- Binary persistence with automatic timestamped backups and rotation.
- Audit logging to `student_bank.log`.
- ctest suite covering validation, students, accounts, transactions, access.
- Repository docs: CONSTRAINTS.md, CONTRIBUTING.md, AGENTS.md, PLAN.md.

### Fixed

- Stack switcher tabs were invisible because pages were added with
  `gtk_stack_add_named` (no title), which hides the switcher buttons; switched
  to `gtk_stack_add_titled`.
