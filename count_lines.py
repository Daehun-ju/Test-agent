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
    grand_total_lines = 0
    folder_line_counts = {} # Stores {folder_path: line_count}

    # Normalize the input folder path to ensure consistent keying for the root folder
    normalized_folder_path = os.path.normpath(folder_path)
    if normalized_folder_path not in folder_line_counts:
        folder_line_counts[normalized_folder_path] = 0


    print(f"Searching for files in: {folder_path}\n")

    for root, _, files in os.walk(folder_path):
        current_folder_total_lines = 0
        normalized_root = os.path.normpath(root)

        if normalized_root not in folder_line_counts:
            folder_line_counts[normalized_root] = 0

        for file in files:
            if file.endswith(valid_extensions):
                filepath = os.path.join(root, file)
                lines = count_lines_in_file(filepath)
                if lines >= 0: # Count even if file is empty (0 lines)
                    folder_line_counts[normalized_root] += lines
                    grand_total_lines += lines
                # If count_lines_in_file returned an error, it's already printed

    # Clean up folders with zero lines if they weren't the initial path and had no sub-files
    # Or, keep them if you want to explicitly show folders that were scanned but empty.
    # For now, let's keep them to show they were scanned.
    # If a folder had files but they all had 0 lines, it will be listed with 0.

    return folder_line_counts, grand_total_lines

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Counts lines in C++ source and header files (.cpp, .h, .hpp) "
                    "in a specified folder (or current directory if no folder is provided) and its subfolders. "
                    "Outputs total lines per folder."
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
        folder_line_counts, grand_total_lines = find_and_count_lines(target_folder)

        if folder_line_counts:
            print("\nLine counts per folder:")
            initial_scan_path = os.path.normpath(target_folder)
            # Sort by folder path for consistent output
            for folder, lines in sorted(folder_line_counts.items()):
                # Only print folders that have lines or was the initial explicitly scanned path
                if lines > 0 or os.path.normpath(folder) == initial_scan_path:
                    print(f"  {folder}: {lines} lines")

            print(f"\nGrand total lines found: {grand_total_lines}")
        else:
            # This case should ideally not be hit if target_folder exists,
            # as folder_line_counts would at least contain the target_folder itself (possibly with 0 lines).
            # However, keeping it for robustness.
            print("No matching files found, or an issue occurred during scanning.")
