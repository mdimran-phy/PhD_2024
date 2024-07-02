#include <iostream>

int main() {
    // Constants and variables based on your problem statement
    const int fMcpActive_x = 108;  // Active size of MCP in x-direction (mm)
    const int fMcpActive_y = 108;  // Active size of MCP in y-direction (mm)
    const int fNRow = 3;           // Number of rows of MCPs
    const int fNCol = 2;           // Number of columns of MCPs
    const double darkNoiseRate = 10000;  // Dark noise rate in Hz/mm²

    // Calculate the total active area of one MCP
    int A_mcp = fMcpActive_x * fMcpActive_y;  // Area in mm²

    // Calculate the dark noise per MCP
    double DN_mcp = A_mcp * darkNoiseRate;  // Dark noise in Hz

    // Calculate the number of PMTs
    int N_pmt = fNRow * fNCol;  // Total number of PMTs

    // Calculate the dark noise per PMT
    double DN_pmt = DN_mcp / N_pmt;  // Dark noise in Hz per PMT

    // Output the results
    std::cout << "Total active area per MCP: " << A_mcp << " mm²" << std::endl;
    std::cout << "Dark noise per MCP: " << DN_mcp << " Hz" << std::endl;
    std::cout << "Number of PMTs: " << N_pmt << std::endl;
    std::cout << "Dark noise per PMT: " << DN_pmt << " Hz" << std::endl;

    return 0;
}
