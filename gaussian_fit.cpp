#include <iostream>
#include <cmath>
#include <TCanvas.h>
#include <TGraph.h>
#include <TF1.h>
#include <TFitResult.h>

// Define the Gaussian function
double gaussian(double *x, double *par) {
    double amplitude = par[0];
    double mean = par[1];
    double stddev = par[2];
    
    return amplitude * exp(-0.5 * pow((x[0] - mean) / stddev, 2));
}

void gaussian_fit() {
    // Create a TGraph with example data points
    const int nPoints = 10;
    double xData[nPoints] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    double yData[nPoints] = {10.0, 25.0, 40.0, 70.0, 90.0, 110.0, 80.0, 50.0, 30.0, 15.0};
    TGraph *graph = new TGraph(nPoints, xData, yData);

    // Create a TF1 function with the Gaussian function
    TF1 *fitFunction = new TF1("fitFunction", gaussian, 1.0, 10.0, 3); // Assuming x range is [1.0, 10.0]

    // Set initial parameter values for the fit
    fitFunction->SetParameter(0, 100.0); // initial amplitude guess
    fitFunction->SetParameter(1, 5.0);   // initial mean guess
    fitFunction->SetParameter(2, 2.0);   // initial standard deviation guess

    // Perform the fit
    TFitResultPtr fitResult = graph->Fit(fitFunction, "S");

    // Print fit results
    std::cout << "Fit Results:" << std::endl;
    std::cout << "Amplitude: " << fitResult->Parameter(0) << std::endl;
    std::cout << "Mean: " << fitResult->Parameter(1) << std::endl;
    std::cout << "Standard Deviation: " << fitResult->Parameter(2) << std::endl;

    // Create a canvas and draw the data and fit function
    TCanvas *canvas = new TCanvas("canvas", "Fit Canvas", 800, 600);
    graph->Draw("AP");
    fitFunction->Draw("same");

    // Update the canvas
    canvas->Update();

    // Wait for user interaction (close the canvas to proceed)
    canvas->WaitPrimitive();
}

int main() {
    gaussian_fit();

    return 0;
}
