# Smart Vocabulary Display

Smart Vocabulary Display is a vocabulary-retention project that reinforces learning through repeated exposure. A mobile application sends vocabulary entries to an ESP32 over Bluetooth Low Energy (BLE), and the ESP32 presents each entry on an OLED display.

## Project Goal

Improve vocabulary retention by repeatedly showing a word, its definition, and an example sentence throughout the day.

## First Prototype Scope

The first prototype is designed for a single user and uses English-only vocabulary entries.

Each entry contains:

- A word
- A short definition
- A short example sentence

The OLED presents the three fields on separate screens in this order:

1. Word
2. Definition
3. Example

The complete sequence repeats while the entry remains active. An entry stays active for at least one hour, and the device advances to the next entry only after completing the current word-definition-example sequence.

Scrolling, paging, and text truncation are not supported. Vocabulary content must therefore fit within the available screen layout.

Restarting the active-entry timer after a power loss is acceptable for the first prototype.

## System Overview

The planned system consists of:

- A mobile application for managing vocabulary entries
- BLE communication between the mobile application and the ESP32
- An ESP32 that stores and schedules received entries
- An OLED display that presents the active entry

AI-assisted vocabulary generation is planned for a later stage and is not part of the first prototype.

## Hardware

- ESP32 DevKit V1
- 2.42-inch SSD1309 128×64 SPI OLED

## Firmware Environment

- PlatformIO
- Arduino framework
- U8g2 display library
