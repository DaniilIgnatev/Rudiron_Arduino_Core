# This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

# Arduino_Core_Rudiron_MDR32F9Qx is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Arduino_Core_Rudiron_MDR32F9Qx is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Arduino_Core_Rudiron_MDR32F9Qx. If not, see <https://www.gnu.org/licenses/>.

# Author: Daniil Ignatev


import os
import re
import argparse


def scan_files_in_folder(folder_path):
    # Check if the provided path is a directory
    if not os.path.isdir(folder_path):
        print(f"{folder_path} is not a valid directory.")
        return

    # List all files in the directory
    entries = []
    for entry in os.scandir(folder_path):
        if entry.is_file():
            entries.append(entry)
        else:
            scan_files_in_folder(entry.path)

    # Print the list of files
    if entries:
        for entry in entries:
            if is_header(entry.name) or is_source(entry.name):
                print(entry.name)

                modify_license(entry.path)


def is_source(file: str):
    extention = file.split(".")[-1]

    if extention == "cpp":
        return True

    if extention == "c":
        return True

    return False


def is_header(file: str):
    extention = file.split(".")[-1]

    if extention == "hpp":
        return True

    if extention == "h":
        return True

    return False


def license_lines():
    license_str = """// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif
"""
    license_lines = license_str.splitlines(keepends=False)

    return license_lines


def modify_license(file_path: str):
    content = read_file_lines(file_path)
    content_original = content.copy()

    Aquarius_license_start, Aquarius_license_end = license_region(content)
    if Aquarius_license_start:
        remove_range_in_place(content, Aquarius_license_start, Aquarius_license_end)

    empty_line_index = first_empty_line(content)
    # content = make_empty_line(content, empty_line_index)
    license = license_lines()
    content = insert_lines(content, license, empty_line_index)

    # print("\n".join(content))
    write_file_lines(file_path, content)

    # Test
    Aquarius_license_start, Aquarius_license_end = license_region(content)
    remove_range_in_place(content, Aquarius_license_start, Aquarius_license_end)
    empty_line_index = first_empty_line(content)
    # content = make_empty_line(content, empty_line_index)
    content = insert_lines(content, license, empty_line_index)

    Aquarius_license_start2, Aquarius_license_end2 = license_region(content)
    remove_range_in_place(content, Aquarius_license_start, Aquarius_license_end)
    content_rollback = content.copy()
    assert Aquarius_license_start2 == Aquarius_license_start
    assert Aquarius_license_end2 == Aquarius_license_end

    # for i in range(len(content_original)):
    #     if i >= len(content_rollback):
    #         print(f"In {file_path}: i >= len(content_rollback)")
    #         break
            
    #     line_original = content_original[i]
    #     line_new = content_rollback[i]

    #     if not line_new == line_original:
    #         print(f"In {file_path}: {line_original} != {line_new}")


def read_file_lines(file_path) -> list:
    try:
        with open(file_path, "r", encoding = 'utf8') as file:
            lines = file.readlines()
            lines = [line[:-1] for line in lines]

            return lines
    except FileNotFoundError:
        print(f"The file '{file_path}' does not exist.")


def write_file_lines(file_path, lines: list):
    with open(file_path, "w", encoding = 'utf8') as file:
        # Write each line to the file
        for line in lines:
            file.write(line + "\n")


def license_region(lines: list):
    start = find_first_occurrence(lines, "Aquarius")
    if start:
        start -= 2

    end = find_last_occurrence(lines, "HelloStringLength")
    if end:
        end += 2

    return (start, end)


# def make_empty_line(lines: list, index: int):
#     last_empty_line = lines[index]
#     if last_empty_line != '' and last_empty_line != '\n':
#         return insert_lines(lines, [''], index)
#     else:
#         return lines


def first_empty_line(lines: list):
    line_numbers = []
    line_numbers.append(find_first_occurrence(lines, "#define"))
    line_numbers.append(find_first_occurrence(lines, "#ifndef"))
    line_numbers.append(find_first_occurrence(lines, "#include"))
    line_numbers.append(find_first_occurrence(lines, "#pragma once"))

    line_numbers.append(find_first_occurrence(lines, "/* Includes"))
    line_numbers.append(find_first_occurrence(lines, "/* Define"))

    line_numbers = [number for number in line_numbers if number is not None]

    line_numbers.sort()

    if len(line_numbers) > 0:
        result = line_numbers[0]
        result -= 1
    else:
        result = len(line_numbers) - 1

    return result


def find_first_occurrence(lines, word):
    word_pattern = re.compile(re.escape(word))
    for line_number, line in enumerate(lines, start=0):
        if word_pattern.search(line):
            # print(f"The word '{word}' is first found on line {line_number}.")
            return line_number
    # print(f"The word '{word}' was not found.")
    return None


def find_last_occurrence(lines, word):
    word_pattern = re.compile(re.escape(word))
    last_occurrence_line = None
    for line_number, line in enumerate(lines, start=0):
        if word_pattern.search(line):
            last_occurrence_line = line_number

    if last_occurrence_line is not None:
        # print(f"The word '{word}' is last found on line {last_occurrence_line}.")
        return last_occurrence_line
    else:
        # print(f"The word '{word}' was not found.")
        return None


def remove_range_in_place(lst, start, end):
    if start and end:
        del lst[start:end]


def insert_lines(target_lines, insert_lines, index):
    return target_lines[:index] + insert_lines + target_lines[index:]


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Rudiron library licence administration script')
    parser.add_argument('folder_path', type=str, help='The root folder of source files')
    args = parser.parse_args()

    folder_path = args.folder_path
    scan_files_in_folder(folder_path)