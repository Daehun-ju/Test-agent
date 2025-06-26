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

def find_and_count_lines(folder_path, main_folder_names_list=None):
    """
Recursively finds C++ source and header files ('.cpp', '.h', '.hpp')
in the specified folder.

If main_folder_names_list is provided, aggregates counts for files whose
paths contain any of the specified main folder names as a directory component.
The keys in the returned 'aggregated_counts' dictionary will be these main folder names.

Otherwise (if main_folder_names_list is None), aggregates counts by the specific
directory path where files are found. The keys in 'aggregated_counts' will be
these directory paths.

Returns:
    tuple: (aggregated_counts (dict), overall_grand_total_lines (int))
        - aggregated_counts: Dictionary mapping aggregation key to line count.
        - overall_grand_total_lines: Total lines from ALL valid files scanned,
          regardless of main folder matching.
    """
    valid_extensions = ('.cpp', '.h', '.hpp')
    # This grand_total_lines will sum up ALL lines from ALL valid files found,
    # regardless of whether they fall into a main_folder or not.
    # The calling function will decide how to interpret this based on main_folder_names_list.
    overall_grand_total_lines = 0

    # This dictionary will store {key: line_count}.
    # If main_folder_names_list is active, key is a main_folder_name.
    # Otherwise, key is an actual directory path (normalized).
    aggregated_counts = {}

    if main_folder_names_list:
        for name in main_folder_names_list:
            aggregated_counts[name] = 0 # Initialize counts for all requested main folders
    else:
        # For default mode, initialize count for the starting scan path
        normalized_folder_path = os.path.normpath(folder_path)
        aggregated_counts[normalized_folder_path] = 0

    print(f"Searching for files in: {folder_path}\n")

    for current_dir_path, _, files in os.walk(folder_path):
        normalized_current_dir = os.path.normpath(current_dir_path)

        if not main_folder_names_list:
            # Default behavior: ensure each traversed directory is a key for aggregation
            if normalized_current_dir not in aggregated_counts:
                aggregated_counts[normalized_current_dir] = 0

        for file_name in files:
            if file_name.endswith(valid_extensions):
                file_path = os.path.join(current_dir_path, file_name)
                lines = count_lines_in_file(file_path)

                if lines >= 0:
                    overall_grand_total_lines += lines # Sum all lines found

                    if main_folder_names_list:
                        # Check if this file_path falls under any main_folder_name
                        # A file path can be /.../main_folder_name/.../file.cpp
                        # We need to correctly attribute this to 'main_folder_name'
                        found_in_main_folder = False
                        for name in main_folder_names_list:
                            # Check if 'name' is a component in the file's directory path
                            # e.g., if name is "app-service" and file_path is "/a/b/app-service/c/d.cpp"
                            # os.path.sep is important for platform independence.
                            # Adding os.sep ensures we match whole folder names, not partials.
                            # E.g., searching for "service" doesn't match "app-service".
                            # We look for "/name/" or "name/" at start or "/name" at end of dir.
                            # A simpler check: is 'name' one of the directory parts?
                            # Path components include the file name itself if not careful.
                            # So, check against the directory of the file.

                            # Check if the main folder name is part of the path segments
                            # Path().parts gives directory components.
                            # Example: /usr/local/my-service/data -> ('/', 'usr', 'local', 'my-service', 'data')
                            # We need to check against os.path.dirname(file_path)
                            # A bit tricky to make it robust against /name vs /name-extended
                            # Let's split the dirname and check parts.
                            # Example: main_folder "foo", path "/bar/foo/baz/file.cpp" -> parts of dirname: "bar", "foo", "baz"
                            # Example: main_folder "foo", path "/bar/foobar/baz/file.cpp" -> should not match "foo"

                            # Using path components and checking for exact match:
                            # Add leading/trailing slashes to name for robust matching within path string
                            # This ensures "name" matches "/name/" or "path/name/" etc.
                            # but not "path/pre-name-suffix/"
                            # A file's path string: /a/b/main_folder/c/file.cpp
                            # A main_folder: main_folder
                            # We need to see if main_folder is a directory in the path.
                            # Search for os.sep + name + os.sep in path, or name + os.sep at start, or os.sep + name at end (if it's a dir)

                            # More robust: get path components of the file's directory
                            # file_dir_parts = Path(os.path.dirname(file_path)).parts doesn't work on PosixPath/WindowsPath well here
                            # Let's use string splitting on os.sep for the directory of the file
                            # Relative path from the initial folder_path might be better for matching.
                            relative_file_dir = os.path.dirname(os.path.relpath(file_path, folder_path))
                            path_components = relative_file_dir.split(os.sep)
                            # Also check against the components of the absolute directory
                            abs_file_dir_components = os.path.dirname(file_path).split(os.sep)

                            if name in path_components or name in abs_file_dir_components:
                                aggregated_counts[name] += lines
                                found_in_main_folder = True
                                break # Count once per file for the first matching main folder

                        # If main_folder_names_list is active, but the file didn't fall into any of them,
                        # its lines are counted in overall_grand_total_lines but not in any specific main_folder's sum.
                        # This is important for the final grand total calculation in __main__.

                    else:
                        # Default behavior: add to current directory's count
                        # normalized_current_dir is the key for aggregated_counts here
                        aggregated_counts[normalized_current_dir] += lines

    return aggregated_counts, overall_grand_total_lines

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Counts lines in C++ source and header files (.cpp, .h, .hpp) "
                    "in a specified folder (or current directory if no folder is provided) and its subfolders.\n"
                    "By default, outputs total lines per discovered folder that contains relevant files.\n"
                    "If --main-folders is specified, output is restricted to totals for those main folders only.",
        formatter_class=argparse.RawTextHelpFormatter # Allows newlines in description
    )
    parser.add_argument(
        "folder",
        type=str,
        nargs='?',  # Makes the argument optional
        default=os.getcwd(),  # Default to current working directory
        help="The path to the folder to scan. Defaults to the current directory if not provided."
    )
    parser.add_argument(
        "--main-folders",
        type=str,
        help="A comma-separated list of main folder base names to report on. "
             "If provided, output will be restricted to these folders."
    )

    args = parser.parse_args()
    target_folder = args.folder
    main_folders_list = [name.strip() for name in args.main_folders.split(',')] if args.main_folders else None

    if not os.path.isdir(target_folder):
        print(f"Error: Folder not found at '{target_folder}'")
    else:
        # Pass main_folders_list to the counting function
        aggregated_counts, overall_scanned_total_lines = find_and_count_lines(target_folder, main_folders_list)

        if aggregated_counts:
            if main_folders_list:
                print("\nLine counts for specified main folders:")
                # Filter out main folders that were specified but had no lines, unless user wants to see all specified.
                # For now, only show main folders that ended up with lines.
                # Or, show all specified in main_folders_list, with their counts (even if 0). Let's do this.
                output_grand_total = 0
                for folder_name in main_folders_list: # Iterate in the order they were specified
                    lines = aggregated_counts.get(folder_name, 0) # Get count, default to 0 if not found (should be initialized)
                    print(f"  {folder_name}: {lines} lines")
                    output_grand_total += lines
                print(f"\nGrand total lines for specified main folders: {output_grand_total}")
                # Optionally, you could also print overall_scanned_total_lines if it's different and relevant.
                # print(f"Total lines scanned in target directory: {overall_scanned_total_lines}")
            else:
                # Default behavior (no --main-folders specified)
                print("\nLine counts per folder:")
                initial_scan_path = os.path.normpath(target_folder)
                for folder, lines in sorted(aggregated_counts.items()):
                    if lines > 0 or os.path.normpath(folder) == initial_scan_path:
                        print(f"  {folder}: {lines} lines")
                # In default mode, overall_scanned_total_lines is the correct grand total to display.
                print(f"\nGrand total lines found: {overall_scanned_total_lines}")
        else:
            # This handles if aggregated_counts is empty.
            # If main_folders_list was given, it would be initialized, so this implies no files found at all.
            # If not main_folders_list, target_folder would be in aggregated_counts.
            print("No matching files found, or an issue occurred during scanning.")
