#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

// Structure to represent an animal
struct Animal {
    string species;
    string name;
    string sex;
    int age;
    string color;
    double weight;
    string birthDate;
    string arrivalDate;
};

// Function to generate a unique ID for each animal
string genUniqueID(const string& species, int index) {
    ostringstream oss;
    oss << species.substr(0, 2) << setw(2) << setfill('0') << index;
    return oss.str();
}

// Function to parse the input line and extract animal details
Animal parseAnimalDetails(const string& line, const string& name) {
    istringstream iss(line);
    string species, sex, color, birthSeason, park, country;
    int age;
    double weight;

    iss >> age >> sex >> species;
    getline(iss, color, ',');
    iss >> weight >> birthSeason >> park >> country;

    // Generate a unique ID based on species and index
    static int index = 1;
    string uniqueID = genUniqueID(species, index++);

    // Calculate birth date from birth season (simplified logic)
    string birthDate = "UNKNOWN";
    if (birthSeason == "spring")
        birthDate = "2022-03-21"; // Assuming year is always 2022
    // Similar logic for other seasons

    return { species, name, sex, age, color, weight, birthDate };
}

// Function to read animal names from file
vector<string> readAnimalNames(const string& filename) {
    vector<string> names;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }
    string name;
    while (getline(file, name)) {
        names.push_back(name);
    }
    return names;
}

// Function to process arriving animals and assign names
vector<Animal> processArrivingAnimals(const string& filename, const vector<string>& names) {
    vector<Animal> animals;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }
    string line;
    int index = 0;
    while (getline(file, line)) {
        if (index >= names.size()) {
            cerr << "Error: Insufficient names in animalNames.txt" << endl;
            exit(1);
        }
        Animal animal = parseAnimalDetails(line, names[index]);
        animals.push_back(animal);
        index++;
    }
    return animals;
}

// Function to categorize animals into habitats
map<string, vector<Animal>> categorizeIntoHabitats(const vector<Animal>& animals) {
    map<string, vector<Animal>> habitats;
    for (const auto& animal : animals) {
        habitats[animal.species].push_back(animal);
    }
    return habitats;
}

// Function to write the zoo population report to file
void writeZooPopulationReport(const string& filename, const map<string, vector<Animal>>& habitats) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }
    for (const auto& entry : habitats) {
        file << entry.first << " Habitat:" << endl;
        for (const auto& animal : entry.second) {
            file << animal.name << "; birth date " << animal.birthDate << "; "
                << animal.color << " color; " << animal.sex << "; "
                << animal.weight << " pounds; arrived " << animal.arrivalDate << endl;
        }
        file << endl;
    }
}

int main() {
    // Read animal names from file
    vector<string> names = readAnimalNames("animalNames.txt");

    // Process arriving animals and assign names
    vector<Animal> animals = processArrivingAnimals("arrivingAnimals.txt", names);

    // Categorize animals into habitats
    map<string, vector<Animal>> habitats = categorizeIntoHabitats(animals);

    // Write the zoo population report to file
    writeZooPopulationReport("zooPopulation.txt", habitats);

    cout << "Zoo population report has been generated successfully." << endl;
    return 0;
}
