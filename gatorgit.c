#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "gatorgit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (gatorgit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the homework spec.
 */

/* gatorgit init
 *
 * - Create .gatorgit directory
 * - Create empty .gatorgit/.index file
 * - Create .gatorgit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int gatorgit_init(void) {
    fs_mkdir(".gatorgit");

    FILE* findex = fopen(".gatorgit/.index", "w");
    fclose(findex);
  
    write_string_to_file(".gatorgit/.prev", "0000000000000000000000000000000000000000");

    return 0;
}

/* gatorgit add <filename>
 * 
 * - Append filename to list in .gatorgit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int gatorgit_add(const char* filename) {
    FILE* findex = fopen(".gatorgit/.index", "r");
    FILE* fnewindex = fopen(".gatorgit/.newindex", "w");

    char line[FILENAME_SIZE];
    while (fgets(line, sizeof(line), findex)) {
        strtok(line, "\n");
        if (strcmp(line, filename) == 0) {
            fprintf(stderr, "ERROR: File %s already added\n", filename);
            fclose(findex);
            fclose(fnewindex);
            fs_rm(".gatorgit/.newindex");
            return 3;
        }

        fprintf(fnewindex, "%s\n", line);
    }

    fprintf(fnewindex, "%s\n", filename);
    fclose(findex);
    fclose(fnewindex);

    fs_mv(".gatorgit/.newindex", ".gatorgit/.index");

    return 0;
}

/* gatorgit rm <filename>
 * 
 * See "Step 2" in the homework 1 spec.
 *
 */

int gatorgit_rm(const char* filename) {
    // Open the .index file for reading
    FILE* findex = fopen(".gatorgit/.index", "r");
    if (!findex) {
        fprintf(stderr, "ERROR: Could not open .index file\n");
        return 1;
    }

    // Temporary file to store updated list
    FILE* fnewindex = fopen(".gatorgit/.newindex", "w");

    char line[FILENAME_SIZE];
    int found = 0;

    // Copy all lines except the one to remove
    while (fgets(line, sizeof(line), findex)) {
        strtok(line, "\n");  // Remove newline
        if (strcmp(line, filename) == 0) {
            found = 1;  // Mark that the file was found
        } else {
            fprintf(fnewindex, "%s\n", line);
        }
    }

    fclose(findex);
    fclose(fnewindex);

    if (!found) {
        fprintf(stderr, "ERROR: File %s not tracked\n", filename);
        fs_rm(".gatorgit/.newindex");
        return 1;
    }

    // Replace original index with the updated version
    fs_mv(".gatorgit/.newindex", ".gatorgit/.index");
    return 0;
}

/* gatorgit commit -m <msg>
 *
 * See "Step 3" in the homework 1 spec.
 *
 */

const char* go_gator = "GOLDEN GATOR!";

int is_commit_msg_ok(const char* msg) {
    // Check if the message contains "GOLDEN GATOR!" (without using standard string functions)
    const char* p = msg;
    int i = 0;

    while (*p) {
        if (*p == go_gator[i]) {
            i++;
            if (go_gator[i] == '\0') {
                return 1;  // Found the complete string
            }
        } else {
            i = 0;  // Reset if the sequence breaks
        }
        p++;
    }
    return 0;  // Not found
}

void next_commit_id(char* commit_id) {
    // TODO: Implement logic to generate the next commit ID
}

int gatorgit_commit(const char* msg) {
    if (!is_commit_msg_ok(msg)) {
        fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_gator);
        return 1;
    }

    // Generate the next commit ID
    char commit_id[COMMIT_ID_SIZE + 1];
    next_commit_id(commit_id);

    // Create the commit directory inside .gatorgit/
    char commit_dir[FILENAME_SIZE];
    sprintf(commit_dir, ".gatorgit/%s", commit_id);
    printf("Creating directory: %s\n", commit_dir);  // Debug print

    // Use fs_mkdir to create the directory
    if (fs_mkdir(commit_dir) != 0) {
        fprintf(stderr, "ERROR: Failed to create directory %s\n", commit_dir);
        return 1;
    }

    // Copy tracked files into the new commit directory
    FILE* findex = fopen(".gatorgit/.index", "r");
    if (!findex) {
        fprintf(stderr, "ERROR: Could not open .index file\n");
        return 1;
    }

    char filename[FILENAME_SIZE];
    while (fgets(filename, sizeof(filename), findex)) {
        filename[strcspn(filename, "\n")] = '\0';  // Remove newline

        char dest[FILENAME_SIZE];
        sprintf(dest, "%s/%s", commit_dir, filename);
        printf("Copying %s to %s\n", filename, dest);  // Debug print

        fs_cp(filename, dest);
    }
    fclose(findex);

    // Write the commit message to .msg
    char msg_file[FILENAME_SIZE];
    sprintf(msg_file, "%s/.msg", commit_dir);
    write_string_to_file(msg_file, msg);

    // Update the .prev file with the new commit ID
    write_string_to_file(".gatorgit/.prev", commit_id);

    return 0;
}



/* gatorgit status
 *
 * Print the list of tracked files and the total count.
 */

int gatorgit_status() {
    // Open the .index file for reading
    FILE* findex = fopen(".gatorgit/.index", "r");
    if (!findex) {
        fprintf(stderr, "ERROR: Could not open .index file\n");
        return 1;
    }

    char line[FILENAME_SIZE];
    int file_count = 0;

    // Print the header
    fprintf(stdout, "Tracked files:\n");

    // Read each line from the .index file
    while (fgets(line, sizeof(line), findex)) {
        // Remove any trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Print the filename and increment the count
        fprintf(stdout, "  %s\n", line);
        file_count++;
    }

    // Print the total number of tracked files
    fprintf(stdout, "\n%d files total\n", file_count);

    fclose(findex);  // Close the file
    return 0;  // Return success
}
