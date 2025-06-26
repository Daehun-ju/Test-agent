import os
import argparse

def count_lines_in_file(filepath):
    """Counts the number of lines in a single file."""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            return len(f.readlines())
    except Exception as e:
        print(f"Error reading file {filepath}: {e}")
        return 0

def find_and_count_lines(folder_path):
    """
Recursively finds C++ source and header files ('.cpp', '.h', '.hpp')
in the specified folder (or current working directory if no path is given)
and its subfolders. It counts the number of lines in each valid file
and prints the individual counts and a grand total.

Usage:
  python count_lines.py [folder_path]

If folder_path is omitted, the script will scan the current working directory.

Examples:
  python count_lines.py ./my_project_src
  python count_lines.py
    """
    valid_extensions = ('.cpp', '.h', '.hpp')
    total_lines = 0
    processed_files_details = [] # Stores tuples of (filepath, lines)

    print(f"Searching for files in: {folder_path}\n")

    for root, _, files in os.walk(folder_path):
        for file in files:
            if file.endswith(valid_extensions):
                filepath = os.path.join(root, file)
                lines = count_lines_in_file(filepath)
                if lines >= 0: # Count even if file is empty (0 lines)
                    processed_files_details.append((filepath, lines))
                    total_lines += lines
                # If count_lines_in_file returned a negative or error indicator,
                # it would have printed an error and we just skip adding it.

    return processed_files_details, total_lines

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Counts lines in C++ source and header files (.cpp, .h, .hpp) "
                    "in a specified folder (or current directory if no folder is provided) and its subfolders."
    )
    parser.add_argument(
        "folder",
        type=str,
        nargs='?',  # Makes the argument optional
        default=os.getcwd(),  # Default to current working directory
        help="The path to the folder to scan. Defaults to the current directory if not provided."
    )

    args = parser.parse_args()
    target_folder = args.folder

    if not os.path.isdir(target_folder):
        print(f"Error: Folder not found at '{target_folder}'")
    else:
        processed_files_details, grand_total_lines = find_and_count_lines(target_folder)

        if processed_files_details:
            print("\nLine counts per file:")
            for filepath, lines in processed_files_details:
                print(f"  {filepath}: {lines} lines")
            print(f"\nGrand total lines found: {grand_total_lines}")
        else:
            print("No matching files found, or all found files were empty/unreadable.")
