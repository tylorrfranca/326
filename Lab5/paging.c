#include <stdio.h>
#include <stdbool.h>
#define MAX_FRAMES 10
#define MAX_PAGES 50

// Function to check if a page is in memory
bool isPagePresent(int page, int frames[], int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        if (frames[i] == page)
            return true;
    }
    return false;
}

// Function to print memory state
void printMemoryState(int frames[], int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        if (frames[i] == -1)
            printf(" - ");
        else
            printf(" %d ", frames[i]);
    }
    printf("\n");
}

// FIFO Page Replacement Algorithm
int fifo(int pages[], int pageCount, int frameSize) {
    int frames[MAX_FRAMES];
    int pageFaults = 0;
    int currentIndex = 0;
    
    // Initialize frames with -1 (empty)
    for (int i = 0; i < frameSize; i++) {
        frames[i] = -1;
    }
    
    printf("\nFIFO Page Replacement:\n");
    for (int i = 0; i < pageCount; i++) {
        printf("\nReference page %d: ", pages[i]);
        
        if (!isPagePresent(pages[i], frames, frameSize)) {
            frames[currentIndex] = pages[i];
            currentIndex = (currentIndex + 1) % frameSize;
            pageFaults++;
            printf("Page Fault! ");
        } else {
            printf("No Page Fault ");
        }
        
        printMemoryState(frames, frameSize);
    }
    
    return pageFaults;
}

// LRU Page Replacement Algorithm
int lru(int pages[], int pageCount, int frameSize) {
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];
    int pageFaults = 0;
    
    // Initialize frames with -1 (empty)
    for (int i = 0; i < frameSize; i++) {
        frames[i] = -1;
        lastUsed[i] = 0;
    }
    
    printf("\nLRU Page Replacement:\n");
    for (int i = 0; i < pageCount; i++) {
        printf("\nReference page %d: ", pages[i]);
        
        if (!isPagePresent(pages[i], frames, frameSize)) {
            // Find the least recently used page
            int lruIndex = 0;
            for (int j = 1; j < frameSize; j++) {
                if (lastUsed[j] < lastUsed[lruIndex])
                    lruIndex = j;
            }
            
            frames[lruIndex] = pages[i];
            lastUsed[lruIndex] = i;
            pageFaults++;
            printf("Page Fault! ");
        } else {
            // Update last used time for the page
            for (int j = 0; j < frameSize; j++) {
                if (frames[j] == pages[i]) {
                    lastUsed[j] = i;
                    break;
                }
            }
            printf("No Page Fault ");
        }
        
        printMemoryState(frames, frameSize);
    }
    
    return pageFaults;
}

int main() {
    int pages[MAX_PAGES], pageCount, frameSize;
    
    // Input
    printf("Enter the number of pages: ");
    scanf("%d", &pageCount);
    
    printf("Enter the page reference sequence: ");
    for (int i = 0; i < pageCount; i++) {
        scanf("%d", &pages[i]);
    }
    
    printf("Enter the number of frames: ");
    scanf("%d", &frameSize);
    
    // Run FIFO
    int fifoFaults = fifo(pages, pageCount, frameSize);
    printf("\nTotal Page Faults (FIFO): %d\n", fifoFaults);
    
    // Run LRU
    int lruFaults = lru(pages, pageCount, frameSize);
    printf("\nTotal Page Faults (LRU): %d\n", lruFaults);
    
    return 0;
}
