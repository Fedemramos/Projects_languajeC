#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average value of RGB and assign it to each component
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixels horizontally
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    // Create a temporary copy of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // Apply blur to each pixel in the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            // Iterate over neighboring pixels
            for (int ni = i - 1; ni <= i + 1; ni++)
            {
                for (int nj = j - 1; nj <= j + 1; nj++)
                {
                    // Check if the neighboring pixel is within bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        // Accumulate color values
                        sumRed += temp[ni][nj].rgbtRed;
                        sumGreen += temp[ni][nj].rgbtGreen;
                        sumBlue += temp[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            // Assign the average color values to the original image
            image[i][j].rgbtRed = round((float)sumRed / count);
            image[i][j].rgbtGreen = round((float)sumGreen / count);
            image[i][j].rgbtBlue = round((float)sumBlue / count);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Sobel operator kernels
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE temp[height][width];

    // Create a temporary copy of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // Apply Sobel operator to each pixel in the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int GxRed = 0, GxGreen = 0, GxBlue = 0;
            int GyRed = 0, GyGreen = 0, GyBlue = 0;

            // Iterate over neighboring pixels using the Sobel kernels
            for (int ni = -1; ni <= 1; ni++)
            {
                for (int nj = -1; nj <= 1; nj++)
                {
                    int ai = i + ni;
                    int aj = j + nj;

                    // Check if the neighboring pixel is within bounds
                    if (ai >= 0 && ai < height && aj >= 0 && aj < width)
                    {
                        // Apply Sobel operator
                        GxRed += temp[ai][aj].rgbtRed * Gx[ni + 1][nj + 1];
                        GxGreen += temp[ai][aj].rgbtGreen * Gx[ni + 1][nj + 1];
                        GxBlue += temp[ai][aj].rgbtBlue * Gx[ni + 1][nj + 1];

                        GyRed += temp[ai][aj].rgbtRed * Gy[ni + 1][nj + 1];
                        GyGreen += temp[ai][aj].rgbtGreen * Gy[ni + 1][nj + 1];
                        GyBlue += temp[ai][aj].rgbtBlue * Gy[ni + 1][nj + 1];
                    }
                }
            }

            // Calculate final color values using the magnitude of gradients
            int finalRed = round(sqrt(GxRed * GxRed + GyRed * GyRed));
            int finalGreen = round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen));
            int finalBlue = round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue));

            // Cap values at 255
            image[i][j].rgbtRed = finalRed > 255 ? 255 : finalRed;
            image[i][j].rgbtGreen = finalGreen > 255 ? 255 : finalGreen;
            image[i][j].rgbtBlue = finalBlue > 255 ? 255 : finalBlue;
        }
    }
}
