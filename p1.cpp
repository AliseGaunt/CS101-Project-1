#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

void Help(string* cmd_list) {
    cout << "command list:" << endl;
    for (size_t i = 0; i < cmd_list->length(); i++)
        cout << "\t" << cmd_list[i] << endl;
    cout << endl;
}

void Print(string** table, int col_width, int row_count, int col_count) {
    for (int i = 0; i < row_count; ++i) {
        for (int j = 0; j < col_count; ++j) {
            cout << right << setw(col_width) << table[i][j];
        }
        cout << endl;
    }
    cout<<endl;
}

void Cols(string** table, int col_count) {
    for (int i=0; i<= col_count; i++)
        cout << table[0][i] << endl;
    cout << endl;
}

int FindColumnIndex(string columnNames[], int col_count, const string& col_name) {
    if (col_name == "*") return -2;
    for (int i = 0; i < col_count; ++i) {
        if (columnNames[i] == col_name) {
            return i;
        }
    }
    return -1; // Return -1 if the column name is not found
}
void SearchAndPrintRows(string** table, int col_width, string columnNames[], int row_count, int col_count, string col_name, string value) {
    int colIndex = FindColumnIndex(columnNames, col_count, col_name);

    // Invalid column cases
    if (colIndex == -1) {
        cout << "Invalid column \"" << col_name << "\"\n" << endl;
        return;
    }

    int cnt = 0;
    // Search * cases
    if (colIndex == -2) {
        for (int i = 1; i < row_count; ++i)
            for (int j = 0; j < col_count; ++j)
                if (table[i][j] == value)
                    cnt++;
        if (cnt == 0)
            cout << "No results\n" << endl;
        else {
            // Print the 1st line - column names
            for (int i = 0; i < col_count; ++i)
                cout << right << setw(col_width) << table[0][i];
            cout << endl;

            // Print the rows that match the condition
            for (int i = 1; i < row_count; ++i)
                for (int j = 0; j < col_count; ++j)
                    if (table[i][j] == value) {
                        for (int k = 0; k < col_count; ++k)
                            cout << right << setw(col_width) << table[i][k];
                        cout << endl;
                        break;
                    }
            cout << endl;
        }
        return;
    }

    // Normal cases
    // Count the number of rows match
    for (int i = 1; i < row_count; ++i) {
        if (table[i][colIndex] == value) {
            cnt++;
        }
    }
    if (cnt == 0)
        cout << "No results" << endl;
    else {
        // Print the 1st line - column names
        for (int i = 0; i < col_count; ++i)
            cout << right << setw(col_width) << table[0][i];
        cout << endl;

        // Print the rows that match the condition
        for (int i = 1; i < row_count; ++i) {
            if (table[i][colIndex] == value) {
                for (int j = 0; j < col_count; ++j) {
                    cout << right << setw(col_width) << table[i][j];
                }
                cout << endl;
            }
        }
    }
    cout << endl;
}

string RemoveQuotes(string& input) {
    string output = "";
    char ch;
    istringstream iss(input);

    while (iss >> noskipws >> ch) {
        if (ch != '\"') {
            output += ch;
        }
    }
    return output;
}

void Min(string** table, int row_count, int colIndex) {
    double minValue;
    bool isInitialized = false;

    for (int i = 1; i <= row_count; ++i) {
        try {
            if (table[i][colIndex].empty()) continue;  // Skip empty strings

            double value = stod(table[i][colIndex]);
            if (!isInitialized) {
                minValue = value;
                isInitialized = true;
            } else if (value < minValue) {
                minValue = value;
            }
        } catch (const invalid_argument&) {
            // Skip non-numeric values
            continue;
        }
    }
    if (!isInitialized) cout<<"The min for col \""<<table[0][colIndex]<<"\" = N/A\n";
    else cout << "The min for col \"" << table[0][colIndex] << "\" = " << minValue << endl;
}

void Max(string** table, int row_count, int colIndex) {
    double maxValue;
    bool isInitialized = false;

    for (int i = 1; i <= row_count; ++i) {
        try {
            if (table[i][colIndex].empty()) continue;  // Skip empty strings

            double value = stod(table[i][colIndex]);
            if (!isInitialized) {
                maxValue = value;
                isInitialized = true;
            } else if (value > maxValue) {
                maxValue = value;
            }
        } catch (const invalid_argument&) {
            // Skip non-numeric values
            continue;
        }
    }
    if (!isInitialized) cout<<"The max for col \""<<table[0][colIndex]<<"\" = N/A\n";
    else cout<<"The max for col \""<<table[0][colIndex]<<"\" = "<<maxValue<<endl;
}

void Avg(string** table, int row_count, int colIndex) {
    double sum;
    bool isInitialized = false;
    int cnt = 0;

    for (int i = 1; i <= row_count; ++i) {
        try {
            if (table[i][colIndex].empty()) continue;  // Skip empty strings

            double value = stod(table[i][colIndex]);
            if (!isInitialized) {
                sum = value;
                isInitialized = true;
                cnt++;
            } else {
                sum += value;
                cnt++;
            }
        } catch (const invalid_argument&) {
            // Skip non-numeric values
            continue;
        }
    }
    if (!isInitialized) cout<<"The avg for col \""<<table[0][colIndex]<<"\" = N/A\n";
    else cout<<"The avg for col \""<<table[0][colIndex]<<"\" = "<<sum / cnt<<endl;
}

int Quit(ifstream& csv_file) {
    // Close the file
    csv_file.close();
    return 0;
}
int main(int argc, char* argv[]) {
    // argument count, argument vector
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        cout << "Usage: ./a database.csv #col_width" << endl;
        return 1;
    }

    string csv_filename = argv[1];
    int col_width = 0;

    // Convert column width argument to integer
    istringstream column_width_stream(argv[2]);
    column_width_stream >> col_width;

    // Open the CSV file
    ifstream csv_file(csv_filename.c_str());
   if (!csv_file.is_open()) {
    cout << "Failed to open \"" << csv_filename << "\"" << endl;
    return 1;
}

    string line;
    int col_count = 0, row_count = 0;
    const int MAX_COL = 50, MAX_ROW = 5000;
    string** table = new string*[MAX_ROW];
    for (int i = 0; i < MAX_ROW; ++i) {
        table[i] = new string[MAX_COL];
    }

    // Count the number of commas
    size_t commaPos = 0;
    getline(csv_file, line);
    while (commaPos != string::npos && col_count < MAX_COL) {
        commaPos = line.find(',', commaPos + 1);
        col_count++;
    }

    // Read the header
    stringstream header(line);
    for (int i=0; i<= col_count; ++i)
    {
        string cell;
        getline(header, cell, ',');
        table[0][i] = cell;
    }
    row_count++;

    // Read the other lines
    while (getline(csv_file, line) && row_count < MAX_ROW) {
        stringstream ss(line);
        string cell;

        // Split line into cells
        for (int i=0; i<= col_count; ++i)
        {
            string cell;
            getline(ss, cell, ',');
            table[row_count][i] = cell;
        }

        row_count++;
    }

    // Save columnNames array
    string columnNames[col_count+1];
    for (int i=0; i<col_count; i++) {
        columnNames[i] = table[0][i];
    }

    string cmd_list[6] = {"print", "cols", "search *|col_name val", "min col_name", "max col_name", "avg col_name"};
    string command;
    // Read command
    while (true) {
        cout<<"Enter a command or \"help\" for a command list:\n";
        getline(cin, command);
        istringstream iss(command);
        string cmd;
        iss>>cmd; // Extract the command

        if (cmd == "help") {
            Help(cmd_list);
        } else if (cmd == "print") {
            Print(table, col_width, row_count, col_count);
        } else if (cmd == "cols") {
            Cols(table, col_count);
        } else if (cmd == "search") {
            string remaining, col_name, value;
            getline(iss, remaining); // Get the rest of the line
            remaining = remaining.substr(remaining.find_first_not_of(" \t")); // Trim leading spaces

            // Extract the column name
            size_t firstQuote = remaining.find('\"');
            if (firstQuote != string::npos && firstQuote == 0) {
                size_t secondQuote = remaining.find('\"', firstQuote + 1);
                if (secondQuote != string::npos) {
                    col_name = remaining.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                    remaining = remaining.substr(secondQuote + 1); // Remaining part after column name
                }
            } else {
                istringstream tempStream(remaining);
                tempStream >> col_name;
                getline(tempStream, remaining); // Remaining part after column name
            }

            remaining = remaining.substr(remaining.find_first_not_of(" \t")); // Trim leading spaces
            // Extract the value, handling quotes if present
            size_t thirdQuote = remaining.find('\"');
            if (thirdQuote != string::npos) {
                size_t endQuote = remaining.find('\"', thirdQuote + 1);
                if (endQuote != string::npos) {
                    value = remaining.substr(thirdQuote + 1, endQuote - thirdQuote - 1);
                }
            } else {
                value = remaining;
            }

            SearchAndPrintRows(table, col_width, columnNames, row_count, col_count, col_name, value);
        } else if (cmd == "min") {
            string remaining, col_name;
            getline(iss, remaining);
            remaining = remaining.substr(remaining.find_first_not_of(" \t")); // Trim leading spaces
            size_t firstQuote = remaining.find('\"');
            if (firstQuote != string::npos) {
                size_t secondQuote = remaining.find('\"', firstQuote + 1);
                if (secondQuote != string::npos) {
                    col_name = remaining.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                }
            } else {
                col_name = remaining;
            }
            int colIndex = FindColumnIndex(columnNames, col_count, col_name);
            if (colIndex == -1) cout<<"Invalid column \""<<col_name<<"\"\n";
            else Min(table, row_count, colIndex);
            cout<<endl;
        } else if (cmd == "max") {
            string remaining, col_name;
            getline(iss, remaining);
            remaining = remaining.substr(remaining.find_first_not_of(" \t")); // Trim leading spaces
            size_t firstQuote = remaining.find('\"');
            if (firstQuote != string::npos) {
                size_t secondQuote = remaining.find('\"', firstQuote + 1);
                if (secondQuote != string::npos) {
                    col_name = remaining.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                }
            } else {
                col_name = remaining;
            }
            int colIndex = FindColumnIndex(columnNames, col_count, col_name);
            if (colIndex == -1) cout<<"Invalid column \""<<col_name<<"\"\n";
            else Max(table, row_count, colIndex);
            cout<<endl;
        } else if (cmd == "avg") {
            string remaining, col_name;
            getline(iss, remaining);
            remaining = remaining.substr(remaining.find_first_not_of(" \t")); // Trim leading spaces
            size_t firstQuote = remaining.find('\"');
            if (firstQuote != string::npos) {
                size_t secondQuote = remaining.find('\"', firstQuote + 1);
                if (secondQuote != string::npos) {
                    col_name = remaining.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                }
            } else {
                col_name = remaining;
            }
            int colIndex = FindColumnIndex(columnNames, col_count, col_name);
            if (colIndex == -1) cout<<"Invalid column \""<<col_name<<"\"\n";
            else Avg(table, row_count, colIndex);
            cout<<endl;
        } else if (cmd == "quit") {
            Quit(csv_file);
            break;
        } else {
            cout<<"Invalid command\n"<<endl;
        }
    }


    return 0;
}
