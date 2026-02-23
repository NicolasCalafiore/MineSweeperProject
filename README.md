# MineSweeperProject

A C++ implementation of the classic Minesweeper game built with SFML, designed with reusable object-oriented GUI and gameplay components.

## Project Overview

This project recreates the classic Minesweeper experience with a custom component-based architecture. Core game behavior and UI features are implemented through reusable classes (such as `Tile`, `Timer`, `Counter`, and `TextField`) to support clean structure and extensibility.

## Tech Stack

- Language: C++
- Graphics Library: SFML 2.5.1
- Compiler: MinGW

## Technical Highlights

- Implemented full Minesweeper game flow (reveal, flag, win/loss states, restart, pause, debug view)
- Added recursive reveal behavior for empty tiles to propagate board discovery
- Developed custom UI systems for timer digits, mine counter, face-state control, and button interactions
- Added leaderboard support with file-based read/write, sorting, and score formatting
- Separated rendering responsibilities from game logic through dedicated UI rendering helpers
- Created reusable GUI/game components for maintainable feature development

## Core Components

- `main.cpp`: window lifecycle, event loop, game state transitions, and high-level orchestration
- `Tile.h`: tile state model, mine/flag behavior, adjacency logic, and click propagation
- `Button.h`: reusable sprite-based interactive base component
- `Timer.h`: elapsed-time tracking and timer sprite position logic
- `Counter.h`: mine counter state and digit mapping
- `TextField.h`: reusable text input handling for player name entry
- `UIRenderer.h`: centralized rendering for tiles, controls, timer, and counter
- `FileIO.h`: config loading, texture loading, and leaderboard persistence

## Lessons Learned

- **Event-driven architecture:** structuring game behavior around a real-time input and rendering loop
- **Recursive state propagation:** implementing controlled flood-style reveal mechanics for zero-adjacent tiles
- **Data persistence design:** handling leaderboard parsing, sorting, and formatting through deterministic file I/O
- **Reusable OOP component design:** building modular classes to separate concerns and enable reuse across gameplay and UI systems
