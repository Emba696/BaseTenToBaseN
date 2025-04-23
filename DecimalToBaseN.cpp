#include <iostream>
#include <cstdint>
#include <cmath>
#include <string>
#include <limits>

double logBase(const double base, const double value) {
    return std::log2(value) / std::log2(base);
}

// Helper to extract the vakye if a symbol
char toSymbol(const double input) {
    int inputInt = std::trunc(input);
    //std::cout << input << " -> " << inputInt<< "\n" ;

    // Use '0' to '9', then 'A' to 'Z'
    if (inputInt < 10) {
        return '0' + inputInt;
    }
    else if (inputInt < 36) {
        inputInt -= 10;
        return 'A' + inputInt;
    }

    // Error
    return '!';
}

// Convert a double to a string in a given base
std::string toStringInBase(const double base, const double value, const double precision) {
    double remainingValue = value;
    std::string result = "";

    // Ensure that the base is more than 1
    if (base <= 1.0 || base > 36.0) {
        return result;
    }

    // Add negative sign if needed, also avoid infinite loops for 0
    if (value < 0.0) {
        result.push_back('-');
        remainingValue *= -1;
    }
    else if (value == 0.0) {
        result = "0.0";
        return result;
    }

    // Only 52 precision bits, with initial implied unless double = 0
    int maxSafeSymbolCount = std::trunc(logBase(base, precision));

    // Reserve for the symbols, the radix point, the parenthases, and 1 extra symbol
    result.reserve(maxSafeSymbolCount + 4);

    double power = std::trunc(logBase(base, remainingValue));
    // Power must be at least 0 to print leading zeros
    if (power < 0.0) {
        power = 0.0;
    }

    // Get the symbol for each power of the base
    size_t loopCounter;
    double currentPowerValue = std::pow(base, power);
    for (loopCounter = 0; (power >= 0) || (loopCounter <= maxSafeSymbolCount && remainingValue > 0.0 && (value + std::pow(base, power)) != value); ++loopCounter) {
        // Radix point
        if (power == -1) {
            result.push_back('.');
        }

        // Parenthases for potentially imprecise symbols
        if (loopCounter == maxSafeSymbolCount) {
            result.push_back('(');
        }

        // Get the proper symbol for the base
        char symbol = toSymbol(remainingValue / std::pow(base, power));
        result.push_back(symbol);

        // Subtract the added symbol value
        remainingValue -= currentPowerValue * std::trunc(remainingValue / currentPowerValue);

        --power;
        currentPowerValue = std::pow(base, power);
    }
    // End parenthases if needed
    if (loopCounter > maxSafeSymbolCount) {
        result.push_back(')');
    }

    return result;
}

double strToBaseTenDoubleWithPrecision(std::string& input, double& precision) {
    precision = 0.0;

    // Count the information digits
    size_t digitCount = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] >= '0' && input[i]  <= '9') {
            ++digitCount;
        }
    }

    // Get how many permutations they can hold
    precision = std::pow(10.0, (double)digitCount);

    // Can only be as precise as the float holding it
    // Also ensure that precision is not rounded if the input ends with &
    constexpr double maxSafeInt = 9007199254740992.0;
    if (input[input.length() - 1] == '&') {
        input.pop_back();
        precision = maxSafeInt;
    }
    else if (precision > maxSafeInt) {
        precision = maxSafeInt;
    }

    double result = 0.0;
    result = stod(input);

    return result;
}

int main() {
    std::string ioString = "";
    double base = 0;
    double input = 1;

    std::cout << "Number base: ";
    std::cin >> base;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the newline char as it messes with getline

    // Base must be at least -1 for now
    if (base < 1.0 || base > 36.0) {
        std::cout << "The base must be greater than 1 and at most 36 (for now)!\n";
        return -1;
    }

    std::cout << "\nEnter 0 once you are done.";
    std::cout << "\nEnd your numbers with & if they are not rounded.\n";
    while (input != 0.0) {
        double precision = 0.0;

        std::cout << "\nInput : ";
        std::getline(std::cin, ioString);
        input = strToBaseTenDoubleWithPrecision(ioString, precision);
        ioString = toStringInBase(base, input, precision);
        std::cout << "Output: " << ioString << "\n";
    }

    return 0;
}