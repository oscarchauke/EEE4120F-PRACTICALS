#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

const int SIZE = 512;
#define high_threshold 50
#define low_threshold 20

// Create a SIZExSIZE output array to hold the result of the Gaussian smoothing operation
uint8_t inputArray[SIZE * SIZE];
uint8_t outputArray[SIZE * SIZE];
uint8_t sobelOutput[SIZE * SIZE];

void readTxTFile()
{
    ifstream inputFile("image.txt");
    // Check if the file was opened successfully
    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open input file." << endl;
        return;
    }

    // Read the decimal numbers from the file and populate the inputArray
    string line;
    int count = 0;
    while (getline(inputFile, line))
    {
        inputArray[count] = std::stoul(line);
        count++;
    }
    // Close the file
    inputFile.close();
}

// Function to perform Gaussian smoothing on a 1D array using a 3 by 3 kernel
void gaussianSmoothing(uint8_t input[], uint8_t output[])
{
    // Define the Gaussian kernel
    float kernel[3][3] = {
        {1 / 16.0, 2 / 16.0, 1 / 16.0},
        {2 / 16.0, 4 / 16.0, 2 / 16.0},
        {1 / 16.0, 2 / 16.0, 1 / 16.0}};

    // Loop through each pixel in the input array
    for (int y = 1; y < SIZE - 1; y++)
    {
        for (int x = 1; x < SIZE - 1; x++)
        {
            // Compute the weighted average of the surrounding pixels using the Gaussian kernel
            float sum = 0.0;
            for (int j = -1; j <= 1; j++)
            {
                for (int i = -1; i <= 1; i++)
                {
                    sum += kernel[j + 1][i + 1] * input[(y + j) * SIZE + (x + i)];
                }
            }
            // Set the output pixel value to the weighted average
            output[y * SIZE + x] = (uint8_t)sum;
        }
    }
}

void hysteresis(uint8_t input[], uint8_t output[])
{
    // Apply hysteresis thresholding
    for (int y = 1; y < SIZE - 1; y++)
    {
        for (int x = 1; x < SIZE - 1; x++)
        {
            int i = y * SIZE + x;
            if (input[i] > high_threshold)
            {
                output[i] = 255;
            }
            else if (input[i] > low_threshold)
            {
                // Check neighboring pixels
                if (input[i - 1] > high_threshold ||
                    input[i + 1] > high_threshold ||
                    input[i - SIZE] > high_threshold ||
                    input[i + SIZE] > high_threshold ||
                    input[i - SIZE - 1] > high_threshold ||
                    input[i - SIZE + 1] > high_threshold ||
                    input[i + SIZE - 1] > high_threshold ||
                    input[i + SIZE + 1] > high_threshold)
                {
                    output[i] = 255;
                }
            }
        }
    }
}

void sobel(uint8_t input[], uint8_t output[])
{
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sumx, sumy;
    for (int i = 1; i < SIZE - 1; i++)
    {
        for (int j = 1; j < SIZE - 1; j++)
        {
            sumx = 0;
            sumy = 0;
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    sumx += gx[k + 1][l + 1] * input[(i + k) * SIZE + j + l];
                    sumy += gy[k + 1][l + 1] * input[(i + k) * SIZE + j + l];
                }
            }
            output[i * SIZE + j] = (uint8_t)(sqrt(sumx * sumx + sumy * sumy) + 0.5);
        }
    }
}

int main()
{
    readTxTFile();

    std::ofstream myfile;
    std::string filename;

    // Perform Gaussian smoothing on the input array using the 1D kernel
    gaussianSmoothing(inputArray, outputArray);

    filename = "gaussian_output.txt";
    myfile.open(filename);
    printf("Creating gaussian output file ... \n");
    for (int j = 0; j < SIZE * SIZE; j++)
    {
        myfile << (int)outputArray[j] << "\n";
    }
    myfile.close();

    sobel(outputArray, sobelOutput);

    filename = "sobel_output.txt";
    myfile.open(filename);
    printf("Creating sobel output file ... \n");
    for (int j = 0; j < SIZE * SIZE; j++)
    {
        myfile << (int)sobelOutput[j] << "\n";
    }
    myfile.close();

    hysteresis(sobelOutput, outputArray);
    filename = "hysteresis_output.txt";
    myfile.open(filename);
    printf("Creating hysteresis output file ... \n");
    for (int j = 0; j < SIZE * SIZE; j++)
    {
        myfile << (int)outputArray[j] << "\n";
    }
    myfile.close();
    // Display the input and output arrays
    /* cout << "Input Array:" << endl;
     for (int y = 0; y < SIZE; y++)
     {
         for (int x = 0; x < SIZE; x++)
         {
             printf("%d\t", inputArray[y * SIZE + x]);
         }
         cout << endl;
     }
     cout << endl;

     cout << "Output Array:" << endl;
     for (int y = 0; y < SIZE; y++)
     {
         for (int x = 0; x < SIZE; x++)
         {
             printf("%d\t", outputArray[y * SIZE + x]);
         }
         cout << endl;
     }*/
    printf("Done");
    return 0;
}
