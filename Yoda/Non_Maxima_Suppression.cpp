#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int IMAGE_WIDTH = 128;
const int IMAGE_HEIGHT = 128;
const int THRESHOLD = 100;

// Define a struct for holding a pixel's (x, y) position and its intensity value
struct Pixel {
    int x;
    int y;
    int intensity;
};

// Define a function for applying non-maxima suppression to an image
void nonMaximaSuppression(int image[IMAGE_WIDTH][IMAGE_HEIGHT]) {
    vector<Pixel> pixels;

    // Collect all pixels with intensity greater than the threshold
    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            if (image[x][y] > THRESHOLD) {
                Pixel p;
                p.x = x;
                p.y = y;
                p.intensity = image[x][y];
                pixels.push_back(p);
            }
        }
    }

    // Apply non-maxima suppression to the collected pixels
    for (int i = 0; i < pixels.size(); i++) {
        Pixel p = pixels[i];

        // Check the four neighboring pixels in the horizontal and vertical directions
        int x1 = max(p.x - 1, 0);
        int y1 = max(p.y - 1, 0);
        int x2 = min(p.x + 1, IMAGE_WIDTH - 1);
        int y2 = min(p.y + 1, IMAGE_HEIGHT - 1);

        bool isMax = true;

        for (int y = y1; y <= y2; y++) {
            for (int x = x1; x <= x2; x++) {
                if (x == p.x && y == p.y) {
                    continue;
                }

                if (image[x][y] >= p.intensity) {
                    isMax = false;
                    break;
                }
            }

            if (!isMax) {
                break;
            }
        }

        // Set the intensity of non-maximum pixels to zero
        if (!isMax) {
            image[p.x][p.y] = 0;
        }
    }
}

int main() {
    int image[IMAGE_WIDTH][IMAGE_HEIGHT];

    // Read the input image from a file
    /*
    ifstream inFile("input.txt");

    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            inFile >> image[x][y];
        }
    }

    inFile.close();
    */
    // Apply non-maxima suppression to the image
    nonMaximaSuppression(image);

    // Output the resulting image as a text file
    ofstream outFile("Non_maxima_suppression_output.txt");

    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            outFile << image[x][y] << " ";
        }
        outFile << endl;
    }

    outFile.close();

    return 0;
}
