/*
 * Copyright (C) 2024 nyaaaww
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "mpkg.h"

long get_file_size(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if (file == NULL) {
		perror("Error opening file for size check");
		return -1;
	}
	long size;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);
	fclose(file);
	return size;
}

void load_entry(FILE *file, int total, MPKGFileEntry *mpkg_files)
{
	for (int i = 0; i < total; i++) {
		fread(&(mpkg_files[i].file_name_length), 4, 1, file);
		fread(&(mpkg_files[i].file_name), mpkg_files[i].file_name_length, 1, file);
		fread(&(mpkg_files[i].index), 4, 1, file);
		fread(&(mpkg_files[i].file_size), 4, 1, file);

		printf("\nName long : %u\n", mpkg_files[i].file_name_length);
		printf("Name : %s\n", mpkg_files[i].file_name);
		printf("File long : %u\n", mpkg_files[i].file_size);
	}
}

int create_dir_recursive(const char *path)
{
	char *cpy_path = strdup(path);
	if (!cpy_path) {
		perror("strdup failed");
		return -1;
	}

	char *cur_path = cpy_path;
	while (*cur_path) {
		if (*cur_path == '/') {
			*cur_path = '\0'; // temp cut dir to create
			if (mkdir(cpy_path, 0777) == -1 && errno != EEXIST) { // check dir is exists.
				free(cpy_path);
				return -1;
			}
			*cur_path = '/';
		}
		cur_path++;
	}

	free(cpy_path);
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "A tool decode the wallpaper engine's mpkg file.\n");
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE *file;
	MPKGHeader mpkg_header;
	size_t result;
	int cdr;

	file = fopen(argv[1], "rb");
	if (file == NULL) {
		perror("Error opening file");
		return -1;
	}

	// read mpkgfile header
	result = fread(&mpkg_header, sizeof(mpkg_header), 1, file);
	if (result != 1) {
		perror("Error reading file");
		fclose(file);
		return -1;
	}
	printf("Version Long: %u\n", mpkg_header.version_length);
	printf("Version : %.8s\n", mpkg_header.version);
	printf("Num of file : %u\n", mpkg_header.file_total);

	// read mpkgfile entries
	MPKGFileEntry *mpkg_files = (MPKGFileEntry *)malloc(mpkg_header.file_total * sizeof(MPKGFileEntry));
	load_entry(file, mpkg_header.file_total, mpkg_files);
	// copy files from mpkgfile

	for (int i = 0; i < mpkg_header.file_total; i++) {
		char *buf;
		buf = (char *)malloc(mpkg_files[i].file_size + 1);
		fread(buf, mpkg_files[i].file_size, 1, file);
		cdr = create_dir_recursive(mpkg_files[i].file_name);
		if (cdr == -1) {
			perror("create dir failed");
			return -1;
		}

		FILE *wfile;
		wfile = fopen(mpkg_files[i].file_name, "wb");
		if (wfile == NULL) {
			perror("Error opening file");
			return EXIT_FAILURE;
		}

		size_t bytes_written;
		bytes_written = fwrite(buf, mpkg_files[i].file_size, 1, wfile);
		if (bytes_written != 1) {
			perror("Error writing to file");
			fclose(wfile);
			return EXIT_FAILURE;
		}

		free(buf);
		fclose(wfile);
	}
	free(mpkg_files);
	fclose(file);
	return 0;
}
