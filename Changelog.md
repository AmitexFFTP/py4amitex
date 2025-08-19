# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html)
(for the Python interface).

## Unreleased

### Added

- More flexible input file generation (#2)

## 1.1.0 - 2025-07-25

### Added

- Input: restart feature (#3).
- Add new pure phase builder for Mérope (!9)
- Output_Diffusion: Introduce an interface to read load and post-process results from Amitex simulations of diffusion processes (#1)

### Changed

- Composite: allow to define zone for each phase even if same material (!3)
- Input: Pointer to classes in C++  to harmonize with python (!4)

### Fixed

- Handling of 0 time increment for vtks

## 1.0.0 - 2024-07-08

