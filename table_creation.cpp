#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void createTable(double num1, double num2, const string& outputPath) {
    // Open the output file
    ofstream outputFile;
    outputFile.open(outputPath, ios::app);

    if (outputFile.is_open()) {
        // Write header line for input numbers
        //outputFile << "For Number 1 = " << num1 << " and Number 2 = " << num2 << ":" << endl;

        // Write a top border for the table
        //outputFile << "+---------------+---------------+---------------+---------------+---------------+" << endl;

        // Write column headers
        outputFile << " Serial Number |   Addition    |  Subtraction  | Multiplication |   Division   |" << endl;

        

        // Write data rows
        for (int i = 1; i <= 1; ++i) {
            double addResult = num1 + num2;
            double subResult = num1 - num2;
            double mulResult = num1 * num2;
            double divResult = (num2 != 0) ? (num1 / num2) : 0.0;

            // Write data row with centered elements in each box
            //outputFile << "|";
            outputFile << setw(15) << std::left << i;
            outputFile << setw(15) << std::fixed << std::setprecision(2) << std::left << addResult;
            outputFile << setw(15) << std::fixed << std::setprecision(2) << std::left << subResult;
            outputFile << setw(15) << std::fixed << std::setprecision(2) << std::left << mulResult;
            outputFile << setw(15) << std::fixed << std::setprecision(2) << std::left << divResult << endl;
        }

     

        // Write a blank line for separation
        outputFile << endl;

        // Close the file
        outputFile.close();
        cout << "Table created and saved to '" << outputPath << "'" << endl;
    } else {
        cerr << "Error: Unable to open the output file." << endl;
    }
}

int main() {
    // Get two numbers from the user
    double num1, num2;

    cout << "Enter the first number: ";
    cin >> num1;

    cout << "Enter the second number: ";
    cin >> num2;

    // Specify the output file path
    string outputPath = "table_create.txt";
    // /home/user/CCPP/HelloWorld/

    // Create the table and save it to the specified directory
    createTable(num1, num2, outputPath);

    return 0;
}
