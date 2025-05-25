#pragma once
#include "raylib.h"
#include <cstdlib>

#define STORAGE_DATA_FILE "highscore.data"

// Save integer value to storage file (at a defined position)
static bool SaveStorageValue(unsigned int position, int value) {
    bool success = false;
    int dataSize = 0;
    unsigned char* fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);
    unsigned char* newFileData = nullptr;

    // If file doesn’t exist or is empty, allocate space for one integer
    if (fileData == nullptr || dataSize == 0) {
        newFileData = (unsigned char*) calloc(1, sizeof(int));
        if (newFileData == nullptr) {
            // Failed to allocate memory
            if (fileData != nullptr) UnloadFileData(fileData);
            return false;
        }
        dataSize = sizeof(int);
    }
    else {
        // If file exists, ensure it has enough space for the position
        if ((position + 1) * sizeof(int) > dataSize) {
            newFileData = (unsigned char*) realloc(fileData, (position + 1) * sizeof(int));
            if (newFileData == nullptr) {
                // Failed to reallocate memory
                UnloadFileData(fileData);
                return false;
            }
            // Zero out new space
            for (int i = dataSize; i < (position + 1) * sizeof(int); i++) {
                newFileData[i] = 0;
            }
            dataSize = (position + 1) * sizeof(int);
        }
        else {
            newFileData = fileData;
        }
    }

    // Write the value at the specified position
    *(int*) (newFileData + position * sizeof(int)) = value;
    success = SaveFileData(STORAGE_DATA_FILE, newFileData, dataSize);

    // Clean up
    if (fileData != newFileData && fileData != nullptr) {
        UnloadFileData(fileData);
    }
    if (newFileData != nullptr) {
        free(newFileData); // Use free() for calloc/realloc memory
    }

    return success;
}

// Load integer value from storage file (at a defined position)
static int LoadStorageValue(unsigned int position) {
	int value = 0;
	int dataSize = 0;
	unsigned char* fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);

	// Check if the position exists in the file
	if (fileData != nullptr && (position + 1) * sizeof(int) <= dataSize) {
		value = *(int*) (fileData + position * sizeof(int));
	}

	if (fileData != nullptr) {
		UnloadFileData(fileData);
	}

	return value;
}